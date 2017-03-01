#include "merge.h"


int compare (const void *a, const void *b){
	int a_f = ((const struct record*)a)->UID2;
	int b_f = ((const struct record*)b)->UID2;
	return (a_f - b_f);
}


int main (int argc, char **argv) {
	//process and validate command-line arguments
	if (argc < 4) {
        printf("disk_sort takes 3 arguments: disk_sort <input filename> <total memory in bytes> <block size>\n");
        return -1;
    }
	

	FILE *fp_read;

	//get blocksize
	long blocksize = atol(argv[3]);
	//calculate allowed memory. Needed to use half of the memory because
	// qsort somehow doubles the allocated memory
	long a_mem = (atoi(argv[2]) - (atoi(argv[2]) % blocksize)) / 2;

	/* open binary file for reading */
	if (!(fp_read= fopen ( argv[1] , "rb" ))) {
		printf ("Could not open file \"%s\" for reading \n", argv[1]);
		return (-1);
	}

	// check if block_size is a multiple of sizeof(record)
	if (blocksize % sizeof(Record) != 0){
		printf ("Block size is not a multiple of record size\n");
		return (-1);
	}

	//get size of the file
	fseek(fp_read, 0L, SEEK_END);
	long filesize = ftell(fp_read);
	// return the file pointer to the start
	fseek(fp_read, 0L, SEEK_SET);

	// calculate the number of records in the buffer
	long records_per_buffer = a_mem / sizeof(Record);

	//calculate k
	long k;
	if (filesize % a_mem > 0){
		k = filesize/a_mem + 1;
	} else {
		k = filesize/a_mem;
	}

	//check if there is sufficient memory
	//a_mem/blocksize - 1 -> max number of input buffers we can have
	//k -> number of input buffers we need
	//a_mem/((k+1)*blocksize) -> max number of heapelements per input buffer
		//a_mem%((k+1)*blocksize) -> can be added to output buffer for better speed?
	long records_per_block = blocksize / sizeof(Record);
	int max_buffers = a_mem/blocksize - 1;

	if (max_buffers < k){
		printf ("Not enough memory\n");
		exit(1);
	}
	// calculate buffer capacities (by blocks)
	int input_buffer_capacity = a_mem/((k+1)*blocksize);
	int output_buffer_capacity = input_buffer_capacity + (a_mem%((k+1)*blocksize) / blocksize);

	//initialize a buffer of appropriate size
	Record * buffer = (Record *) calloc (records_per_buffer, sizeof (Record));
	
	/* Phase 1 of 2PMMS */

	FILE *fp_write;
	
	int i;
	int n;
	for (i = 0; i < k; i++){
		
		//read into buffer
		n = fread (buffer, sizeof(Record), records_per_buffer, fp_read);
		
		//sort
		qsort(buffer, n, sizeof(Record), compare);
		//print_buffer(buffer, n);
		//printf("\n");
	
		//open binary file for writing
		if (!(fp_write = fopen ( concat("run_", i) , "wb" ))) {
			printf ("Could not open file \"%s\" for writing\n", concat("run_", i));
			return (-1);
		}
		
		//write to disk
		fwrite (buffer, sizeof(Record), n, fp_write);

		fclose(fp_write);
		
	}
	

	/* Phase 2 of 2PMMS */
	
	MergeManager *manager;
	manager = (MergeManager *) malloc (sizeof (MergeManager));
	//printf("size of FILE: %lu\n", sizeof(FILE));
	
	/* initialize manager */
	// initialize capacities
	manager->heap_capacity = k;
	manager->current_heap_size = k;
	manager->output_buffer_capacity = output_buffer_capacity * records_per_block;
	manager->input_buffer_capacity = input_buffer_capacity * records_per_block;
	// initialize file names
	strcpy(manager->output_file_name, "merged_runs.dat");
	strcpy(manager->input_prefix, "run_");
	// allocate memory for arrays
	manager->heap = (HeapElement *) calloc (manager->heap_capacity, sizeof (HeapElement));
	manager->output_buffer = (Record *) calloc (manager->output_buffer_capacity, sizeof (Record));
	manager->input_buffers = calloc (manager->heap_capacity, sizeof (Record));
	for (i = 0; i < manager->heap_capacity; i++){
		manager->input_buffers[i] = (Record *) calloc (manager->input_buffer_capacity, sizeof (Record));
	}
	manager->current_input_file_positions = calloc (manager->heap_capacity, sizeof (int));
	manager->current_input_buffer_positions = calloc (manager->heap_capacity, sizeof (int));
	manager->total_input_buffer_elements = calloc (manager->heap_capacity, sizeof (int));
	// initialize positions
	manager->current_output_buffer_position = 0;
	for (i = 0; i < manager->heap_capacity; i++){
		manager->current_input_file_positions[i] = 0;
		manager->current_input_buffer_positions[i] = 0;
		manager->total_input_buffer_elements[i] = 0;
	}
		
	/* free all allocated memory */ //call cleanup in merge_external???
	free(buffer);

	//initialize all fields according to the input and the results of Phase I
	return merge_runs (manager);
}
