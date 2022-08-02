#! /bin/bash

#[ $(whoami) == "root"  ] || exit 1

#I have't cheked if it works

info="$(ps h -eo user,rss,pid | sort -t " " -k 1,1 | awk 'NR==1 {user=$1;rss=0;} user!=$1 {printf("%s %d\n",user,rss); user=$1;rss=0} user==$1 {rss=rss+$2;}')"

rootrss="$(echo "$info" | egrep "^root\>" | awk 'BEGIN {sum=0} {sum=sum+$2} END {print sum}')"

while read line;do
	user="$(echo "$line" | cut -d " " -f 1)"

	[ "$user" != "root" ] || continue

	userrss="$(echo "$line" | cut -d " " -f 2)"
	directory="$(cat /etc/passwd | egrep "^$user:" | cut -d ":" -f 6)"
	[ ! -d "$directory" ] || [ "$(stat "$directory" --printf="%U")" != "$user" ] || [ "$(stat "$directory" --printf="%A" | cut -b 3)" != 'w'  ] || continue
	if (( $(echo "$userrss > $rootrss" | bc -l) )); then
		echo "$(ps h -eo user,pid | egrep "^$user\>" | awk '{print "kill " $2}')"
	fi

done <<< ${info}
