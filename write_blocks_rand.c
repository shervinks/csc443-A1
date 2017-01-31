#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
        printf("write_blocks_rand takes 1 arguments: write_blocks_rand X\n");
        return -1;
    }
    FILE *fp_write;
	/* open dat file for updating */
	if (!(fp_write= fopen ( "records.dat" , "r+" ))) {
		printf ("Could not open file \"%s\" for reading \n", "records.dat");
		return (-1);
	}
	//Create a dummy record to insert into dat file
	Record *r = (Record *) malloc (sizeof(Record)); 
	r->uid1 = 1;
	r->uid2 = 2;
	// calculate the number of records in the dat file
	fseek(fp_write, 0L, SEEK_END);
	long filesize = ftell(fp_write);
	long total_records = filesize / sizeof(Record);
	// return the file pointer to the start
	fseek(fp_write, 0L, SEEK_SET);
	
	int x = atoi(argv[1]);
	int i;
	int r_val;
	srand(time(NULL));
	for (i=0; i<x; i++) {
		r_val = rand() % total_records;
		fseek(fp_write, r_val*sizeof(Record), SEEK_SET);
		fwrite(r, sizeof(Record), 1, fp_write);
	}
	free(r);
	fclose (fp_write);
	print_dat_file("records.dat");
	return 0;	
}
