#! /bin/bash

while read line;do
	echo "$line" | awk '{print $1 " " $2}'
	if [ -n "$(echo "$line" | awk '$2 < 2*$4 {print "kill}"')"  ];then
		pid="$(echo "$line" | awk '{print $3}')"
		echo "kill $pid"
		#kill -15 $pid
		#sleep 2
		#kill -9 $pid
	fi
done< <(ps h -eo user,rss,pid | awk 'NF==1 {user=$1;rss=0;pid=$3;max=$2;num=0} user!=$1 {if(num==0) {num=1}; print user " " rss/num " " pid " " max; user=$1; rss=0;pid=$3;max=$2;num=0} user==$1 {rss=rss+$2; if(max < $2) {pid=$3;max=$2};num=num+1}')
