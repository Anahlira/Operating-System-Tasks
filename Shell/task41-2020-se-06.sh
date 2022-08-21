#! /bin/bash

[ $# -eq 3 ] && [ -f "$1" ] || exit 1 

[ -z "$(echo "$2" | egrep -v "^[[:alpha:]0-9_]+$")" ] && [ -z "$(echo "$2" | egrep -v "^[[:alpha:]0-9_]+$")" ] || exit 2

#if confing file is needed to be cheked
[ -z "$(cat "$1" | cut -d "#" -f 1 | egrep -v "^[[:blank:]]*[[:alpha:]0-9_]+[[:blank:]]*=[[:blank:]]*[[:alpha:]0-9_]+[[:blank:]]*$")" ] || exit 3

line="$(cat "$1" | egrep "^[[:blank:]]*${2}[[:blank:]]*=")"

if [ -z "$line" ];then
	echo "$2 = $3 # added at $(date) by $(whoami)" >> "fixed_${1}"
	exit 0;
fi

value="$(echo "$line" | cut -d "=" -f 2 | cut -d "#" -f 1 | egrep -o "\<${3}\>")"

[ -z "$value" ] || exit 0

cat "$1" | awk -v key="$2" -v value="$3" -v date="$(date)" -v user="$(whoami)" '$1==key {printf("# %s # edited at %s buy %s\n",$0,date,user); printf("%s = %s # added at %s by %s\n",key,value,date,user)} $1!=key {print $0}' > "fixed_${1}"
#in real task is without the fixed_, but here is for testing
