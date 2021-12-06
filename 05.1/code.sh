#! /bin/bash

nb_segments=0

increase_cell()
{
	point=${lines[$2]:$1:1}
	point=$((${point} + 1))
	lines[$2]=${lines[$2]:0:$(($1))}${point}${lines[$2]:$(($1 + 1))}
}

display_lines()
{
	for y in $(seq ${ymin} ${ymax})
	do
		printf "%4d " $y
		echo ${lines[$y]}
	done
}

ymin=-1
ymax=0
xmax=0

while read line
do
	x1[$nb_segments]="${line%%,*}"
	line="${line#*,}"
	if [ ${x1[$nb_segments]} -gt $xmax ]; then xmax=${x1[$nb_segments]}; fi
	y1[$nb_segments]=${line%% -*}
	line="${line#*> }"
	if [ ${y1[$nb_segments]} -gt $ymax ]; then ymax=${y1[$nb_segments]}; fi
	if [ ${y1[$nb_segments]} -lt $ymin ] || [ $ymin -lt 0 ]; then ymin=${y1[$nb_segments]}; fi

	x2[$nb_segments]="${line%%,*}"
	line="${line#*,}"

	if [ ${x2[$nb_segments]} -gt $xmax ]; then xmax=${x2[$nb_segments]}; fi

	y2[$nb_segments]=${line}
	if [ ${y2[$nb_segments]} -gt $ymax ]; then ymax=${y2[$nb_segments]}; fi
	if [ ${y2[$nb_segments]} -lt $ymin ] || [ $ymin -lt 0 ]; then ymin=${y2[$nb_segments]}; fi

	nb_segments=$((nb_segments + 1))
done

#printf "nb segments = %d, ymin=%d, ymax=%d, xmax=%d\n" ${nb_segments} $ymin $ymax $xmax >&2


line=""
for x in $(seq 0 ${xmax})
do
	line="${line}0"
done

for y in $(seq ${ymin} ${ymax})
do
	lines[$y]="$line"
done

#display_lines

for seg in $(seq 0 $((${nb_segments} - 1)) )
do
# echo "${x1[$seg]},${y1[$seg]} -- ${x2[$seg]},${y2[$seg]}"
	if [ ${x1[$seg]} -eq ${x2[$seg]} ]
	then
		if [ ${y1[$seg]} -lt ${y2[$seg]} ]
		then
			start=${y1[$seg]}
			end=${y2[$seg]}
		else
			start=${y2[$seg]}
			end=${y1[$seg]}
		fi
		for y in $(seq $start $end)
		do
			increase_cell ${x1[$seg]} $y
		done

	elif [ ${y1[$seg]} -eq ${y2[$seg]} ]
	then
		if [ ${x1[$seg]} -lt ${x2[$seg]} ]
		then
			start=${x1[$seg]}
			end=${x2[$seg]}
		else
			start=${x2[$seg]}
			end=${x1[$seg]}
		fi
		for x in $(seq $start $end)
		do
			increase_cell $x ${y1[$seg]}
		done
	fi
done

display_lines

count=0

for y in $(seq ${ymin} ${ymax})
do
	for  x in $(seq 0 ${xmax})
	do
		point=${lines[$y]:$x:1}
		if [ $point -gt 1 ]
		then
			count=$((count + 1))
		fi
	done
done


echo "Number of segments $nb_segments, xmax=$xmax, ymin=$ymin, ymax=$ymax, count=$count"






