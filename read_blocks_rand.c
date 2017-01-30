#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <time.h>

#define MAX_CHARS_PER_LINE 16


int main(int argc, char *argv[]) {
	if (argc < 4) {
        printf("read_blocks_seq takes 2 arguments: read_blocks_seq <binary input filename> <block size> <X>\n");
        return -1;
    }
	FILE *fp_read;

	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	long block_size = atol(argv[2]);
	int x = atoi(argv[3]);

	// check if block_size is a multiple of sizeof(record)
	if (block_size % sizeof(Record) != 0){
		printf ("Block size is not a multiple of record size\n");
		return (-1);
	}	

	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
	long filesize = ftell(fp_read);
	long total_records = filesize / sizeof(Record);
	// return the file pointer to the start
	fseek(fp_read, 0L, SEEK_SET);

	// calculate number of records in a block
	long records_per_block = block_size / sizeof(Record);

	Record * buffer;

	/* reading records */
	float calculations[x*2];
	int i;
	int r_val;
	srand(time(NULL));
	for (i = 0; i < x; i++){
		buffer = (Record *) calloc (records_per_block, sizeof (Record));
		// go to a random position in file
		r_val = rand() % total_records;
		fseek(fp_read, r_val*sizeof(Record), SEEK_SET);

		int n = fread (buffer, sizeof(Record), records_per_block, fp_read);
		get_calculation(buffer, n, calculations, i*2, 0);	
		free(buffer);
	}

	print_calculations(calculations, x);

	return 0;
}
