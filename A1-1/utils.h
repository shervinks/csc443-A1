typedef struct record {
 int uid1;
 int uid2;
} Record;

/** Print out the context of buffer(record array). For testing purpose only **/
void print_buffer(Record r[], int size);

/** Print out the content of a binary file **/
int print_dat_file(char *filename);

/** calculate the average and max for the given list of records **/
void get_calculation(Record * buffer, int size, float calculations[], int index, int start_pos);

/** calculate and print the overall max and average **/
void print_calculations(float calculations[], int size);
