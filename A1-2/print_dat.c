#include "merge.h"

int main (int argc, char **argv) {
	//process and validate command-line arguments
	if (argc < 2) {
        printf("print_dat takes 1 argument: disk_sort <input filename>\n");
        return -1;
    }
	

	FILE *fp_read;

	/* open dat file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	/* get file size */
	fseek(fp_read, 0L, SEEK_END);
	long filesize = ftell(fp_read);
	fseek(fp_read, 0L, SEEK_SET);
	
	long total_records = filesize / sizeof(Record);
	
	Record * buffer = (Record *) calloc (total_records, sizeof (Record));

	int n = fread (buffer, sizeof(Record), total_records, fp_read);
	
	for (int i = 0; i < n; i++){
		printf("UID1: %d, UID2 %d\n", buffer[i].UID1, buffer[i].UID2);
	}
	free(buffer);
	return 0;
}
