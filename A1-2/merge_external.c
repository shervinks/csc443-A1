#include "merge.h"

//manager fields should be already initialized in the caller
int merge_runs (MergeManager * merger){	
	int  result; //stores SUCCESS/FAILURE returned at the end	
	
	//1. go in the loop through all input files and fill-in initial buffers
	if (init_merge (merger)!=SUCCESS)
		return FAILURE;
	

	while (merger->current_heap_size > 0) { //heap is not empty
		HeapElement smallest;
		Record next; //here next comes from input buffer
		
		if(get_top_heap_element (merger, &smallest)!=SUCCESS)
			return FAILURE;
		result = get_next_input_element (merger, smallest.run_id, &next);

		if (result==FAILURE)
			return FAILURE;

		if(result==SUCCESS) {//next element exists, may also return EMPTY
			if(insert_into_heap (merger, smallest.run_id, &next)!=SUCCESS)
				return FAILURE;	
		}		

		merger->output_buffer [merger->current_output_buffer_position].UID1=smallest.UID1;
		merger->output_buffer [merger->current_output_buffer_position].UID2=smallest.UID2;
		
		merger->current_output_buffer_position++;
        //staying on the last slot of the output buffer - next will cause overflow
		if(merger->current_output_buffer_position == merger-> output_buffer_capacity ) {
			if(flush_output_buffer(merger)!=SUCCESS) {
				return FAILURE;		
				merger->current_output_buffer_position=0;
			}	

		}
	
	}


	//flush what remains in output buffer
	if(merger->current_output_buffer_position > 0) {
		if(flush_output_buffer(merger)!=SUCCESS)
			return FAILURE;
	}
	
	clean_up(merger);
	return SUCCESS;	
}


int get_top_heap_element (MergeManager * merger, HeapElement * result){
	HeapElement item;
	int child, parent;

	if(merger->current_heap_size == 0){
		printf( "UNEXPECTED ERROR: popping top element from an empty heap\n");
		return FAILURE;
	}

	*result=merger->heap[0];  //to be returned

	//now we need to reorganize heap - keep the smallest on top
	item = merger->heap [--merger->current_heap_size]; // to be reinserted 


	parent =0;
	while ((child = (2 * parent) + 1) < merger->current_heap_size) {
		// if there are two children, compare them 
		if (child + 1 < merger->current_heap_size && 
				(compare_heap_elements(&(merger->heap[child]),&(merger->heap[child + 1]))>0)) 
			++child;
		
		// compare item with the larger 
		if (compare_heap_elements(&item, &(merger->heap[child]))>0) {
			merger->heap[parent] = merger->heap[child];
			parent = child;
		} 
		else 
			break;
	}
	merger->heap[parent] = item;
	
	return SUCCESS;
}

int insert_into_heap (MergeManager * merger, int run_id, Record *input){

	HeapElement new_heap_element;
	int child, parent;

	new_heap_element.UID1 = input->UID1;
	new_heap_element.UID2 = input->UID2;
	new_heap_element.run_id = run_id;
	
	if (merger->current_heap_size == merger->heap_capacity) {
		printf( "Unexpected ERROR: heap is full\n");
		return FAILURE;
	}
  	
	child = merger->current_heap_size++; /* the next available slot in the heap */
	
	while (child > 0) {
		parent = (child - 1) / 2;
		if (compare_heap_elements(&(merger->heap[parent]),&new_heap_element)>0) {
			merger->heap[child] = merger->heap[parent];
			child = parent;
		} 
		else 
			break;
	}
	merger->heap[child]= new_heap_element;	
	return SUCCESS;
}


/*
** TO IMPLEMENT
*/

int init_merge (MergeManager * manager) {
	/* read in from all the runs */
	int i;
	int n;
	for (i = 0; i < manager->heap_capacity; i++){
		/* open dat file for reading */
		char *str = concat(manager->input_prefix, i);
		if (!(manager->inputFP= fopen (str , "rb" ))) {
			printf ("Could not open run file \"%s\" for reading \n", str);
			return FAILURE;
		}

		/* read allowed number of blocks from runs into buffers */
		n = fread (manager->input_buffers[i], sizeof(Record), 
			manager->input_buffer_capacity, manager->inputFP);

		// check if file is empty
		if (n == 0){
			printf ("File \"%s\" could not be read, beacuse it was empty", str);
		}
		// update current number of elements in this buffer
		manager->total_input_buffer_elements[i] = n;
		// update file position
		if (n == manager->input_buffer_capacity){
			manager->current_input_file_positions[i] += sizeof(Record) * manager->input_buffer_capacity;
		} else {
			manager->current_input_file_positions[i] = -1;
		}
		fclose(manager->inputFP);
	}

	/* initialize heap */
	int buffer_position;
	for (i = 0; i < manager->heap_capacity; i++){
		buffer_position = manager->current_input_buffer_positions[i];
		manager->heap[i].UID1 = manager->input_buffers[i][buffer_position].UID1;
		manager->heap[i].UID2 = manager->input_buffers[i][buffer_position].UID2;
		manager->heap[i].run_id = i;
	}
	// sort heap
	qsort(manager->heap, manager->heap_capacity, sizeof(HeapElement), compare_heap_elements);

	/* open binary file for writing */
	if (!(manager->outputFP = fopen ( manager->output_file_name , "wb" ))) {
		printf ("Could not open file \"%s\" for writing\n", manager->output_file_name);
		return FAILURE;
	}
	
	return SUCCESS;
}

int flush_output_buffer (MergeManager * manager) {
	/* write to disk */
	fwrite (manager->output_buffer, sizeof(Record), 
		manager->current_output_buffer_position, manager->outputFP);
	fflush (manager->outputFP);
	/* adjust buffer output position */
	manager->current_output_buffer_position = 0;
	return SUCCESS;
}

int get_next_input_element(MergeManager * manager, int file_number, Record *result) {
	int res;
	/* check if all elements are processed */
	if (manager->current_input_buffer_positions[file_number] == 
		manager->total_input_buffer_elements[file_number]-1){
		res = refill_buffer(manager, file_number);
		if (res == EMPTY){
			return EMPTY;
		}
		//result = &manager->input_buffers[file_number][0];
		result->UID1 = manager->input_buffers[file_number][0].UID1;
		result->UID2 = manager->input_buffers[file_number][0].UID2;
	} else {
		/* get next element */
		manager->current_input_buffer_positions[file_number]++;
		int buffer_index = manager->current_input_buffer_positions[file_number];

		result->UID1 = manager->input_buffers[file_number][buffer_index].UID1;
		result->UID2 = manager->input_buffers[file_number][buffer_index].UID2;
	}
	return SUCCESS;
}

int refill_buffer (MergeManager * manager, int file_number) {
	// check if run is complete
	if (manager->current_input_file_positions[file_number] == -1){
		return EMPTY;
	}
	//printf("file_num: %d\n", file_number);
	int n;
	/* open dat file for reading */
	char *str = concat(manager->input_prefix, file_number);
	if (!(manager->inputFP= fopen (str , "rb" ))) {
		printf ("Could not open run file \"%s\" for reading \n", str);
		return FAILURE;
	}
	/* get file size */
	fseek(manager->inputFP, 0L, SEEK_END);
	long filesize = ftell(manager->inputFP);

	// seek to the correct position
	fseek(manager->inputFP, manager->current_input_file_positions[file_number], SEEK_SET);

	/* read allowed number of blocks from runs into buffer */
	n = fread (manager->input_buffers[file_number], sizeof(Record), 
		manager->input_buffer_capacity, manager->inputFP);
	fclose(manager->inputFP);
	
	// check if empty
	if (n == 0){
		return EMPTY;
	}
	
	// update current number of elements in this buffer
	manager->total_input_buffer_elements[file_number] = n;
	/* update file position */
	manager->current_input_file_positions[file_number] += sizeof(Record) * n;
	if (manager->current_input_file_positions[file_number] >= filesize){
		manager->current_input_file_positions[file_number] = -1;
	}
	/* update buffer position */
	manager->current_input_buffer_positions[file_number] = 0;

	return SUCCESS;
}

void clean_up (MergeManager * manager) {

	/* free all allcoated memory */
	free(manager->heap);
	free(manager->output_buffer);
	int i;
	for (i = 0; i < manager->heap_capacity; i++){
		free(manager->input_buffers[i]);
	}
	free(manager->input_buffers);
	free(manager->current_input_file_positions);
	free(manager->current_input_buffer_positions);
	free(manager->total_input_buffer_elements);
	
	// close file for writing
	fclose(manager->outputFP);
}

int compare_heap_elements (const void *a, const void *b) {
	int a_f = ((const struct HeapElement*)a)->UID2;
	int b_f = ((const struct HeapElement*)b)->UID2;
	return (a_f - b_f);
}

char* concat(char *s1, int s2){
	char str[MAX_PATH_LENGTH];
	snprintf(str, 32, "%d", s2);
	//str = (char*)(s2);
	char *result = malloc(strlen(s1)+strlen(str)+1);
	strcpy(result, s1);
	strcat(result, str);
	strcat(result, ".dat");
	return result;
}

/** Print out the context of buffer. For testing purpose only **/
void print_buffer(Record r[], int size) {
	int i = 0;
	while(i < size) {
		printf("First: %d, Second: %d\n", r[i].UID1, r[i].UID2);
		i++;
	}
}

/** Print out the context of buffer. For testing purpose only **/
void print_heap(HeapElement r[], int size) {
	int i = 0;
	while(i < size) {
		printf("First: %d, Second: %d\n", r[i].UID1, r[i].UID2);
		i++;
	}
}

void print_all_buffers(MergeManager * manager){
	for(int i = 0; i < manager->heap_capacity; i++){
		printf("Buffer number: %d\n", i);
		print_buffer(manager->input_buffers[i], manager->input_buffer_capacity);
		
	}
}
