#! /bin/bash

[ $# -eq 2 ] || exit 1

homedir="$(cat /etc/passwd | egrep "^$(whoami)" | cut -d ":" -f 6)"

[ -d "${homedir}" ] || exit 2
#echo "$(find ${homedir})"

[ -f ~/"$1" ] && [ -f "${homedir}"/"$2" ] || exit 3

if [ $(cat "${homedir}"/"$1" | wc -l) -gt $(cat "${homedir}"/"$2" | wc -l) ]; then
	file="$1"
else 
	file="$2"
fi

cat "${homedir}"/"${file}" | cut -d "-" -f 2,3 | sed -r 's/^ //' | sort > "${homedir}"/"${file}.songs"
