package main

import (
	"fmt"
	"os"
	"path/filepath"
	"text/tabwriter"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	dirs := 0
	links := 0
	sockets := 0
	devices := 0
	other := 0
	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		switch mode := info.Mode(); {
		case mode.IsDir():
			dirs++
		case mode&os.ModeSymlink != 0:
			links++
		case mode&os.ModeSocket != 0:
			sockets++
		case mode&os.ModeDevice != 0:
			devices++
		default:
			other++
		}
		return nil
	})
	if err != nil {
		return err
	}
	w := tabwriter.NewWriter(os.Stdout, 8, 8, 0, '\t', 0)
	fmt.Fprintf(w, "+\t+\t+\n")
	fmt.Fprintf(w, "|Path\t| %s\t|\n", dir)
	fmt.Fprintf(w, "+\t+\t+\n")
	fmt.Fprintf(w, "| Directories\t| %d\t|\n", dirs)
	fmt.Fprintf(w, "| Symbolic Links\t| %d\t|\n", links)
	fmt.Fprintf(w, "| Devices\t| %d\t|\n", devices)
	fmt.Fprintf(w, "| Sockets\t| %d\t|\n", sockets)
	fmt.Fprintf(w, "| Other files\t| %d\t|\n", other)
	fmt.Fprintf(w, "+\t+\t+\n")
	w.Flush()
	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
	d := scanDir(os.Args[1])

	if d != nil {
		fmt.Println(d.Error())
	}
}
