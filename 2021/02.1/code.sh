#! /bin/sh


if [ ! -f "$1" ]
then
	echo "Please give me the input file as argument" >&2
	exit 1
fi


awk '
	($1 ~ /^forward$/) { horiz += $2 };
	($1 ~ /^down$/) { depth += $2 };
	($1 ~ /^up$/) { depth -= $2 };

	END {
		printf "%d * %d = %d\n", horiz, depth, horiz * depth;
	}
' < "$1"

