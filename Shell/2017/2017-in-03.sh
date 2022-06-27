#! /bin/bash

cat 2017-in-03-file | egrep ":* *a[,:]" | egrep "/home/Inf" | cut -b 3,4 | sort -n | uniq -c | sort -n -r -k 1,1 | awk 'BEGIN {num = 0} num == 0 {num = $1} num == $1'
