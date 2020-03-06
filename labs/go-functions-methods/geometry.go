// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// Angle ... Obtain the angle from the point
func (p Point) Angle() float64 {
	angle := math.Asin(p.Y() / math.Sqrt(math.Pow(p.Y(), 2)+math.Pow(p.X(), 2)))
	if p.X() < 0 && p.Y() > 0 {
		angle += (math.Pi / 2)
	} else if p.X() < 0 && p.Y() < 0 {
		angle += (math.Pi * 3 / 2)
	} else if p.Y() < 0 {
		angle += (2 * math.Pi)
	}

	return angle
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func getPosition(path Path, p Point) int {
	len := len(path)
	if len == 0 {
		return 0
	}
	start := 0
	end := len - 1
	for end-start > 1 {
		middle := (start + end) / 2
		if path[middle].Angle() > p.Angle() {
			end = middle
		} else {
			start = middle
		}
	}
	if path[start].Angle() > p.Angle() {
		return start
	} else if path[end].Angle() > p.Angle() {
		return end
	}
	return end + 1
}

//!-path

func main() {
	rand.Seed(time.Now().UnixNano())

	if len(os.Args) != 2 {
		println("Please introduce as an argument the number of points of the figure")
		return
	}
	input := os.Args[1]
	total, err := strconv.Atoi(input)
	if err != nil {
		println("Please introduce the number of points of the figure")
		return
	}
	fmt.Printf("- Generating a [%d] sides figure\n", total)
	var path Path
	for i := 0; i < total; i++ {
		point := Point{
			x: (rand.Float64() * 200) - 100,
			y: (rand.Float64() * 200) - 100,
		}
		position := getPosition(path, point)
		path = append(path, point)
		copy(path[position+1:], path[position:])
		path[position] = point
	}
	fmt.Printf("- Figure's vertices\n")
	for i := range path {
		fmt.Printf("\t - (%f), (%f)\n", path[i].X(), path[i].Y())
	}
	fmt.Printf("- Figure's perimeter\n")

	fmt.Printf("\t-")
	for i := 0; i < len(path)-1; i++ {
		fmt.Printf(" %f +", path[i].Distance(path[i+1]))
	}
	fmt.Printf(" %f =  %f\n", path[len(path)-1].Distance(path[0]), path.Distance())
}
