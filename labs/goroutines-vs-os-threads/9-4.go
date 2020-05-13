package main

import (
	"flag"
	"fmt"
	"os"
	"runtime"
	"time"
)

var idle = make(chan struct{})

func pipeline(in chan struct{}, out chan struct{}, current int, total *int) {
	if current != *total {
		go pipeline(out, in, current+1, total)
		out <- <-in
	} else {
		idle <- struct{}{}
	}
}

func main() {
	totalPipes := flag.Int("pipes", 10000000, "total number of pipes")
	flag.Parse()

	file, err := os.Create("9-4.txt")
	checkError(err)

	defer file.Close()

	_, e1 := file.WriteString("Before the pipeline the memory usage was:\n" + memUsage())
	checkError(e1)
	in := make(chan struct{})
	out := make(chan struct{})
	go pipeline(in, out, 0, totalPipes)

	start := time.Now()
	in <- struct{}{}
	<-idle

	_, e2 := file.WriteString("After the pipeline the memory usage was:\n" + memUsage())
	checkError(e2)
	_, e3 := file.WriteString(fmt.Sprintf("The total time for data to traverse through all the pipeline of %d elements was: %s", *totalPipes, time.Since(start)))
	checkError(e3)

}

func memUsage() string {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	return fmt.Sprintf("\tAlloc = %v MiB\n\tMallocs = %v\n\tFrees = %v\n\tCurrent Mallocs = %v\n\tHeapAlloc = %v MiB\n\tSys = %v MiB\n\tInUse = %v\n",
		bToMb(m.Alloc), m.Mallocs, m.Frees, m.Mallocs-m.Frees, bToMb(m.HeapAlloc), bToMb(m.Sys), bToMb(m.HeapInuse))
}

func bToMb(b uint64) uint64 {
	return b / 1024 / 1024
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}
