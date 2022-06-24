#! /bin/bash

[ -f philip-j-fry.txt ] || exit 1

echo "Броят на търсените редове е $(cat philip-j-fry.txt | egrep "[02468]+" | egrep -v "[a-w]" | wc -l)"
