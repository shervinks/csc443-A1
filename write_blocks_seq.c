#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_CHARS_PER_LINE 16

/** Convert line into Record **/
Record parse_line(char line[]) {
	Record r;
	char * token = strtok(line, ",");
	r.uid1 = atoi(token);
    token = strtok(NULL, "\r\n");
    r.uid2 = atoi(token);
	return r;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("write_blocks_seq takes 2 arguments: write_blocks_seq <input filename> <block size>\n");
        return -1;
    }
	long block_size = atol(argv[2]);
	long records_per_block = block_size / sizeof(Record);
	char current_line[MAX_CHARS_PER_LINE];
	// Change buffer size to records_per_block. This is temp for testing
	Record buffer[27];
	FILE *fp_read;
	/* open text file for reading */
	if (!(fp_read= fopen ( argv[1] , "r" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}
	int i = 0;
	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		buffer[i] = parse_line(current_line);
		i++;
	}
	fclose (fp_read);
	// Write a dat file. This is temporary. Need to move this to inside the while loop
	FILE *fp_write;
	if (!(fp_write = fopen ( "records.dat" , "wb" ))) {
		printf ("Could not open file \"%s\" for writing \n", "records.dat");
		return (-1);
	}
	fwrite ( &buffer, sizeof(Record), 27, fp_write);
	fflush (fp_write);
	fclose (fp_write);
	//free(buffer);
	
	print_dat_file("records.dat");
	return 0;
}