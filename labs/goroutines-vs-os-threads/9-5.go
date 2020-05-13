package main

import (
	"flag"
	"fmt"
	"os"
	"time"
)

func messager(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}

func main() {
	t := flag.Int("time", 5, "total time running messagers in seconds")
	flag.Parse()
	a := make(chan int)
	b := make(chan int)
	var total = 0

	file, err := os.Create("9-5.txt")
	checkError(err)
	defer file.Close()

	go messager(a, b)
	go messager(b, a)
	for i := 0; i < *t; i++ {
		b <- 0
		time.Sleep(time.Duration(*t) * time.Second)
		round := <-a
		_, e1 := file.WriteString(fmt.Sprintf("Second %d: %d messages sent\n", i, round))
		checkError(e1)
		total += round
	}

	_, e1 := file.WriteString(fmt.Sprintf("Total messages sent: %d\n", total))
	checkError(e1)
	_, e2 := file.WriteString(fmt.Sprintf("Avg meessage/second: %d\n", total/(*t)))
	checkError(e2)
}
