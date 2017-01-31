#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#define MAX_CHARS_PER_LINE 16

int main(int argc, char *argv[]) {
	if (argc < 2) {
        printf("write_lines takes 1 arguments: write_lines <input filename>\n");
        return -1;
    } 
    char current_line[MAX_CHARS_PER_LINE];
    FILE *fp_read;
	FILE *fp_write;
	/* open text file for reading */
	if (!(fp_read= fopen ( argv[1] , "r" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}
	/* open binary file for writing */
	if (!(fp_write = fopen ( "lines.csv" , "w" ))) {
		printf ("Could not open file \"%s\" for writing \n", "lines.csv");
		return (-1);
	}
	int i = 0;
    struct timeb t_begin, t_end;
	long time_spent_ms;
	//Start timing
	ftime(&t_end);  	
	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		fwrite ( &current_line, strlen(current_line), sizeof(char), fp_write);
		i++;
	}
	fclose (fp_write);
	ftime(&t_end);
	time_spent_ms = (long double) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm));
	printf ("Data rate: %.9f MBPS\n", 
		(i/(float)time_spent_ms * 1000)/1000000);
	fclose (fp_read);
	return 0;
}
