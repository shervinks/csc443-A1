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

	long block_size = atol(argv[2]);
	
	// check if block_size is a multiple of sizeof(record)
	if (block_size % sizeof(Record) != 0){
		printf ("Block size is not a multiple of record size\n");
		return (-1);
	}	

	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
	long filesize = ftell(fp_read);
	long number_of_blocks = filesize / block_size;
	if (filesize % block_size != 0){
		number_of_blocks++;	
	}
	// return the file pointer to the start
	fseek(fp_read, 0L, SEEK_SET);

	// calculate number of records in a block
	long records_per_block = block_size / sizeof(Record);

	Record * buffer = (Record *) calloc (records_per_block, sizeof (Record)) ;
	
	/* reading records */
	int calculations[number_of_blocks*2];
	int i;
	for (i = 0; i < number_of_blocks){
		int n = fread (&buffer, 8, records_per_block, fp_read);
		get_calculation(buffer, records_per_block, calculations, i);	
	}

	return 0;
}


