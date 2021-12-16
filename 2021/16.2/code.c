#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_BITS (4096 * 8)

int bits[MAX_BITS];
int nb_bits = 0;

int versions_sum = 0;


long long int read_packet(int *start);

int main(int argc, char *argv[])
{
	int c;

	while ((c = getchar()) != EOF) {
		if (isupper(c)) {
			c -= 'A' - 10;
		} else if (isdigit(c)) {
			c -= '0';
		} else {
			continue;
		}
		bits[nb_bits++] = (c >> 3) & 0x01;
		bits[nb_bits++] = (c >> 2) & 0x01;
		bits[nb_bits++] = (c >> 1) & 0x01;
		bits[nb_bits++] = (c >> 0) & 0x01;
	}
	fprintf(stderr, "Found %d bits: ", nb_bits);
	for (int i = 0; i < nb_bits; i++)
		fprintf(stderr, "%d", bits[i]);
	fprintf(stderr, "\n");

	int start = 0;
	long long int value = read_packet(&start);

	fprintf(stdout, "\nValue = %lld\n", value);

	return 0;
}



long long int read_packet(int *start)
{
	int packet_version;
	int packet_type;
	long long int retval = 0;

	packet_version = (bits[*start] << 2) | (bits[(*start) + 1] << 1) | (bits[(*start) + 2]);
	(*start) +=3;
	fprintf(stderr, "Packet version =  %d\n", packet_version);
	versions_sum += packet_version;

	packet_type = (bits[*start] << 2) | (bits[(*start) + 1] << 1) | (bits[(*start) + 2]);
	(*start) +=3;
	fprintf(stderr, "Packet type =  %d\n", packet_type);

	if (packet_type == 4) {
		long long int value = 0;
		do {
			(*start)++;
			for (int i = 0; i < 4; i ++) {
				value <<= 1;
				value |= bits[*start];
				(*start)++;
			}
		} while (bits[(*start) - 5] == 1);
		retval = value;
		fprintf(stderr, "Value = %lld\n", retval);
	} else {

		int length_type;
		length_type = bits[*start];
		(*start)++;
		fprintf(stderr, "Length type =%d\n", length_type);
		long long int val;

		if (length_type == 0) {
			int subpackets_length = 0;
			for (int i = 0; i < 15; i ++) {
				subpackets_length <<= 1;
				subpackets_length |= bits[*start];
				(*start) ++;
			}
			int end = subpackets_length + (*start);
			int n = 0;
			while ((*start) < end) {
				val = read_packet(start);
				switch(packet_type) {
					case 0: retval += val; break;
					case 1: if (n == 0) retval = val; else retval *= val; break;
					case 2: if ((retval > val) || (n == 0)) retval = val; break;
					case 3: if ((retval < val) || (n == 0)) retval = val; break;
					case 5: if (n == 0) { retval = val; } else { retval = (retval > val); } break;
					case 6: if (n == 0) { retval = val; } else { retval = (retval < val); } break;
					case 7: if (n == 0) { retval = val; } else { retval = (retval == val); } break;
					default: break;
				}
				n++;
			}

		} else {
			int subpackets_number = 0;
			for (int n = 0; n < 11; n ++) {
				subpackets_number <<= 1;
				subpackets_number |= bits[*start];
				(*start) ++;
			}
			fprintf(stderr, "Subpackets number = %d\n", subpackets_number);
			for (int n = 0; n < subpackets_number; n++) {
				val = read_packet(start);
				switch(packet_type) {
					case 0: retval += val; break;
					case 1: if (n == 0) retval = val; else retval *= val; break;
					case 2: if ((retval > val) || (n == 0)) retval = val; break;
					case 3: if ((retval < val) || (n == 0)) retval = val; break;
					case 5: if (n == 0) { retval = val; } else { retval = (retval > val); } break;
					case 6: if (n == 0) { retval = val; } else { retval = (retval < val); } break;
					case 7: if (n == 0) { retval = val; } else { retval = (retval == val); } break;
					default: break;
				}
			}
		}

	}
	return retval;
}
