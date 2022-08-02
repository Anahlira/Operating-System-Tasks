#! /bin/bash

[ $# -eq 1 -o $# -eq 2  ] && [ -d "$1"  ] || exit 1

syml="$(find "$1" -type l -exec test -e {} \; -printf "%p\n" 2>/dev/null)"
broken="$(find "$1" -type l -exec test ! -e {} \; -printf "%p \n" 2>/dev/null| wc -l)"

while read line;do
	follow="$(readlink -f $line)"
	pr="$(echo "$line -> $follow")"
	if [ $# -eq 2 -a -f "$2" ];then
		echo "$pr" >> "$2"
	else
		echo "$pr"
	fi	
done <<<${syml}

if [ $# -eq 2 -a -f "$2" ];then
	echo "Broken symlinks: $broken" >> "$2"
else
	echo "Broken symlinks: $broken"
fi
