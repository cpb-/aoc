#! /bin/bash

if [ $# -ne 1 ]; then echo "usage: $0 <file>" >&2; exit 1; fi


cat "$1" | \
sed -e 's/^.*|//' | \
awk '
{
	for(f=1; f <= NF; f++) {
		if ((length($f) == 2) || (length($f) == 3) || (length($f) == 4) || (length($f) == 7)) {
			count++
		}
	}
}

END {
	print count
}'





