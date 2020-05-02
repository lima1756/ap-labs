// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"strconv"
	"strings"
	"time"
)

type User struct {
	name    string
	admin   bool
	conn    net.Conn
	channel client
	kicked  bool
}

type message struct {
	from string
	to   string
	msg  string
}

type client chan<- string // an outgoing message channel

var (
	admin      = false
	entering   = make(chan *User)
	leaving    = make(chan *User)
	all_users  = make(chan *User)
	broadMsg   = make(chan string)  // broadcast messages
	allMsg     = make(chan message) // cast message (uni or multi)
	privateMsg = make(chan message)
	kicking    = make(chan message)
	info_user  = make(chan message)
	clients    = make(map[string]*User) // all connected clients
)

func messageManager() {
	for {
		select {
		case msg := <-privateMsg:
			go sendPrivateMessage(msg)
		case msg := <-allMsg:
			go sendToAllMessage(msg.from, msg.msg, msg.from)
			break
		case msg := <-broadMsg:
			for cli := range clients {
				go sendServerMessage(cli, msg)
			}
		case user := <-all_users:
			var users string
			for cli := range clients {
				users += cli + ", "
			}
			user.channel <- "irc-server > " + users
		case msg := <-info_user:
			if clients[msg.to] == nil {
				(*clients[msg.from]).channel <- "irc-server > User [" + msg.to + "] doesn't exist in the channel."
				break
			}
			(*clients[msg.from]).channel <- "irc-server > " + "username: " + msg.to + ", IP: " + clients[msg.to].conn.RemoteAddr().String()
		case msg := <-kicking:
			if clients[msg.to] == nil {
				(*clients[msg.from]).channel <- "irc-server > User [" + msg.to + "] doesn't exist in the channel."
				break
			}
			sendServerMessage(msg.to, "You can't send new messages")
			sendServerMessage(msg.to, "You were kicked from the server.")

			sendToAllMessage("irc-server", "["+msg.to+"] was kicked from the server.", msg.to)
			(*clients[msg.to]).kicked = true
			removeUser(clients[msg.to])
			fmt.Printf("irc-server > [%s] was kicked\n", msg.to)
		case cli := <-entering:
			fmt.Printf("irc-server > New connected user [%s]\n", cli.name)
			clients[cli.name] = cli
		case cli := <-leaving:
			removeUser(cli)
		}
	}
}

func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	input := bufio.NewScanner(conn)

	var who = User{}
	// scanning user name
	input.Scan()
	who = User{
		name:    input.Text(),
		admin:   !admin,
		conn:    conn,
		channel: ch,
		kicked:  false,
	}
	who.channel <- "irc-server > Welcome to the Simple IRC Server"
	who.channel <- "irc-server > Your user [" + who.name + "] is successfully logged"
	allMsg <- message{from: "irc-server", msg: "New connected user [" + who.name + "]"}
	entering <- &who

	if !admin {
		who.channel <- "irc-server > Congrats, you were the first user."
		who.channel <- "irc-server > You're the new IRC Server ADMIN"
		fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", who.name)
		admin = true
	}

	for input.Scan() {
		msg := input.Text()
		data := strings.SplitN(msg, " ", 2)
		if data[0][0] == '/' {
			switch data[0] {
			case "/users":
				all_users <- &who
			case "/msg":
				if len(data) != 2 {
					sendServerMessage(who.name, "the format to send a private message is /msg [user] [message]")
					break
				}
				data2 := strings.SplitN(data[1], " ", 2)
				if len(data2) != 2 {
					sendServerMessage(who.name, "the format to send a private message is /msg [user] [message]")
					break
				}
				privateMsg <- message{from: who.name, to: data2[0], msg: data2[1]}
			case "/time":
				t := time.Now()
				who.channel <- "irc-server > Local Time: " + t.Location().String() + t.Format(" 15:04")
			case "/user":
				if len(data) != 2 {
					sendServerMessage(who.name, "the format to send a private message is /user [user]")
					break
				}
				info_user <- message{from: who.name, to: data[1]}
			case "/kick":
				if len(data) != 2 {
					sendServerMessage(who.name, "the format to send a private message is /kick [user]")
					break
				}
				if !who.admin {
					who.channel <- "irc-server > Only admin can kick someone."
					break
				}
				kicking <- message{from: who.name, to: data[1]}
			default:
				sendServerMessage(who.name, "Not recongnized command")
			}
		} else {
			allMsg <- message{from: who.name, msg: input.Text()}
		}
	}

	if !who.kicked {
		leaving <- &who
		broadMsg <- "[" + who.name + "] left"
		fmt.Printf("irc-server > [%s] left\n", who.name)
		conn.Close()
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func removeUser(user *User) {
	delete(clients, (*user).name)
	close((*user).channel)
	(*user).conn.Close()
	if user.admin && len(clients) > 0 {
		for cli := range clients {
			(*clients[cli]).admin = true
			(*clients[cli]).channel <- "You're the new IRC Server ADMIN"
			fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", (*clients[cli]).name)
			break
		}
	} else if (*user).admin {
		admin = false
	}
}

func sendPrivateMessage(msg message) {
	if clients[msg.to] == nil {
		(*clients[msg.from]).channel <- "irc-server > User [" + msg.to + "] doesn't exist in the channel."
		return
	}
	(*clients[msg.to]).channel <- "" + msg.from + " [private] > " + msg.msg
}

func sendToAllMessage(from string, msg string, notInclude string) {
	for cli := range clients {
		if cli != notInclude {
			(*clients[cli]).channel <- from + " > " + msg
		}
	}
}

func sendServerMessage(to string, msg string) {
	(*clients[to]).channel <- "irc-server > " + msg
}

func main() {
	var host = flag.String("host", "localhost", "host address")
	var port = flag.Int("port", 8000, "port")
	flag.Parse()
	listener, err := net.Listen("tcp", *host+":"+strconv.Itoa(*port))
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("irc-server > Simple IRC Server started at %s:%d\nirc-server > Ready for receiving new clients\n", *host, *port)
	go messageManager()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}
