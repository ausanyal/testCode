CC=g++
CCFLAGS=-std=c++0x
GDBFLAG=-g
TARGET=trie_tile
GDB_TARGET=gdb_trie_tile

all: $(TARGET) $(GDB_TARGET) 

$(TARGET): $(TARGET).cc
	$(CC) $(CCFLAGS) -o $(TARGET) $(TARGET).cc

$(GDB_TARGET): $(TARGET).cc
	$(CC) $(CCFLAGS) $(GDBFLAG) -o $(GDB_TARGET) $(TARGET).cc

clean:
	$(RM) $(TARGET) $(GDB_TARGET)
