// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"./links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

// Reading the depth flag
var totalDepth = flag.Int("depth", 1, "crawling depth")

type site struct {
	url   string
	depth int
}

func crawl(s site) []site {
	fmt.Println(s.url)
	if s.depth < *totalDepth {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(s.url)
		data := make([]site, 0)
		for _, url := range list {
			data = append(data, site{url: url, depth: s.depth + 1})
		}
		<-tokens // release the token

		if err != nil {
			log.Print(err)
		}
		return data
	}
	return []site{}
}

//!-sema

//!+
func main() {
	flag.Parse()
	worklist := make(chan []site)
	var n int // number of pending sends to worklist
	fmt.Printf("%d", *totalDepth)
	// Start with the command-line arguments.
	n++
	go func() {
		urls := os.Args[1:]
		data := make([]site, 0)
		for _, url := range urls {
			data = append(data, site{url: url, depth: 0})
		}
		worklist <- data
	}()
	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.url] {
				seen[link.url] = true
				n++
				go func(link site) {
					worklist <- crawl(link)
				}(link)
			}
		}
	}
}

//!-
