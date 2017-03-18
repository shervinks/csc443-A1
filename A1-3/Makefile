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
FIND_TRUEFRIENDS= find_truefriends.c 

# Binaries
all: find_truefriends
 
#sequential writing in blocks
find_truefriends: $(FIND_TRUEFRIENDS)
	$(CC) $(CFLAGS) $^ -o $@
	
clean:  
	rm find_truefriends
