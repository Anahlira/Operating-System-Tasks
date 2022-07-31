#! /bin/bash

[ $# -eq 1 -o $# -eq 2 ] && [ -d "$1" ] && [ $# -eq 1 -o "$(echo "$2" | egrep "^[0-9]+$")" ] || exit 1

#I use for in the awk, becouse the name can have " "
if [ $# -eq 2  ]; then
	find "$1" -printf '%n %p \n' 2>/dev/null | awk -v num=$2 '$1>=num {for(i=2;i<=NF;i++) print $i}'
else
	find "$1" -type l -printf '%Y %p \n' 2>/dev/null | awk '{if($1=="N" || $1=="L" || $1=="?") {for(i=2;i<=NF;i++) print $i}}'
fi
