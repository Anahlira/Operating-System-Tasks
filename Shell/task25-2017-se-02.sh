#! /bin/bash

[ $# -eq 3 ] && [ -d "$1" ] && [ -z "$(find "$2" -mindepth 1)" ] || exit 1
#[$(whoami) == "root"] || exit 2

while read file;do
	mv "$file" "$2"
done < <(find "$1" | egrep "$3")
