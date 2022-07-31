#! /bin/bash

[ $# -eq 1 ] || exit 1 # && [ $(whoami) == "root" ] || exit 1

#check with etc/passwd for name

pr="$(ps h -eo user,pid,time | tr ':' ' ')"

info="$(echo "$pr" | sort -k 1,1 | awk 'NR==1 {user=$1;prosses=0;time=0} user!=$1 {print user " " prosses " " time; user=$1;prosses=0;time=0} user==$1 {prosses=prosses+1; time=time + $3*60*60 + $4*60 + $5}')"

#echo "$info"

echo "a------------------"
echo "$info" | awk -v user="$1" -v num="$(echo "$info" | egrep "${1}" | cut -d " " -f 2)" '$1!=user {if ($2 > num) {print $1 " " $2 ">" num}}'

echo "b------------------"
avg=$(echo "$info" | awk 'BEGIN {time=0;br=0} {br=br+$2;time=time+$3} END {print time/br}')
echo "${avg}"

echo "c-----------------"
while read line; do
	secondsM=$(echo "$line" | awk 'BEGIN {time=$3*60*60 + $4*60 + $5} {print time*2}')
	#echo "$avg $secondsM"
	if (( $(echo "$secondsM > $avg" | bc -l) )); then
		pid=$(echo "$line" | awk '{print $2}')
		echo "kill $pid"
		#kill -15 $pid
		#sleep 2
		#kill -9 $pid
	fi
done < <(echo "$pr" | egrep "^$1")
