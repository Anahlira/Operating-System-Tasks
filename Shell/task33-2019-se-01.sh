#! /bin/bash

absolute=0
hasabsolute=1
numbers="$(
while read line;do
	num="$(echo "$line" | egrep "^-?[[:digit:]]+$")"
	if [ -n "$num" ]; then
		echo "$num"

	fi
done )"

#------------a
while read line; do
	num="$(echo "$line" | egrep "^-?[[:digit:]]+$")"
	if [ -n "$num" ]; then
		#echo "$num - $(echo "$(echo "$num" | egrep -o '[0-9]+$') > $absolute" | bc -l)  - $absolute"

		if (( $(echo "$(echo "$num" | egrep -o '[0-9]+$') > $absolute" | bc -l)  ));then
			absolute="$(echo "$num" | egrep -o '[0-9]+$')"
			hasabsolute=0
		fi
	fi

done <<< $numbers

if (( $hasabsolute )); then
	echo "no numbers!"
	exit 0
fi

echo "$numbers" | egrep "^-?${absolute}$" | sort -n | uniq
