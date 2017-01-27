#include "utils.h"
#include <stdio.h>

/** Print out the context of buffer. For testing purpose only **/
void print_buffer(Record r[]) {
	int i = 0;
	while(i < 27) {
		printf("First: %d, Second: %d\n", r[i].uid1, r[i].uid2);
		i++;
	}
}

/** Print out the content of a binary file**/
int print_dat_file(char *filename) {
	FILE *fp_read;
	/* open dat file for reading */
	if (!(fp_read= fopen ( filename , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}
	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
    long filesize = ftell(fp_read);
	long number_of_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);

	Record r[number_of_records];
	/* reading records */
	int n = fread (&r, 8, number_of_records, fp_read);
	int i;
	for (i=0; i < n; i++) {
		printf("uid1: %d, uid2: %d\n", r[i].uid1, r[i].uid2);
	}
	fclose(fp_read);
	printf("Total number of records: %ld, Records read: %d\n", number_of_records, n);
	return 0;
}


