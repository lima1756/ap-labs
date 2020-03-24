#!/bin/bash

for i in {1..3};
do
    size=`stat -c %s ./test.txt `
    time_start1=`date +%s`
    cat test.txt >> echo
    time_end1=`date +%s`

    size=`stat -c %s ./test.txt `
    time_start2=`date +%s`
    ./cat2 test.txt >> echo
    time_end2=`date +%s`

    echo $size', '$((time_end1-time_start1))', '$((time_end2-time_start2)) >> results.csv
    cat ./test.txt | tee -a test.txt > /dev/null
done