#! /bin/bash

[ $# -eq 3 ] && [ -f "$1" ] || exit 1

lines="$(cat "$1" | egrep -e "^${3}=|^${2}=")"

#check if keys are in file
[ $(echo "${lines}" | wc -l) -eq 2 ] || exit 2


for value in $(echo "${lines}" | egrep "${2}=" | cut -d "=" -f 2); do
	if [ -n "$(echo "${lines}" | egrep "${3}=" | cut -d "=" -f 2 | egrep "${value}")" ]
   	then
		#to work in the task it should be added -i 
		sed -r "s/(${3}=[[:print:]]*)${value}([[:print:]]*)/\1\2/g" "$1" | sed -r "s/[[:space:]]+/ /g" | sed -r "s/=[[:space:]]+/=/"
		echo " "
	fi
done


