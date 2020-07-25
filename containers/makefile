LIBDIR = $$HOME/.local/lib
REQUIRES = containers.h linked-list.c linked-list.h queue.c queue.h
SRC_FILES = linked-list.c queue.c
OBJ_FILES = linked-list.o queue.o

containers: $(REQUIRES)
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -shared -o $(LIBDIR)/libcontainers.so $(OBJ_FILES)
	cp containers.h $(LIBDIR)
