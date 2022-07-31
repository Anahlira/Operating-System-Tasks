#! /bin/bash

max="0 file \n"
username=""
while read line;do
	user="$(echo "$line" | cut -d ":" -f 1)"
	home="$(echo "$line" | cut -d ":" -f 2)"
	files="$(find "$home" -type f -printf '%T@ %p \n' 2>/dev/null)"

	[ -n "$(echo "$files")" ] || continue;
	last="$(echo "$files" | sort -n -t ' ' -k 1,1 | tail -1)"
	if (( $(echo "$(echo "$max" | cut -d ' ' -f 1) < $(echo "$last" | cut -d ' ' -f 1)" | bc -l) )); then
		max="$last"
		username="$user"
	fi
done < <(cat /etc/passwd | cut -d ":" -f 1,6)

echo "$username $(echo "$max" | cut -d ' ' -f 2)"
