package main

import (
	"fmt"
	"net"
	"os"
	"strings"
)

type clock struct {
	place      string
	connection net.Conn
}

func (c clock) handleConn() {

	data := make([]byte, 11)
	_, err := c.connection.Read(data)
	if err != nil {
		fmt.Printf("%s", err)

	} else {
		fmt.Printf("%s: %s", c.place, data)
	}
	c.connection.Close()
}

func main() {
	if len(os.Args) == 1 {
		fmt.Printf("Error")
		return
	}
	for _, input := range os.Args[1:] {
		data := strings.Split(input, "=")
		conn, err := net.Dial("tcp", data[1])
		if len(data) != 2 || err != nil {
			fmt.Printf("Error\n")
			return
		}
		clk := clock{
			place:      data[0],
			connection: conn,
		}
		clk.handleConn()
	}

}
