#! /bin/bash

[ $# -eq 2 ] && [ -f "$1" ] && [ -d "$2" ] || exit 1
[ -n "$(find "$2" -mindepth 1)" ] || exit 2

#name fam(ds sds a): th dha ka

touch "${2}/dist.txt"
file="${2}/dist.txt"
ID=1
while read line;do
	name="$(echo "$line" | egrep -o "^[[:alpha:]-]+ ^[[:alpha:]-]+")"
	if [ -z "$(echo "$file" | egrep "^$name;")" ];then
		echo "${name};${ID}\n" >> "$file"
		ID=$ID+1
	fi

	echo "$line" >> "${2}/$(echo "$file" | egrep "^$name;" | cut -d ";" -f2).txt"

done < <(cat "$1")
