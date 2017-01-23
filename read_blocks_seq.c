#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_CHARS_PER_LINE 16


int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("read_blocks_seq takes 2 arguments: read_blocks_seq <binary input filename> <block size>\n");
        return -1;
    }
	FILE *fp_read;

	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	// calculate number of records in a block
	long block_size = atol(argv[2]);
	long number_of_records = block_size / sizeof(Record);

	Record r[number_of_records];
	/* reading records */
	int n = fread (&r, 8, number_of_records, fp_read);

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


