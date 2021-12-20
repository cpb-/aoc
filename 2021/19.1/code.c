#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

typedef struct s_point {
	int x;
	int y;
	int z;
} point_t;


typedef struct s_beacon {
	point_t p;
} beacon_t;

beacon_t *beacons = NULL;
int nb_beacons = 0;


typedef struct s_scanner {
	beacon_t *beacons;
	int nb_beacons;
	point_t p;
	int computed;
	int **dist;
	int transf;
} scanner_t;

scanner_t *scanners = NULL;
int nb_scanners = 0;


int transform(int num, point_t *p, point_t *q);
int overlap(int s1, int s2);
void add_beacon(point_t p);

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int scanner = -1;
	int x, y, z;

	while (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		if (sscanf(buffer, "--- scanner %d ---", &scanner) == 1) {
			if (scanner != nb_scanners) {
				fprintf(stderr, "Unexpected scanner number: %d\n", scanner);
				exit(EXIT_FAILURE);
			}
			scanners = realloc(scanners, (nb_scanners + 1) * sizeof(scanner_t));
			scanners[nb_scanners].beacons = NULL;
			scanners[nb_scanners].nb_beacons = 0;
			scanners[nb_scanners].p.x = 0;
			scanners[nb_scanners].p.y = 0;
			scanners[nb_scanners].p.z = 0;
			scanners[nb_scanners].computed = 0;
			nb_scanners ++;
			continue;
		}
		if ((scanner > -1) && (sscanf(buffer, "%d,%d,%d", &x, &y, &z) == 3)){
			scanners[nb_scanners - 1].beacons = realloc(scanners[nb_scanners - 1].beacons, (scanners[nb_scanners - 1].nb_beacons + 1) * sizeof(beacon_t));
			scanners[nb_scanners - 1].beacons[scanners[nb_scanners - 1].nb_beacons].p.x = x;
			scanners[nb_scanners - 1].beacons[scanners[nb_scanners - 1].nb_beacons].p.y = y;
			scanners[nb_scanners - 1].beacons[scanners[nb_scanners - 1].nb_beacons].p.z = z;
			scanners[nb_scanners - 1].nb_beacons++;
			scanners[nb_scanners - 1].transf = 0;
			continue;
		}
	}
	scanners[0].computed = 1;

	int nb_computed = 1;

	while (nb_computed < nb_scanners) {
		for (int i = 0; i < nb_scanners; i ++) {
			if (! scanners[i].computed)
				continue;
			for (int j = 0; j < nb_scanners; j ++) {
				if (scanners[j].computed)
					continue;
				if (overlap(i, j)) {
					scanners[j].computed = 1;
					nb_computed ++;
					scanners[j].p.x += scanners[i].p.x;
					scanners[j].p.y += scanners[i].p.y;
					scanners[j].p.z += scanners[i].p.z;

					fprintf(stderr, "scanner %d at (%d, %d, %d)\n", j,
						scanners[j].p.x, scanners[j].p.y, scanners[j].p.z);
				}
			}
		}
	}

	for (int i = 0; i < nb_scanners; i++) {
		if (! scanners[i].computed)
                        continue;
		for (int j = 0; j < scanners[i].nb_beacons; j++) {
			point_t p;
			transform(scanners[i].transf, &(scanners[i].beacons[j].p), &p);
			p.x += scanners[i].p.x;
			p.y += scanners[i].p.y;
			p.z += scanners[i].p.z;
			add_beacon(p);
		}
	}
	fprintf(stderr, "Nb beacons: %d\n", nb_beacons);

	return 0;
}


void add_beacon(point_t p)
{
	for (int i = 0; i < nb_beacons; i++) {
		if ((p.x == beacons[i].p.x) && (p.y == beacons[i].p.y) && (p.z == beacons[i].p.z))
			return;
	}
	beacons = realloc(beacons, (nb_beacons + 1) * sizeof(beacon_t));
	beacons[nb_beacons].p.x = p.x;
	beacons[nb_beacons].p.y = p.y;
	beacons[nb_beacons].p.z = p.z;
	nb_beacons ++;
}



int translated(point_t m1, point_t p1, point_t m2, point_t p2)
{
	return (((m2.x - m1.x) == (p2.x - p1.x))
	     && ((m2.y - m1.y) == (p2.y - p1.y))
	     && ((m2.z - m1.z) == (p2.z - p1.z)));
}



int overlap(int s1, int s2)
{
	point_t  mm1, pp1;
	point_t  mm2, pp2;

	for (int t2 = 0; t2 < 24; t2 ++) {
		int count = 0;

		for (int m1 = 0; m1 < scanners[s1].nb_beacons; m1++) {
			transform(scanners[s1].transf, &(scanners[s1].beacons[m1].p), &mm1);

			for (int p1 = 0; p1 < scanners[s1].nb_beacons; p1++) {
				if (p1 == m1)
					continue;
				transform(scanners[s1].transf, &(scanners[s1].beacons[p1].p), &pp1);

				for (int m2 = 0; m2 < scanners[s2].nb_beacons - 1; m2++) {
					transform(t2, &(scanners[s2].beacons[m2].p), &mm2);

					for (int p2 = m2 + 1; p2 < scanners[s2].nb_beacons; p2++) {
						transform(t2, &(scanners[s2].beacons[p2].p), &pp2);

						scanners[s2].p.x = mm1.x - mm2.x;
						scanners[s2].p.y = mm1.y - mm2.y;
						scanners[s2].p.z = mm1.z - mm2.z;
						scanners[s2].transf = t2;

						if (translated(mm1, pp1, mm2, pp2))
							count++;
						if (count >= 12)
							return 1;
					 }
				}
			}
		}
	}
	return 0;
}



int transform(int num, point_t *p, point_t *q)
{
	switch (num) {

		default:
		case  0: q->x =  p->x;  q->y =  p->y;  q->z =  p->z; break;
		case  1: q->x =  p->x;  q->y =  p->z;  q->z = -p->y; break;
		case  2: q->x =  p->x;  q->y = -p->y;  q->z = -p->z; break;
		case  3: q->x =  p->x;  q->y = -p->z;  q->z =  p->y; break;

		case  4: q->x = -p->x;  q->y =  p->y;  q->z = -p->z; break;
		case  5: q->x = -p->x;  q->y = -p->z;  q->z = -p->y; break;
		case  6: q->x = -p->x;  q->y = -p->y;  q->z =  p->z; break;
		case  7: q->x = -p->x;  q->y =  p->z;  q->z =  p->y; break;

		case  8: q->x =  p->y;  q->y =  p->z;  q->z =  p->x; break;
		case  9: q->x =  p->y;  q->y =  p->x;  q->z = -p->z; break;
		case 10: q->x =  p->y;  q->y = -p->z;  q->z = -p->x; break;
		case 11: q->x =  p->y;  q->y = -p->x;  q->z =  p->z; break;

		case 12: q->x = -p->y;  q->y =  p->z;  q->z = -p->x; break;
		case 13: q->x = -p->y;  q->y = -p->x;  q->z = -p->z; break;
		case 14: q->x = -p->y;  q->y = -p->z;  q->z =  p->x; break;
		case 15: q->x = -p->y;  q->y =  p->x;  q->z =  p->z; break;

		case 16: q->x =  p->z;  q->y =  p->x;  q->z =  p->y; break;
		case 17: q->x =  p->z;  q->y =  p->y;  q->z = -p->x; break;
		case 18: q->x =  p->z;  q->y = -p->x;  q->z = -p->y; break;
		case 19: q->x =  p->z;  q->y = -p->y;  q->z =  p->x; break;

		case 20: q->x = -p->z;  q->y =  p->x;  q->z = -p->y; break;
		case 21: q->x = -p->z;  q->y = -p->y;  q->z = -p->x; break;
		case 22: q->x = -p->z;  q->y = -p->x;  q->z =  p->y; break;
		case 23: q->x = -p->z;  q->y =  p->y;  q->z =  p->x; break;
	}
	return 0;
}

