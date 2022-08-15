#! /bin/bash

[ $# -eq 2 ] && [ -d "$2" ] || exit 1

[ ! -f "$1" ] || exit 2

echo "hostname,phy,vlans,hosts,failover,VPN,peers,VLAN TP,license,SN,key" > "$1"

while read file;do
	name="$(echo "$file" | sed -r 's/[[:print:]]*\/([[:print:]]+)\.log/\1/')"	
	ufile="$(cat "$file" | egrep -v "Licensed features" | sed -r '/^$/d')"
   	st="$(echo "$ufile" | head -n -3 | awk '{printf("%s,",$NF)}')"
	lic="$(echo "$ufile" | egrep "^This" | cut -d " " -f 5- | sed -r 's/ license\.$//')"
	en="$(echo "$ufile" | tail -2 | awk '{printf("%s,",$NF)}' | sed -r 's/,$//')"
   	
	echo "${name},${st}${lic},${en}" >> "$1"
	
done < <(find "$2" | egrep "\.log$")

#rm "$1"
