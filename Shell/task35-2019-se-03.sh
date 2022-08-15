#! /bin/bash

[ $# -eq 1 ] && [ -d "$2" ] || exit 1

time=$(stat -c "%X" "$0")

while read file; do
	if (( $(echo "$time > $(stat -c "%Y" "$file")" | bc -l) ));then
		continue;
	fi
	
	meow="$(tar -tf "$file" | egrep "[/]?meow\.txt$")"
	[ -n "$meow" ] || continue;
	
	tar -xf "$file" "$meow"
	name="$(echo "$file" | cut -d "_" -f 1)"
	timest="$(echo "$file" | cut -d "_" -f 2 | cut -b 8- | sed -r 's/\.tgz$//')"
	mv "$meow" "/extracted/${name}_${timest}.txt"

done< < (find "$2" -mindepth 1 -maxdepth 1 | egrep '^[^_]+_report-[1-9][0-9]*\.tgz')


#using it so it would change when executed
touch "$0"
