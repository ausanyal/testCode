CC=g++
CCFLAGS=-std=c++0x
GDBFLAG=-g
TARGET=lid
GDB_TARGET=gdb_lid

all: $(TARGET) $(GDB_TARGET) 

$(TARGET): $(TARGET).cc
	$(CC) $(CCFLAGS) -o $(TARGET) $(TARGET).cc

$(GDB_TARGET): $(TARGET).cc
	$(CC) $(CCFLAGS) $(GDBFLAG) -o $(GDB_TARGET) $(TARGET).cc

clean:
	$(RM) $(TARGET) $(GDB_TARGET)
