CC := g++
TARGETS = cadb0093

all: $(TARGETS)	

project: cadb0093.cpp 
	$(CC) -o cadb0093 $^

clean:
	rm $(TARGETS)
	rm *.csv
