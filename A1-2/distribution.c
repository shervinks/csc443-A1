#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merge.h"

void print_hist(int *histogram, int size, int column_id) {
	for (int i = 0; i < size; i++) {
		if (column_id == 1) {
			printf("Out-Degree: %d, Count: %d\n", i+1, histogram[i]);
		}
		else {
			printf("In-Degree: %d, Count: %d\n", i+1, histogram[i]);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("distribution takes 4 arguments: distribution <file_name> <block_size> <column_id> <max_degree>\n");
		return -1;
	}

	FILE *fp_read;
	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	int column_id;
	/* Check if a valid column id is passed of not*/
	if (!strncmp(argv[3], "UID1", 4)) {
		column_id = 1;
	}
	else if (!strncmp(argv[3], "UID2", 4)) {
		column_id = 2;
	}
	else {
		printf("<column_id> should be either \"UID1\" or \"UID2\"\n");
		return -1;
	}

	int block_size = atoi(argv[2]);
	// check if block_size is a multiple of sizeof(record)
	if (block_size % sizeof(Record) != 0){
		printf ("Block size should be a multiple of record size\n");
		return (-1);
	}

	/* create an array to store the count of eack in/out degree. So element i
	in the array will correspond to the count of degree i+1 in the histogram*/
	int max_degree = atoi(argv[4]);
	int *histogram = (int *) calloc (max_degree, sizeof(int));

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
	Record * buffer = (Record *) calloc (records_per_block, sizeof (Record));

	/* Read the dat file in blocks and fill up the histogram array*/
	int current_id = -1;
	int count = 0;
	int uid;
	for (int i = 0; i < number_of_blocks; i++) {
		int n = fread (buffer, sizeof(Record), records_per_block, fp_read);
		/* Read the block*/
		for (int j = 0; j < n; j++) {
			/* Check if were are calculating in-degree or out-degree */
			if (column_id == 1) {
				uid = buffer[j].UID1;
			}
			else {
				uid = buffer[j].UID2;
			}

			if (current_id != uid) {
				current_id = uid;
				if (count > 0 && count <= max_degree) histogram[count-1]++;
				count = 1;
			}
			else {
				count++;
			}
		}
	}
	if (count > 0 && count <= max_degree) histogram[count-1]++;
	print_hist(histogram, max_degree, column_id);
	return 0;
}
