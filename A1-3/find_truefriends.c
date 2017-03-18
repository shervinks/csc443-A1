#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

typedef struct record {
 int uid1;
 int uid2;
} Record;

// Return 1 if r is in the array records else return 0
int binary_search(Record * records, Record r, int size) {
    int lowerBound = 0;
    int upperBound = size -1;
    int midPoint = -1;

    while(lowerBound <= upperBound) {
        // compute the mid point
        // midPoint = (lowerBound + upperBound) / 2;
        midPoint = lowerBound + (upperBound - lowerBound) / 2;

        // data found
        if(records[midPoint].uid1 == r.uid1 && records[midPoint].uid2 == r.uid2) {
            return 1;
        } else {
            // if data is larger
            if(records[midPoint].uid1 < r.uid1) {
                // data is in upper half
                lowerBound = midPoint + 1;
            }
            else if (records[midPoint].uid1 == r.uid1 && (records[midPoint].uid2 < r.uid2)){
                lowerBound = midPoint + 1;
            }
            // data is smaller
            else {
                // data is in lower half
                upperBound = midPoint -1;
            }
        }
    }

    return 0;
}

int compare (const void *a, const void *b){
	int a_f = ((const struct record*)a)->uid1;
	int b_f = ((const struct record*)b)->uid1;
    if (a_f == b_f) {
        int a_2 = ((const struct record*)a)->uid2;
        int b_2 = ((const struct record*)b)->uid2;
        return (a_2 - b_2);
    }
    return (a_f - b_f);
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("find_truefriends takes 2 arguments: find_truefriends <input filename> <block size>\n");
        return -1;
    }
    int block_size = atoi(argv[2]);
    // check if block_size is a multiple of sizeof(record)
    if (block_size % sizeof(Record) != 0){
        printf ("Block size is not a multiple of record size\n");
        return (-1);
    }
    /* open dat file for reading */
	FILE *fp_read_R; // R
    FILE *fp_read_S; // S
    if (!(fp_read_R= fopen ( argv[1] , "rb" ))) {
        printf ("Could not open R for reading \n");
        return (-1);
    }
    if (!(fp_read_S= fopen ( argv[1] , "rb" ))) {
        printf ("Could not open S for reading \n");
        return (-1);
    }

    int memory = 200 * 1024 * 1024; // memory contraint
    int m = memory / block_size; // number of blocks we can hold at any given time
    int chunk_size = (m - 1) * block_size;
    // calculate the number of chunks of R needed to read the whole file
	fseek(fp_read_R, 0L, SEEK_END);
	long filesize = ftell(fp_read_R);
    // return the file pointer to the start
	fseek(fp_read_R, 0L, SEEK_SET);

    int number_of_blocks = filesize / block_size;
	if (filesize % block_size != 0){
		number_of_blocks++;
	}
    int chunk_num = filesize / chunk_size;
    if (filesize % chunk_size != 0){
		chunk_num++;
	}
    Record * buffer_R = (Record *) calloc (chunk_size / sizeof (Record), sizeof (Record));
    Record * buffer_S = (Record *) calloc (block_size / sizeof (Record), sizeof (Record));
    // use BNLJ algorithm
    int count = 0;
    int uid1, uid2;
    for (int i=0; i < chunk_num; i++) {
        int read_R = fread (buffer_R, sizeof(Record), chunk_size / sizeof (Record), fp_read_R);
        qsort(buffer_R, read_R, sizeof(Record), compare); // Sort R
        fseek(fp_read_S, 0L, SEEK_SET);
        for (int j=0; j<number_of_blocks; j++) {
            int read_S = fread (buffer_S, sizeof(Record), block_size / sizeof (Record), fp_read_S);
            for (int k = 0; k<read_S; k++) {
                uid1 = buffer_S[k].uid1;
                uid2 = buffer_S[k].uid2;
                Record r;
                r.uid1 = uid2;
                r.uid2 = uid1;
                if (uid1 < uid2 && binary_search(buffer_R, r, read_R)) {
                    count++;
                    printf("%d, %d\n", uid1, uid2);
                }
            }
        }
    }
    printf("The number of true friend pairs is %d\n", count);
    return 0;

}
