#! /bin/bash

#[ $# -eq -3 ] && [ -d $3 ] && [ -f "$1" ] || exit 1

#[ ! -f "$2" ] || exit 2

while read file; do
	lines="$(cat "$file" | egrep -n -v '^{[[:print:]]+};$|^#|^$')"
	
	if [ -z "$lines" ];then
		cat "$file" >> "$2"
		continue;
	fi

	echo "Error in $file:"
	echo "$lines" | awk '{print "Line " $0}'

done < <(find "$3" | egrep "\.cfg$")

user=$(echo "$2" | sed -r 's/\.cfg$//')

[ -z "$(cat "$1" | egrep "^${user}:")" ] || exit 0

#create user

pass=$(pwgen 16 1)
echo "${user}:${pass}" >> "$1"
echo "$user $pass"

