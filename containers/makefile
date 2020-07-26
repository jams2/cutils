LIBDIR = $$HOME/.local/lib
REQUIRES = linked-list.c queue.c containers.h
SRC_FILES = linked-list.c queue.c
OBJ_FILES = linked-list.o queue.o
CHECK_ARGS = -lcheck -lm -lpthread -lrt

containers: $(REQUIRES)
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -shared -o $(LIBDIR)/libcontainers.so $(OBJ_FILES)
	cp containers.h $(LIBDIR)

check: $(REQUIRES)
	gcc -c -Wall -Werror -fpic linked-list.c queue.c
	gcc -c -Wall -Werror check_containers.c
	gcc queue.o linked-list.o check_containers.o $(CHECK_ARGS) -o check_containers
	./check_containers

clean:
	rm *.o
