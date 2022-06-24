#! /bin/bash

#A#

#create empty files to be deleted
#touch a b c
#[ -d bar ] || mkdir bar
#touch bar/a bar/b bar/c

#rm -r "$(find . -printf '%s %p\n' | awk '$1 == 0' | cut -d " " -f 2-)"

#B#
#rm -r "$(find ~ -mindepth 1 -printf '%s %p\n' | sort -n -k 1,1 | tail -5 | cut -d " " -f 2-)"

#important things
# %s is first because %p can have spaces
