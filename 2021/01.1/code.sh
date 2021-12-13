#! /bin/sh


if [ ! -f "$1" ]
then
	echo "Please give me the input file as argument" >&2
	exit 1
fi

awk 'BEGIN { prev=-1 }; (($1 > prev) && (prev != -1)) { increases ++}; { prev = $1 }; END {print increases}' < "$1"

