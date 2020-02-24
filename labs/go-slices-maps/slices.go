package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	data := make([][]uint8, dy)
	for i := 0; i < dy; i++ {
		data[i] = make([]uint8, dx)
	}
	for y := 0; y < dy; y++ {
		for x := 0; x < dx; x++ {
			data[y][x] = uint8(((x + y) / 2) % 255)
		}
	}
	return data
}

func main() {
	pic.Show(Pic)
}
