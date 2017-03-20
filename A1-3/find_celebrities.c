#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#define MAX_PATH_LENGTH 1024

typedef struct record {
	int uid1;
	int uid2;
} Record;

typedef struct indeg {
	int uid2;
	int indeg;
} Indeg;

typedef struct outdeg {
	int uid1;
	int outdeg;
} Outdeg;

typedef struct celebrity {
	int uid;
	int indeg;
	int outdeg;
} Celebrity;

int compare (const void *a, const void *b){
	int a_f = ((const struct celebrity*)a)->indeg - ((const struct celebrity*)a)->outdeg;
	int b_f = ((const struct celebrity*)b)->indeg - ((const struct celebrity*)b)->outdeg;

    return (b_f - a_f);
}

int insert_celebrity(Celebrity c, Celebrity * top_10, int last_pos){
	if (last_pos < 9){
		top_10[last_pos].uid = c.uid;
		top_10[last_pos].indeg = c.indeg;
		top_10[last_pos].outdeg = c.outdeg;
		qsort(top_10, last_pos + 2, sizeof(Celebrity), compare);
		return last_pos + 1;
	} else {
		if ((top_10[last_pos].indeg - top_10[last_pos].outdeg) < (c.indeg - c.outdeg)){ 
			top_10[last_pos].uid = c.uid;
			top_10[last_pos].indeg = c.indeg;
			top_10[last_pos].outdeg = c.outdeg;
			// sort top_10
			qsort(top_10, 10, sizeof(Celebrity), compare);
			
		}
		return last_pos;
	}
	return 0;
}

// Return 1 if r is in the array records else return 0. Based on the binary search code in
// https://www.tutorialspoint.com/data_structures_algorithms/binary_search_program_in_c.htm
int binary_search(Outdeg * records, Indeg r, int size) {
    int lowerBound = 0;
    int upperBound = size -1;
    int midPoint = -1;

    while(lowerBound <= upperBound) {
        // compute the mid point
        // midPoint = (lowerBound + upperBound) / 2;
        midPoint = lowerBound + (upperBound - lowerBound) / 2;
		//printf("mid: %d\n", midPoint);

		//printf("uid2: %d uid1 %d\n", records[midPoint].uid2, r.uid1);
        // data found
        if(records[midPoint].uid1 == r.uid2) {
            return midPoint;
        } else {
            // if data is larger
            if(records[midPoint].uid1 < r.uid2) {
                // data is in upper half
                lowerBound = midPoint + 1;
            }
            // data is smaller
            else {
                // data is in lower half
                upperBound = midPoint -1;
            }
        }
    }

	// check for selfish celebrities (not following anyone)
	if (lowerBound != 0 && upperBound != size - 1){
		return -2;
	}
    return -1;
}

char* concat(char *s1, int s2){
	char str[MAX_PATH_LENGTH];
	snprintf(str, 32, "%d", s2);
	//str = (char*)(s2);
	char *result = malloc(strlen(s1)+strlen(str)+1);
	strcpy(result, s1);
	strcat(result, str);
	return result;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("find_clebrities takes 2 arguments: find_celebrities <input filename> <block size>\n");
        return -1;
    }
    int block_size = atoi(argv[2]);
    // check if block_size is a multiple of sizeof(record)
    if (block_size % sizeof(Record) != 0){
        printf ("Block size is not a multiple of record size\n");
        return (-1);
    }

	int memory = 200 * 1024 * 1024; // memory contraint
	char child[MAX_PATH_LENGTH];
	char temp[MAX_PATH_LENGTH];


	/* create new table for outdeg */

	// create sorted file by uid1
	strcpy(temp, "./disk_sort_1 ");
	strcat(temp, argv[1]);
	strcat(temp, " ");
	strcpy(child, concat(temp, memory));
	strcat(child, " ");
	strcat(child, argv[2]);
	if (system (child) == -1){
		printf("Error when sorting\n");
		exit(1);
	}
	if (system ("make clean_runs") == -1){
		printf("Error when cleaning\n");
		exit(1);
	}

	/* open sorted file for reading */
	FILE *fp_read_outdeg;
    if (!(fp_read_outdeg= fopen ("sorted_1.dat" , "rb" ))) {
        printf ("Could not open 'sorted_1.dat' for reading \n");
        return (-1);
    }
	// calculate filesize
	fseek(fp_read_outdeg, 0L, SEEK_END);
	long filesize = ftell(fp_read_outdeg);
    // return the file pointer to the start
	fseek(fp_read_outdeg, 0L, SEEK_SET);
	int read_mem = (memory * 9) / 10;
	read_mem -= read_mem % block_size;

	int write_mem = memory / 10;
	write_mem -= write_mem % block_size;
	
	int records_per_block = block_size / sizeof(Record);
	int m = read_mem / block_size; // number of blocks we can hold at any given time
	int number_of_blocks = filesize / block_size; // number of blocks to read
	if (filesize % block_size != 0){
		number_of_blocks++;
	}
	int loops = number_of_blocks / m + 1;

	Record * outdeg_in_buffer = (Record *) calloc (records_per_block * m, sizeof(Record));

	// create a new binary file for writing
	FILE *fp_write_outdeg;
	if (!(fp_write_outdeg= fopen ("outdeg.dat" , "wb" ))) {
        printf ("Could not open 'outdeg.dat' for writing \n");
        return (-1);
    }

	int output_buffer_capacity = write_mem / sizeof(Outdeg);
	int output_buffer_count = 0;

	Outdeg * outdeg_out_buffer = (Outdeg *) calloc (output_buffer_capacity, sizeof(Outdeg));

	int cur_uid1 = -1;
	int cur_count = 0;
	for (int i = 0; i < loops; i++){
		int records_read = fread(outdeg_in_buffer, sizeof(Record), records_per_block * m, fp_read_outdeg);
		for (int j = 0; j < records_read; j++){
			//printf("uid1: %d\n", outdeg_in_buffer[j].uid1);
			if (cur_uid1 == -1){
				cur_uid1 = outdeg_in_buffer[j].uid1;
				cur_count++;
			} else {
				if (cur_uid1 != outdeg_in_buffer[j].uid1){
					// complete current count and write to output buffer
					outdeg_out_buffer[output_buffer_count].uid1 = cur_uid1;
					outdeg_out_buffer[output_buffer_count].outdeg = cur_count;
					output_buffer_count++;
					cur_uid1 = outdeg_in_buffer[j].uid1;
					cur_count = 1;	
				} else {
					cur_count++;
				}
			}
			// check if output buffer is full
			if (output_buffer_count == output_buffer_capacity){
				// write to disk
				fwrite (outdeg_out_buffer, sizeof(Outdeg), output_buffer_count, fp_write_outdeg);
				fflush(fp_write_outdeg);
				output_buffer_count = 0;
			}
		}
	}
	// complete last count and write to ouput buffer
	outdeg_out_buffer[output_buffer_count].uid1 = cur_uid1;
	outdeg_out_buffer[output_buffer_count].outdeg = cur_count;
	output_buffer_count++;
	// empty whatever is left in the output buffer
	fwrite (outdeg_out_buffer, sizeof(Outdeg), output_buffer_count, fp_write_outdeg);
	fflush(fp_write_outdeg);
	
	// free buffers
	free(outdeg_in_buffer);
	free(outdeg_out_buffer);

	// close FPs
	fclose(fp_read_outdeg);
	fclose(fp_write_outdeg);
	
	/* create new table for indeg */

	// create sorted file by uid2
	strcpy(temp, "./disk_sort_2 ");
	strcat(temp, argv[1]);
	strcat(temp, " ");
	strcpy(child, concat(temp, memory));
	strcat(child, " ");
	strcat(child, argv[2]);
	if (system (child) == -1){
		printf("Error when sorting\n");
		exit(1);
	}
	if (system ("make clean_runs") == -1){
		printf("Error when cleaning\n");
		exit(1);
	}

	/* open sorted file for reading */
	FILE *fp_read_indeg;
    if (!(fp_read_indeg= fopen ("sorted_2.dat" , "rb" ))) {
        printf ("Could not open 'sorted_1.dat' for reading \n");
        return (-1);
    }
	// calculate filesize
	fseek(fp_read_indeg, 0L, SEEK_END);
	filesize = ftell(fp_read_indeg);
    // return the file pointer to the start
	fseek(fp_read_indeg, 0L, SEEK_SET);
	

	number_of_blocks = filesize / block_size; // number of blocks to read
	if (filesize % block_size != 0){
		number_of_blocks++;
	}
	loops = number_of_blocks / m + 1;

	Record * indeg_in_buffer = (Record *) calloc (records_per_block * m, sizeof(Record));

	// create a new binary file for writing
	FILE *fp_write_indeg;
	if (!(fp_write_indeg= fopen ("indeg.dat" , "wb" ))) {
        printf ("Could not open 'indeg.dat' for writing \n");
        return (-1);
    }

	output_buffer_capacity = write_mem / sizeof(Indeg);
	output_buffer_count = 0;

	Indeg * indeg_out_buffer = (Indeg *) calloc (output_buffer_capacity, sizeof(Indeg));

	int cur_uid2 = -1;
	cur_count = 0;
	for (int i = 0; i < loops; i++){
		int records_read = fread(indeg_in_buffer, sizeof(Record), records_per_block * m, fp_read_indeg);
		for (int j = 0; j < records_read; j++){
			//printf("uid1: %d\n", outdeg_in_buffer[j].uid1);
			if (cur_uid2 == -1){
				cur_uid2 = indeg_in_buffer[j].uid2;
				cur_count++;
			} else {
				if (cur_uid2 != indeg_in_buffer[j].uid2){
					// complete current count and write to output buffer
					indeg_out_buffer[output_buffer_count].uid2 = cur_uid2;
					indeg_out_buffer[output_buffer_count].indeg = cur_count;
					output_buffer_count++;
					cur_uid2 = indeg_in_buffer[j].uid2;
					cur_count = 1;	
				} else {
					cur_count++;
				}
			}
			// check if output buffer is full
			if (output_buffer_count == output_buffer_capacity){
				// write to disk
				fwrite (indeg_out_buffer, sizeof(Indeg), output_buffer_count, fp_write_indeg);
				fflush(fp_write_indeg);
				output_buffer_count = 0;
			}
		}
	}
	// complete last count and write to ouput buffer
	indeg_out_buffer[output_buffer_count].uid2 = cur_uid2;
	indeg_out_buffer[output_buffer_count].indeg = cur_count;
	output_buffer_count++;
	// empty whatever is left in the output buffer
	fwrite (indeg_out_buffer, sizeof(Indeg), output_buffer_count, fp_write_indeg);
	fflush(fp_write_indeg);
	
	// free buffers
	free(indeg_in_buffer);
	free(indeg_out_buffer);

	// close FPs
	fclose(fp_read_indeg);
	fclose(fp_write_indeg);

	/* get top 10 celebtrities */

	Celebrity * top_10 = (Celebrity *) calloc (10, sizeof(Celebrity));
	int last_pos = 0;
	

	/* open outdeg file for reading */
    if (!(fp_read_outdeg= fopen ("outdeg.dat" , "rb" ))) {
        printf ("Could not open 'outdeg.dat' for reading \n");
        return (-1);
    }
	// calculate filesize
	fseek(fp_read_outdeg, 0L, SEEK_END);
	int outdeg_filesize = ftell(fp_read_outdeg);
    // return the file pointer to the start
	fseek(fp_read_outdeg, 0L, SEEK_SET);

	/* open sorted file for reading */
    if (!(fp_read_indeg= fopen ("indeg.dat" , "rb" ))) {
        printf ("Could not open 'indeg.dat' for reading \n");
        return (-1);
    }
	// calculate filesize
	fseek(fp_read_indeg, 0L, SEEK_END);
	int indeg_filesize = ftell(fp_read_indeg);
    // return the file pointer to the start
	fseek(fp_read_indeg, 0L, SEEK_SET);

	m = memory / block_size; // number of blocks we can hold at any given time
	int chunk_size = ((m - 1) * block_size);

	// calculate loop numbers
	number_of_blocks = indeg_filesize / block_size;
	if (indeg_filesize % block_size != 0){
		number_of_blocks++;
	}
	//printf("number of b: %d\n", number_of_blocks);
    int chunk_num = outdeg_filesize / chunk_size;
    if (outdeg_filesize % chunk_size != 0){
		chunk_num++;
	}

	Celebrity temp_celebrity = {.uid = 0, .indeg = 0, .outdeg = 0};
	//printf("b/indeg: %lu\n", block_size / sizeof (Indeg));
	Indeg * buffer_indeg = (Indeg *) calloc (block_size / sizeof (Indeg), sizeof (Indeg));
    Outdeg * buffer_outdeg = (Outdeg *) calloc (chunk_size/ sizeof (Outdeg), sizeof (Outdeg));
	// implement BNJL
	for (int i=0; i < chunk_num; i++) {
		int read_outdeg = fread (buffer_outdeg, sizeof(Outdeg), chunk_size / sizeof (Outdeg), fp_read_outdeg);
		//printf("read_outdeg: %d\n", read_outdeg);
		fseek(fp_read_indeg, 0L, SEEK_SET);
		for (int j=0; j < number_of_blocks; j++){
			int read_indeg = fread (buffer_indeg, sizeof(Indeg), block_size / sizeof (Indeg), fp_read_indeg);
			//printf("read_indeg: %d\n", read_indeg);
			for (int k = 0; k < read_indeg; k++){
				//printf("uid: %d outdeg: %d\n", buffer_outdeg[k].uid1, buffer_outdeg[k].outdeg);
				int index = binary_search(buffer_outdeg, buffer_indeg[k], read_outdeg);
				if (index != -1){
					// found, create a celebrity
					temp_celebrity.uid = buffer_indeg[k].uid2;
					temp_celebrity.indeg = buffer_indeg[k].indeg;
					temp_celebrity.outdeg = buffer_outdeg[index].outdeg;
					last_pos = insert_celebrity(temp_celebrity, top_10, last_pos);
				} else if (index == -1){
					temp_celebrity.uid = buffer_indeg[k].uid2;
					temp_celebrity.indeg = buffer_indeg[k].indeg;
					temp_celebrity.outdeg = 0;
					last_pos = insert_celebrity(temp_celebrity, top_10, last_pos);				
				}
			}
		}
	}

	// print top 10 celebrities
	for (int i = 0; i < 10; i++){
		printf("uid: %d indeg: %d outdeg: %d diff: %d\n", top_10[i].uid, top_10[i].indeg, 
			top_10[i].outdeg, top_10[i].indeg - top_10[i].outdeg);
	} 
	return 0;

}


	

	
	

