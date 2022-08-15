#! /bin/bash

[ $# -eq 1 ] && [ -f "$1" ] || exit 1

sites=$(cat "$1" | cut -d " " -f 2 | sort | uniq -c | sort -n -t " " | tail -3 | awk '{print $2}')

for site in $sites;do
	siteInfo=$(cat "$1" | egrep "\<$site\>")
	H2=$(echo "$siteInfo" | egrep "\<HTTP/2.0\>" | wc -l)
	HT=$(echo "$siteInfo" | egrep "\<HTTP/1.0\>|\<HTTP/1.1\>" | wc -l)

	users=$(while read user;do
		if (( $(echo "$(echo "$user" | awk '{print $2}') > 302" | bc -l)  ));then
			echo "$user" | awk '{print $1}'
		fi
	done < <(echo "$siteInfo" | cut -d " " -f 1,9) | sort | uniq -c | sort -nr -t " " | head -5 )

	echo "$site HTTP/2.0: $H2 non-HTTP/2.0: $HT"
	[ -z "$users" ] || echo "$users"
done
