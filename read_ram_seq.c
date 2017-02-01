#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
        printf("read_blocks_seq takes 1 argument: read_blocks_seq <binary input filename> \n");
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

	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
    long filesize = ftell(fp_read);
	long total_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);
	
	Record * buffer = (Record *) calloc (total_records, sizeof (Record));

	/* reading records */
	int n = fread (buffer, sizeof(Record), total_records, fp_read);

	// beging recording time
	ftime(&t_begin);

	int j;
	int temp_max = 0;
	int max = 0;
	int unique = 1;
	int current_id = buffer[0].uid1;

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
	
	// finish recording time
	ftime(&t_end);

	free(buffer);
	fclose(fp_read);

	time_spent_ms = (long double) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm));
	printf("Max: %d Average: %.6f\n", max, (float)total_records/unique);
	printf ("Data rate: %.9f MBPS\n", 
		((n*sizeof(Record))/(float)time_spent_ms * 1000)/1048576);
	printf("n: %d\n", n);

	return 0;
}
