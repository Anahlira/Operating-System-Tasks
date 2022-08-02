#! /bin/bash

#id,s,a,m
#id,s,a,m

[ $# -eq 2 ] && [ -f "$1" ] || exit 1
[ ! -f "$2" ] || exit 2 #if b.csv exist

while read line;do
	if [ -n $(echo "$line" | awk '$1==1 {print "yes"}')  ];then
		echo "$line" >> "$2"
	else
		cat "$2" | egrep "$line$" | sort -n -t "," -k 1,1 | head -1 >> "$2"
	fi
done < <(cat "$2" | sort -t "," -k 2 | cut -d "," -f 2- | uniq - c | awk '{print $1 " " $2}')
