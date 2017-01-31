#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <time.h>
#include <sys/timeb.h>

int main(int argc, char *argv[]) {
	if (argc < 4) {
        printf("read_blocks_seq takes 1 argument: read_blocks_seq <binary input filename> <block size> <x>\n");
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

	Record * buffer = (Record *) calloc (total_records, sizeof (Record));

	// beging recording time
	ftime(&t_begin);

	/* reading records */
	float calculations[x*2];
	int n = fread (buffer, sizeof(Record), total_records, fp_read);

	/* get random samples */
	int i;
	int r_val;
	int buffer_size;
	int total_records_read;
	srand(time(NULL));
	for (i = 0; i < x; i++){
		r_val = rand() % total_records;
		if (r_val + records_per_block >= total_records){
			buffer_size = total_records;
		} else {
			buffer_size = r_val + records_per_block;
		}
		total_records_read += buffer_size;
		get_calculation(buffer, buffer_size, calculations, i*2, r_val);
	}
	free(buffer);
	// finish recording time
	ftime(&t_end);

	time_spent_ms = (long double) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm));
	print_calculations(calculations, x);
	printf ("Data rate: %.9f MBPS\n", 
		((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/1000000);


	return 0;
}





	
