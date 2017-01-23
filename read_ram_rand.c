#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <time.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("read_blocks_seq takes 1 argument: read_blocks_seq <binary input filename> <block size>\n");
        return -1;
    }
	FILE *fp_read;

	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
    long filesize = ftell(fp_read);
	long number_of_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);

	
	Record r[number_of_records];
	/* reading records into memory */
	int n = fread (&r, 8, number_of_records, fp_read);

	// make list of sample_size followers starting at a random position
	long sample_size = atoi(argv[2])/sizeof(Record);

	srand(time(NULL));
	int ran_val = rand() % (number_of_records - sample_size);

	int i;
	int followers[sample_size*2];
	int unique = 0;
	int index;
	for (i=ran_val; i < sample_size+ran_val; i++) {
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
