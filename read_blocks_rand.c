#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <time.h>

#define MAX_CHARS_PER_LINE 16


int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("read_blocks_seq takes 2 arguments: read_blocks_seq <binary input filename> <X>\n");
        return -1;
    }
	FILE *fp_read;

	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	int x = atoi(argv[2]);

	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
    long filesize = ftell(fp_read);
	long number_of_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);

	Record r[x];
	/* reading records randomly */
	int j;
	int r_val;
	int n = 0;
	int r_pos = 0;

	srand(time(NULL));

	for (j=0; j<x; j++){
		r_val = rand() % number_of_records;
		fseek(fp_read, 8*r_val, SEEK_SET);
		n += fread (&r[r_pos], 8, 1, fp_read);
		r_pos++;
	}

	// make a list of unique followers and how many they are following
	int i;
	int followers[number_of_records*2];
	int unique = 0;
	int index;
	for (i=0; i < n; i++) {
		index = exists(followers, unique, r[i].uid1);
		if (index == -1){
			// add follower to our list of accounted followers
			followers[unique*2] = r[i].uid1;
			followers[unique*2+1] = 1;	
			unique++;	
		} else {
			followers[index+1]++;
		}
	}
	print_followers(followers, unique);
	print_max_avg(followers, unique);

	return 0;
}
