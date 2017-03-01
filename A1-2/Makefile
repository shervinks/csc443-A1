CC = gcc
CFLAGS = -O3 -Wall 
CFLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -g

# Source files
DISK_SORT_SRC= merge_external.c disk_sort.c 
PRINT_DAT_SRC= merge_external.c print_dat.c
DISTRIBUTION_SRC = merge_external.c distribution.c

# Binaries
all: disk_sort print_dat distribution
 
#sequential writing in blocks
disk_sort: $(DISK_SORT_SRC)
	$(CC) $(CFLAGS) $^ -o $@

print_Dat: $(PRINT_DAT_SRC)
	$(CC) $(CFLAGS) $^ -o $@

distribution: $(DISTRIBUTION_SRC)
	$(CC) $(CFLAGS) $^ -o $@
	
clean:  
	rm disk_sort run_* print_dat distribution
