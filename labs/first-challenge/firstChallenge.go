package main

import (
	"fmt"
	"os"
)

func main() {
	longest := 0
	curr := 0
	var ascii [256]int
	for i:=0;i<256;i++ {
		ascii[i] = -1;
	}

	for i, char := range os.Args[1] {
		last := ascii[char]

		if last == -1 || last < i - curr{
			curr++
		} else {
			if curr>longest {
				longest = curr
			}
			curr = i - last
		}
		ascii[char] = i
	}
	if curr > longest {
		longest = curr
	}
	fmt.Println(longest)
}