#! /bin/bash

[ $(whoami) == "oracle" ] || [ "$(whoami)" == "grid" ] || exit 1

ADR="$($(find "$ORACLE_HOME" | egrep "/bin/adrci$") exec="show homes")"

[ -n "$(echo "$ADR" | egrep "ADR Homes:")" ] || exit 0

while read file;do

	find "/u01/app/$(whoami)" -printf '%s %p\n'| egrep "${file}$"

done < <(echo "$ADR" | sed -r '/^$/d' | tail -n +2)



