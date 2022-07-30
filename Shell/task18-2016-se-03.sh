#! /bin/bash

#check if only files
[ -z "$(find . -mindepth 1 -printf "%y\n" | egrep -v  "^f$")" ] || exit 1
#easier - [ -z $(find . -mindepth 1 ! -type f)  ] || exit 1

#check two parameters
[ $# -eq 2 ] && [ -n "$(echo "${1}" | egrep '^[0-9]+$')" ] && [ -n "$(echo "${2}" | egrep '^[0-9]+$')" ] || exit 2

#kick if directories already exists
[ ! -d "a" ] && [ ! -d "b" ] && [ ! -d "c" ] || exit 3

mkdir a b c

#using while because with "for" you can't have files with spaces
while read file; do
	[ ! -d "${file}" ] || continue;

	numLines="$(cat "${file}" | wc -l)"
	if [ $1 -gt ${numLines} ]; then
		mv "${file}" a/
		continue;
	fi

	if [ $1 -gt $2 ]; then
		num1="$2"
		num2="$1"
	else
	   	num1="$1"
		num2="$2"
	fi

	if [ ${numLines} -gt ${num1} -a ${num2} -gt ${numLines} ]; then
		mv "${file}" b/
	else mv "${file}" c/
	fi
done < <(find . -mindepth 1 -maxdepth 1)
