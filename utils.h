typedef struct record {
 int uid1;
 int uid2;
} Record;

/** Print out the context of buffer(record array). For testing purpose only **/
void print_buffer(Record r[]);

/** Print out the content of a binary file **/
int print_dat_file(char *filename);