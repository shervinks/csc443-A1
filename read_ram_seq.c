#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
        printf("read_blocks_seq takes 1 argument: read_blocks_seq <binary input filename> \n");
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
	long total_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);
	
	Record * buffer = (Record *) calloc (total_records, sizeof (Record));

	/* reading records */
	float calculations[2];
	int n = fread (buffer, sizeof(Record), total_records, fp_read);
	get_calculation(buffer, n, calculations, 0, 0);
	
	print_calculations(calculations, 1);	

	return 0;
}
