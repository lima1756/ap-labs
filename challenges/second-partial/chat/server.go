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
}

type message struct {
	from string
	to   string
	msg  string
}

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	admin      = false
	entering   = make(chan *User)
	leaving    = make(chan *User)
	kicked     = make(chan *User)
	b_messages = make(chan string)  // broadcast messages
	c_messages = make(chan message) // cast message (uni or multi)
	all_users  = make(chan *User)
	info_user  = make(chan message)
)

func messageManager() {
	clients := make(map[string]*User) // all connected clients
	var users []*User
	for {
		select {
		case msg := <-b_messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				(*clients[cli]).channel <- "irc-server > " + msg
			}
		case msg := <-c_messages:
			if msg.to == "" {
				for cli := range clients {
					if cli != msg.from {
						(*clients[cli]).channel <- msg.from + " > " + msg.msg
					}
				}
				break
			}
			elem, ok := clients[msg.to]
			if !ok {
				(*clients[msg.from]).channel <- "irc-server > User [" + msg.to + "] doesn't exist in the channel."
				break
			}
			(*elem).channel <- "" + msg.from + " > " + msg.msg
		case user := <-all_users:
			var s string
			for cli := range clients {
				s += cli + ", "
			}
			(*user).channel <- "irc-server > " + s
		case msg := <-info_user:
			elem, ok := clients[msg.to]
			if !ok {
				(*clients[msg.from]).channel <- "irc-server > User [" + msg.to + "] doesn't exist in the channel."
				break
			}
			(*clients[msg.from]).channel <- "irc-server > " + "username: " + msg.to + ", IP: " + (*elem).conn.RemoteAddr().String()
		case cli := <-entering:
			fmt.Printf("irc-server > New connected user [%s]\n", cli.name)
			users = append(users, cli)
			clients[(*cli).name] = cli

		case cli := <-leaving:
			fmt.Printf("irc-server > [%s] left\n", cli.name)
			var i int
			for i = 0; users[i] != cli; i++ {
			}
			users = append(users[:i], users[i+1:]...)

			if cli.admin && len(users) > 0 {
				(*users[0]).admin = true
				(*users[0]).channel <- "You're the new IRC Server ADMIN"
				fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", (*users[0]).name)
			} else if cli.admin {
				admin = false
			}
			delete(clients, (*cli).name)
			close(cli.channel)
		}
	}
}

//!-broadcaster

//!+handleConn
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
	}
	who.channel <- "irc-server > Welcome to the Simple IRC Server"
	who.channel <- "irc-server > Your user [" + who.name + "] is successfully logged"
	c_messages <- message{from: "irc-server", msg: "New connected user [" + who.name + "]"}
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
				// TODO: Verify it exists data 1
				data2 := strings.SplitN(data[1], " ", 2)
				// TODO: Verify it exists data2 1
				c_messages <- message{from: who.name + " [Private]", to: data2[0], msg: data2[1]}
			case "/time":
				t := time.Now()
				who.channel <- "irc-server > Local Time: " + t.Location().String() + t.Format(" 15:04")
			case "/user":
				// TODO: Verify there is data1
				info_user <- message{from: who.name, to: data[1]}
			case "/kick":
				if !who.admin {
					who.channel <- "irc-server > Only admin can kick someone."
					break
				}
				// TODO: falta hacer el kick
			}
		} else {
			c_messages <- message{from: who.name, msg: input.Text()}
		}
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- &who
	b_messages <- "[" + who.name + "] left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
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

//!-main
