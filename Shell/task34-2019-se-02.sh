#! /bin/bash

if [ "$1" == "-n" ];then
	[ -n "$(echo "$2" | egrep "^[[:digit:]]+$")"  ] || exit 1
	N=$2
	shift 2
else 
	N=10
fi

var=$(
while [ ! $# -eq 0 ]; do
	while read line; do
		time="$(echo "$line" | cut -d " " -f 1,2)"
		IDF="$(echo "$1" | sed -r 's/\.txt$//')"
		data="$(echo "$line" | cut -d " " -f 3- | cut -d ":" -f 1)"
		
		echo "$time $IDF $data"
		
	done < <( cat "$1" | tail -$N)
	shift 1
done)

echo "$var" | sort -n

#date idf data
#date idf data
#date idf data
