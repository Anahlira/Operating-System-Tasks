#! /bin/bash

[ $# -eq 2 ] && [ -d "$1" ] || exit 2 

files="$(find "$1" -maxdepth 1 -mindepth 1 | awk -F "/" '{print $NF}' | egrep "^[[:alnum:]]+-[0-9]+\.[0-9]+\.[0-9]+-${2}$")"

save="$(echo "$files" | head -1)"

while read file; do
	fileV="$(echo "$file" | cut -d "-" -f 2)"
	savedV="$(echo "$save" | cut -d "-" -f 2)"
	if (( $(echo "$(echo "$savedV" | cut -d "." -f 1) < $(echo "$fileV" | cut -d "." -f 1)" | bc -l)  )); then
		save="$file"
		continue;
	elif (( $(echo "$(echo "$savedV" | cut -d "." -f 1) == $(echo "$fileV" | cut -d "." -f 1)" | bc -l)  )); then
	   if (( $(echo "$(echo "$savedV" | cut -d "." -f 2) < $(echo "$fileV" | cut -d "." -f 2)" | bc -l)  )); then
			save="$file"
			continue;			
		elif (( $(echo "$(echo "$savedV" | cut -d "." -f 2) == $(echo "$fileV" | cut -d "." -f 2)" | bc -l)  )); then
			if (( $(echo "$(echo "$savedV" | cut -d "." -f 3) < $(echo "$fileV" | cut -d "." -f 3)" | bc -l)  )); then
			save="$file"
			continue;
			fi
		fi
	fi

done<<<${files}
echo "$save"
