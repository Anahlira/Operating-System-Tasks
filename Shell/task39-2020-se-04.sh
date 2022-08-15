#! /bin/bash

[ $# -eq 2 ] && [ -d "$1" ] && [ ! -d "$2" ] || exit 2

mkdir "$2" "${2}/images"
#другите папки пак ли на ръка трябва да ги правя??
#има ли нещо умно което пропускам

while read file; do
	cleanf=$(echo "file" | sed -r 's/\.jpg$//g' | sed -r 's/ +/ /g' | sed -r's/^ +//g' | sed -r 's/ +$//g' ) #remove spaces
	title=$(echo "$cleanf" | sed -r 's/\([[:print:]]\)//g' | sed -r 's/ +/ /g')
	album=$(echo "$cleanf" | awk -F "(" '{prit $NF}' | cut -d ")" -f 1)
	date=$(stat "$file" --printf="%y" | cut -d " " -f 1)
	hex=$(sha256sum $file | cut -b 1-16)

	cp "$file" "${2}/images/${hex}.jpg"
	
	mkdir "$2/by-date" "$2/by-album" "$2/by-title" #"........"

	#make symlink
	touch "$2/by-title/${title}.jpg"
	ln -s "$2/by-title/${title}.jpg" "../../images/${hex}.jpg"

	#....
done < <(find "$1" -mindepth 1 | egrep '\.jpg$')
