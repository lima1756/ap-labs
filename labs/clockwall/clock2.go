// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"io"
	"log"
	"net"
	"strconv"
	"time"
)

func handleConn(c net.Conn) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	var port = flag.Int("port", 8080, "define the connection port")
	flag.Parse()
	listener, err := net.Listen("tcp", "localhost:"+strconv.FormatInt(int64(*port), 10))
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}


/*
TZ=US/Eastern    go run clock2.go -port 8010 &
TZ=Asia/Tokyo    go run clock2.go -port 8020 &
TZ=Europe/London go run clock2.go -port 8030 &
*/