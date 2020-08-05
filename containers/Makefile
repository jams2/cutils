LIBDIR = $$HOME/.local/lib
HEADERS = stack.h linked-list.h containers-common.h stack.h queue.h rbtree.h hashtable.h
SRC_FILES = hashtable.c stack.c linked-list.c queue.c rbtree.c
REQUIRES = $(HEADERS) $(SRC_FILES)
OBJ_FILES = hashtable.c stack.o linked-list.o queue.o rbtree.o
CHECK_ARGS = -lcheck -lm -lpthread -lrt

containers: $(REQUIRES) containers.h
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -shared -o $(LIBDIR)/libcontainers.so $(OBJ_FILES)
	cp containers.h $(LIBDIR)

configure: $(HEADERS)
	echo "#ifndef containers_h_" > containers.h
	echo "#define containers_h_" >> containers.h
	cat $(HEADERS) >> containers.h
	echo "#endif" >> containers.h

check: $(REQUIRES) containers.h
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -c -Wall -Werror check_containers.c
	gcc $(OBJ_FILES) check_containers.o $(CHECK_ARGS) -o check_containers
	./check_containers

memcheck: check_containers
	CK_FORK=no valgrind -s --leak-check=full ./check_containers

clean:
	rm *.o containers.h
