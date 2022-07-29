#! /bin/bash

#this allows the name of the file to be changed quickly
file=spacex.txt

#egrep accepts | as special character idk why, because it means "or"
#after uniq, to cut a field use awk
site=$(cat ${file} | egrep '\|Failure\|' | cut -d "|" -f 2 | sort | uniq -c | sort -n | head -1 | awk '{print $2}')

#I sort by biggest number - the sooner, the more seconst went
cat ${file} | egrep "\|${site}\|" | sort -t "|" -nk 1,1 | tail -1 | sed -r 's/\|/:/g' | cut -d ":" -f 3,4
