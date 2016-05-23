CC=g++
CCFLAGS=-std=c++0x
GDBFLAG=-g
TARGET=trie_tile
GDB_TARGET=gdb_trie_tile

all: $(TARGET) $(GDB_TARGET) 

$(TARGET): $(TARGET).cpp
	$(CC) $(CCFLAGS) -o $(TARGET) $(TARGET).cpp

$(GDB_TARGET): $(TARGET).cpp
	$(CC) $(CCFLAGS) $(GDBFLAG) -o $(GDB_TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET) $(GDB_TARGET)
