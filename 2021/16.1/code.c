#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_BITS (4096 * 8)

int bits[MAX_BITS];
int nb_bits = 0;

int versions_sum = 0;


void read_packet(int *start);

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
	read_packet(&start);

	fprintf(stdout, "\nVersions number sum = %d\n", versions_sum);

	return 0;
}



void read_packet(int *start)
{
	int packet_version;
	int packet_type;

	packet_version = (bits[*start] << 2) | (bits[(*start) + 1] << 1) | (bits[(*start) + 2]);
	(*start) +=3;
	fprintf(stderr, "Packet version =  %d\n", packet_version);
	versions_sum += packet_version;

	packet_type = (bits[*start] << 2) | (bits[(*start) + 1] << 1) | (bits[(*start) + 2]);
	(*start) +=3;
	fprintf(stderr, "Packet type =  %d\n", packet_type);

	if (packet_type == 4) {
		while (bits[*start] == 1) {
			(*start) += 5;
		}
		(*start) += 5;
//		while (((*start) % 8) != 0)
//			(*start) ++;
	} else {

		int length_type;
		length_type = bits[*start];
		(*start)++;
		fprintf(stderr, "Length type =%d\n", length_type);
		if (length_type == 0) {
			int subpackets_length = 0;
			for (int i = 0; i < 15; i ++) {
				subpackets_length <<= 1;
				subpackets_length |= bits[*start];
				(*start) ++;
			}
			int end = subpackets_length + (*start);
			while ((*start) < end) {
				read_packet(start);
			}

		} else {
			int subpackets_number = 0;
			for (int i = 0; i < 11; i ++) {
				subpackets_number <<= 1;
				subpackets_number |= bits[*start];
				(*start) ++;
			}
			fprintf(stderr, "Subpackets number = %d\n", subpackets_number);
			for (int i = 0; i < subpackets_number; i++)
				read_packet(start);
		}

	}
}
