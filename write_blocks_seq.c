#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("write_blocks_seq takes 2 arguments: write_blocks_seq <input filename> <block size>\n");
        return -1;
    }
	
	Record * r = (Record *)  malloc(sizeof(Record));
    r->uid1 = 20;
    r->uid2 = 10;
	
	printf("uid1: %d, uid2: %d\n", r->uid1, r->uid2);
	
	return 0;
}