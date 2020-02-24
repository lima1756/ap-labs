package main

import (
	"strings"

	"golang.org/x/tour/wc"
)

func WordCount(s string) map[string]int {
	data := make(map[string]int)
	for _, val := range strings.Fields(s) {
		data[val]++
	}
	return data
}

func main() {
	wc.Test(WordCount)
}
