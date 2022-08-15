#! /bin/bash

[ $# -eq 2 ] && [ -d "$1" ] && [ -d "$2" ] || exit 1

#$1-хранилище $2-пакет
#if structure is not the same
[ -f "$1/db" ] && [ -f "$2/version" ] && [ -d "$1/packages" ] && [ -d "$2/tree" ] || exit 2

name=$(echo "$2")
version=$(cat "$2/version" | egrep '^[0-9]+\.[0-9]+\.[0-9]+-[0-9]+$')

[ -n "$(echo "$version")" ] && [ $(echo "$version" | wc -l) -eq 1 ] || exit 3

tar -cf "$2/tree.tar" "$2/tree"
xz "$2/tree.tar"

cs=sha256 "$2/tree.tar.xz"

if [ -n "$(cat "$1/db" | egrep "^${name}-${version}")" ]; then
	oldcs=$(cat "$1/db" | egrep "${name}-${version}" | cut -d " " -f 2)
	rm "$1/packages/${oldcs}.tar.xz"
	sed -r -i "s/${oldcs}/${cs}/"
else
	echo "${name}-${version} $cs"
fi

mv "$2/tree.tar.xz" "$1/packages/${cs}.tar.xz"
echo "$(cat "$1/db" | sort)" > "$1/db"

