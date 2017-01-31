#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_CHARS_PER_LINE 32

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
	long block_capacity = block_size / sizeof(Record);
	char current_line[MAX_CHARS_PER_LINE];
	Record * buffer = (Record *) calloc (block_capacity, sizeof (Record)) ;
	FILE *fp_read;
	FILE *fp_write;
	/* open text file for reading */
	if (!(fp_read= fopen ( argv[1] , "r" ))) {
		printf ("Could not open file \"%s\" for writing \n", argv[1]);
		return (-1);
	}
	/* open binary file for writing */
	if (!(fp_write = fopen ( "records.dat" , "wb" ))) {
		printf ("Could not open file \"%s\" for writing\n", "records.dat");
		return (-1);
	}
	int i = 0;
	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		buffer[i] = parse_line(current_line);
		i++;
		// If the buffer is full flush the content into disk
		if (i == block_capacity) { // buffer is full
			fwrite ( buffer, sizeof(Record), block_capacity, fp_write);
			fflush (fp_write);
			i = 0;
		}
	}
	// Check if there is anything remaning in the buffer to write
	if (i > 0) {
		fwrite ( buffer, sizeof(Record), i, fp_write);
	}
	free(buffer);
	fclose (fp_write);
	fclose (fp_read);
	return 0;
}
