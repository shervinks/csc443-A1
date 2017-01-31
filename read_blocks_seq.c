#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.h"



int main(int argc, char *argv[]) {
	if (argc < 3) {
    	printf("read_blocks_seq takes 2 arguments: read_blocks_seq <binary input filename> <block size>\n");
        return -1;
    }
	FILE *fp_read;
	struct timeb t_begin, t_end;
	long time_spent_ms;

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
	long total_records = filesize / sizeof(Record);
	if (filesize % block_size != 0){
		number_of_blocks++;	
	}
	// return the file pointer to the start
	fseek(fp_read, 0L, SEEK_SET);

	// calculate number of records in a block
	long records_per_block = block_size / sizeof(Record);

	Record * buffer;
	
	ftime(&t_begin);
	/* reading records */
	int i;
	int j;
	int temp_max = 0;
	int max = 0;
	int unique = 1;
	int current_id = -1;
	for (i = 0; i < number_of_blocks; i++){
		buffer = (Record *) calloc (records_per_block, sizeof (Record));
		int n = fread (buffer, sizeof(Record), records_per_block, fp_read);

		if (current_id == -1) current_id = buffer[0].uid1;
		for (j = 0; j < n; j++){
			// check if uid1 has changed
			if (current_id == buffer[j].uid1){
				temp_max++;
			} else {
				current_id = buffer[j].uid1;
				if (temp_max > max){
					max = temp_max;
				}
				temp_max = 0;
				unique++;
			}	
		}
		if (temp_max > max){
			max = temp_max;
		}
		free(buffer);
	}
	ftime(&t_end);

	time_spent_ms = (long double) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm));

	printf("Max: %d Average: %.6f\n", max, (float)total_records/unique);
	printf ("Data rate: %.9f MBPS\n", 
		((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/1000000);
	printf("total records: %d\n", total_records);
	return 0;
}


