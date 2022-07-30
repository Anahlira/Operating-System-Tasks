#! /bin/bash

#[ $(whoami) == "root" ] || exit 1

#от /etc/passwd 6тата колонка е евентуално директорията
#1.дали е директория 2.може ли потребителя да пише в нея 
#2.1 собственик ли е потребителя => има ли write права
#2.1 ако не е - дали някоя от групите на потребилеля е като групата на директорията
#2.1 ако е, групата трябва да има write права
#2.1 и накрая дали others имат wright права
#и да се изкара кои са тези

while read line
do
	directory="$(echo "$line" | cut -d ":" -f 6)"
	user="$(echo "$line" | cut -d ":" -f 1)"

	if [ ! -d $directory ]; then
		echo "${line}"
		continue;
	fi	

	perm="$(stat ${directory} --printf "%A")"
	
	if [ $(stat -c '%U' ${directory}) == "${user}" -a "$(echo ${perm} | cut -c 3)" != 'w' ] ; then
		echo "${line}"
		continue;
	fi

	if [ $(stat -c '%U' ${directory}) != "${user}" ]; then
		ugroups="$(id -nG ${user})"
		filegroup="$(stat -c '%G' ${directory})"
		#echo "${ugroups} ->  ${filegroup}"
		if [ -n "$(echo ${ugroups} | egrep "${filegroup}")" -a "$(echo ${perm} | cut -c 6)" != 'w' ]; then
			echo ${line}
			continue;
		fi
		if [ -z "$(echo ${ugroups} | egrep "${filegroup}")"  -a "$(echo ${perm} | cut -c 9)" != 'w' ]; then
			echo ${line}
			continue;
		fi
	fi

done < <(cat /etc/passwd)
