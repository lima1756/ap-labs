Lab - Goroutines vs OS Threads
==============================
Read section 9.8 from The [Go Programming Language](https://www.amazon.com/dp/0134190440/ref=cm_sw_em_r_mt_dp_U_Uz0RDbHAH27PH) book.
Then, do the 9.4 and 9.5 exercises.

- Update the `README.md` file on instructions about how to run your programs.
- Your 2 programs must automatically generate their proper  result report depending on what parameters they received.
- Generated reports are free format and written on any file extension (i.e. .txt, .pdf, .md, .svg, .png, .jpg, .tex)

General Requirements and Considerations
---------------------------------------
- Submit only `*.go` files.
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.

Run
---

### Exercise 9.4

```
go run 9-4.go
```
Exercise 9.4 also has an optional flag parameter: --pipes that defines the number of pipes for the pipeline. Ex:
```
go run 9-4.go --pipes 500000
```
The output is **9-4.txt**
### Exercise 9.5

```
go run 9-5.go
```
Exercise 9.5 also has an optional flag parameter: --time that defines the number of seconds is going to run the communication between threads. Ex:
```
go run 9-5.go --time 10
```
The output is **9-5.txt**


How to submit your work
=======================
```
GITHUB_USER=<your_github_user>  make submit
```
More details at: [Classify API](../../classify.md)
