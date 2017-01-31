#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
        printf("write_ram_rand takes 1 arguments: write_ram_rand X\n");
        return -1;
    }
    FILE *fp_read;
	/* open dat file for updating */
	if (!(fp_read= fopen ( "records.dat" , "r" ))) {
		printf ("Could not open file \"%s\" for reading \n", "records.dat");
		return (-1);
	}
	//Create a dummy record to insert into dat file
	Record *r = (Record *) malloc (sizeof(Record)); 
	r->uid1 = 1;
	r->uid2 = 1;
	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
	long filesize =  ftell(fp_read);
	long total_records = filesize / sizeof(Record);
	// return the file pointer to the start
	fseek(fp_read, 0L, SEEK_SET);
	Record * buffer = (Record *) calloc (total_records, sizeof(Record));
	// Load whole data in RAM
	int n = fread (buffer, sizeof(Record), total_records, fp_read);
	int x = atoi(argv[1]);
	int i;
	int r_val;
	srand(time(NULL));
	for (i=0; i<x; i++) {
		r_val = rand() % total_records;
		printf("rval: %d\n", r_val);
		buffer[r_val] = *r;
	}
	free(r);
	free(buffer);
	fclose (fp_read);
	// testing purpose only. needs to remove it
	for (i=0; i < total_records; i++) {
		printf("uid1: %d, uid2: %d\n", buffer[i].uid1, buffer[i].uid2);
	}
	printf("Total number of records: %ld, Records read: %d\n", total_records, n);
	return 0;	
}
 
