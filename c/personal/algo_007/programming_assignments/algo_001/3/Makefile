# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

GLIB_CFLAGS = -D_REENTRANT -I/usr/local/Cellar/glib/2.42.1/include/glib-2.0 -I/usr/local/Cellar/glib/2.42.1/lib/glib-2.0/include -I/usr/local/opt/gettext/include 
GLIB_LIBS = -L/usr/local/Cellar/glib/2.42.1/lib -L/usr/local/opt/gettext/lib -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl 

# the build target executable:
TARGET = randomized_cntrct

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(GLIB_CFLAGS) $(GLIB_LIBS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)


