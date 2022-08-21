#! /bin/bash

while [ ! $# -eq 0];do
	if [ ! -f "$1" ];then
		echo "$1 is not a file" > &2
		shift 1
		continue;
	fi
	
	FQDN="^([a-z0-9]\.{1})+$"
	mid="([0-9]+ )*IN"
	typ="NS|A|AAAA"
	firstline="$(cat "$1" | head -1)"
	N=2

	if [ -z "$(echo "$firstline" | egrep '(' )"];then
		if [ -z "$(echo "$firstline" | egrep "$FQDN $mid SOA $FQDN $FQDN ($")" ];then
			echo "$1 is not right format" > &2
			continue;
		fi
		N=8
	else
		if [ -z "$(echo "$firstline" | egrep "$FQDN $mid SOA $FQDN $FQDN [0-9]{10} ([0-9]+ ){3}[0-9]+")" ];then
			echo "$1 is not right format" > &2
			continue;
		fi
		serial="$(echo "$firstline" | egrep -o "[0-9]{10} ([0-9]+ ){3}[0-9]+" | cut -d ' ' -f 1)"
	fi

	if [ $N -eq 8 ];then
		if [ -z "$(cat $1 | head -7 | tail -1 | egrep "^[[:blank:]]*)[[:blank:]]*")" ];then
			echo "$1 is not right format" > &2
			continue;
		fi
		if [ -n "$(cat "$1" | head -6 | tail -5 | egrep "^[[:blank:]][0-9]+[[:blank:]]+;")" ];then
			echo "$1 is not right format" > &2
			continue;
		fi
		serial="$(cat "$1" | head -2 | tail -1 | egrep -o "[0-9]{10}[[:blank:]]+; | cut -d " " -f 1")"
	fi

	if [ -n "$(cat "$1" | tail -$N | egrep -v "$FQDN $mid $typ ")" ];then
			echo "$1 is not right format" > &2
			continue;
	fi

	_date="$(date +'%Y%m%d')"

	if [ $_date == "echo $serial | cut -b 1-8" ];then
		num="$(echo $serial | cut -b 8-10)"
		if [ $num -eq 99 ];then
			echo "Can't change $1 any more"
			continue;
		fi
		num1="$(echo $num | cut -b 1)"
		num2="$(echo $num | cit -b 2)"
		if [ $num2 -eq 9];then
			num2=0
			num1=$(($num1 + 1))
		else
			num2=$(($num2 + 1))
		fi
		D="${_date}${num1}${num2}"
	else 
		D="${_date}00"
	fi

	sed -i -r "s/${serial}/${D}/" "$1"

	shift 1
done
