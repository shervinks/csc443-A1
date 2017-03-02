CC = gcc
CFLAGS = -O3 -Wall 
CFLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -g
CFLAGS += -std=c99

# Source files
DISK_SORT_SRC= merge_external.c disk_sort.c 
DISTRIBUTION_SRC = merge_external.c distribution.c

# Binaries
all: disk_sort distribution
 
#sequential writing in blocks
disk_sort: $(DISK_SORT_SRC)
	$(CC) $(CFLAGS) $^ -o $@

distribution: $(DISTRIBUTION_SRC)
	$(CC) $(CFLAGS) $^ -o $@
	
clean:  
	rm disk_sort run_* distribution
