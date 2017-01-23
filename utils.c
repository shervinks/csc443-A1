#include "utils.h"
#include <stdio.h>

/** Print out the context of buffer. For testing purpose only **/
void print_buffer(Record r[]) {
	int i = 0;
	while(i < 27) {
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

/** return index of follower in a list of followers, return -1 if it does not exist **/
int exists(int followers[], int unique, int follower){
	int i;
	for (i=0; i < unique * 2; i=i+2){
		if(followers[i] == follower){
			return i;
		}
	}
	return -1;
}

/** print out the max and average for followers **/
void print_max_avg(int followers[], int unique){
	/* print max and average */
	int i;
	int max = 0;
	float total = 0;
	for (i=0; i < unique * 2; i=i+2){
		if (followers[i+1] > max) max = followers[i+1];
		total += followers[i+1]; 
	}
	printf("Max: %d  Average: %.3f\n", max, total/unique);
}

/** print out each unique follower and how many people they are following, testing only **/ 
void print_followers(int followers[], int unique){
	int i;
	for (i=0; i < unique * 2; i=i+2){
		printf("follower: %d following: %d\n", followers[i], followers[i+1]);
	}
}
