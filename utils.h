typedef struct record {
 int uid1;
 int uid2;
} Record;

/** Print out the context of buffer(record array). For testing purpose only **/
void print_buffer(Record r[]);

/** Print out the content of a binary file **/
int print_dat_file(char *filename);

/** print out each unique follower and how many people they are following, testing only **/
void print_followers(int followers[], int unique);

/** print out the max and average for followers **/
void print_max_avg(int followers[], int unique);

/** return index of follower in a list of followers, return -1 if it does not exist **/
int exists(int followers[], int unique, int follower);
