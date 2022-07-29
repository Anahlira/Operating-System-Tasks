#! /bin/bash

[ $# -eq 1 ] && [ -n "$(echo "$1" | egrep "^[0-9]+$")"  ] || exit 1 # && [ "$(whoami)" = "root"  ] || exit 1

show=$(ps h -eo uid,rss,pid | sort -k 1 | awk 'NR==1 {num=$1; sum=$2; max=$2;pid=$3} num==$1 {sum=sum+$3; if(max<$2) {max=$2;pid=$3}} num!=$1 {printf("%s %d %d \n",num,sum,pid); sum=$3; num=$1;max=$2;pid=$3}')

echo "${show}"
echo "---------------------------------------"

while read line;
do
	if [ -n "$(echo "$line" | awk -v number="$1" '$2>=number {print "kill"} $2<number {}')" ];
	then
		echo "kill $(echo "$line" | awk '{print "pid=" $3}')"
		#uncomment below
		#kill -15 "$(echo "$line" | awk '{print $3}')"
		#sleep 2
		#kill -9 "$(echo "$line" | awk '{print $3}')"
	else echo "$(echo "$line" | awk '{print $1 " " $2}')"
	fi
done <<<${show}

