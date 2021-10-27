CC=gcc
INCLUDES=../
CFLAGS=-c -Wall
LDFLAGS= -I $(INCLUDES)
SOURCES=ioctl.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f ./*.o; rm ./$(EXECUTABLE)
