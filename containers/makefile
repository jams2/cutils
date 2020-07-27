LIBDIR = $$HOME/.local/lib
REQUIRES = stack.c linked-list.c queue.c containers.h
SRC_FILES = stack.c linked-list.c queue.c
OBJ_FILES = stack.o linked-list.o queue.o
CHECK_ARGS = -lcheck -lm -lpthread -lrt

containers: $(REQUIRES)
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -shared -o $(LIBDIR)/libcontainers.so $(OBJ_FILES)
	cp containers.h $(LIBDIR)

check: $(REQUIRES)
	gcc -c -Wall -Werror -fpic linked-list.c queue.c
	gcc -c -Wall -Werror check_containers.c
	gcc $(OBJ_FILES) check_containers.o $(CHECK_ARGS) -o check_containers
	./check_containers

memcheck: check_containers
	CK_FORK=no valgrind -s --leak-check=full ./check_containers

clean:
	rm *.o
