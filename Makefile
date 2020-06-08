CC:=g++
TARGETS=project

all: $(TARGETS)	

project: project.cpp 
	$(CC) -o project $^

clean:
	rm $(TARGETS)
