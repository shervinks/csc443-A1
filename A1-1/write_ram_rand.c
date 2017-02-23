#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
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
    	struct timeb t_begin, t_end;
	long time_spent_ms;
	//Start timing
	ftime(&t_begin);	
	for (i=0; i<x; i++) {
		r_val = rand() % total_records;
		buffer[r_val] = *r;
	}
	free(r);
	free(buffer);
	//End timing
	ftime(&t_end);
	time_spent_ms = (long double) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm));
	printf ("Data rate: %.9f MBPS\n", 
		((x*sizeof(Record))/(float)time_spent_ms * 1000)/1000000);
	fclose (fp_read);
	return 0;	
}
 
