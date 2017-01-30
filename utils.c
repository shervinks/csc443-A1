#include "utils.h"
#include <stdio.h>

/** Print out the context of buffer. For testing purpose only **/
void print_buffer(Record r[], int size) {
	int i = 0;
	while(i < size) {
		printf("First: %d, Second: %d\n", r[i].uid1, r[i].uid2);
		i++;
	}
}

/** Print out the content of a binary file**/
int print_dat_file(char *filename) {
	FILE *fp_read;
	/* open dat file for reading */
	if (!(fp_read= fopen ( filename , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}
	// calculate the number of records in the dat file
	fseek(fp_read, 0L, SEEK_END);
    long filesize = ftell(fp_read);
	long number_of_records = filesize / sizeof(Record);
	// return the file pointer to the start
    fseek(fp_read, 0L, SEEK_SET);

	Record r[number_of_records];
	/* reading records */
	int n = fread (&r, 8, number_of_records, fp_read);
	int i;
	for (i=0; i < n; i++) {
		printf("uid1: %d, uid2: %d\n", r[i].uid1, r[i].uid2);
	}
	fclose(fp_read);
	printf("Total number of records: %ld, Records read: %d\n", number_of_records, n);
	return 0;
}

/** calculate the average and max for the given list of records **/
void get_calculation(Record * buffer, int size, float calculations[], int index, int start_pos){
	int i;
	int current_id = buffer[0].uid1;
	int temp_max = 0;
	int max = 0;
	int unique = 1;
	for (i = start_pos; i < size; i++){
		// check if uid1 has changed
		if (current_id == buffer[i].uid1){
			temp_max++;
		} else {
			current_id = buffer[i].uid1;
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
	calculations[index] = max;
	calculations[index+1] = (float)(size-start_pos)/unique;
	//printf("max: %.6f average: %.6f\n", calculations[index], calculations[index+1]);
}

/** calculate and print the overall max and average **/
void print_calculations(float calculations[], int size){
	int i;
	int max = 0;
	float total = 0; 
	for (i = 0; i < size; i++){
		if (calculations[2*i] > max){
			max = calculations[2*i];
		}
		total += calculations[2*i+1];
	}
	
	printf("Max: %d Average: %.6f\n", max, total/size);
}











