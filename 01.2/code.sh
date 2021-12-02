#! /bin/sh


if [ ! -f "$1" ]
then
	echo "Please give me the input file as argument" >&2
	exit 1
fi


awk '
	BEGIN {
		count_1=-1;
		count_2=-2;
		count_3=-3;
		prev_1=-1;
		prev_2=-1;
		prev_3=-1;
	};

	{
		count_1++;
		count_2++;
		count_3++;
	};

	(count_1 >= 0) {
		sum_1 += $1;

		if (count_1 == 2) {
			printf "C1: %d ", sum_1;
			if ((prev_3 >= 0) && (sum_1 > prev_3)) {
				increases ++;
				printf "(increased)";
			};
			printf("\n");
			prev_1 = sum_1;
			sum_1 = 0;
			count_1 = -1;
		};
	 };


	(count_2 >= 0) {
		sum_2 += $1;

		if (count_2 == 2) {
			printf "C2: %d ", sum_2;
			if ((prev_1 >= 0) && (sum_2 > prev_1)) {
				increases ++;
				printf "(increased)";
			};
			printf("\n");
			prev_2 = sum_2;
			sum_2 = 0;
			count_2 = -1;
		};
	 };

	(count_3 >= 0) {
		sum_3 += $1;

		if (count_3 == 2) {
			printf "C3: %d ", sum_3;
			if ((prev_2 >= 0) && (sum_3 > prev_2)) {
				increases ++;
				printf "(increased)";
			};
			printf("\n");
			prev_3 = sum_3;
			sum_3 = 0;
			count_3 = -1;
		};
	 };

	END {
		print increases;
	}
' < "$1"

