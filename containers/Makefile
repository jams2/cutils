LIBDIR = $$HOME/.local/lib
HEADERS = stack.h linked-list.h containers-common.h stack.h queue.h rbtree.h hashtable.h siphash.h
SRC_FILES = hashtable.c stack.c linked-list.c queue.c rbtree.c siphash.c
REQUIRES = $(HEADERS) $(SRC_FILES)
OBJ_FILES = hashtable.o stack.o linked-list.o queue.o rbtree.o siphash.o
CHECK_ARGS = -lcheck -lm -lpthread -lrt

containers: $(REQUIRES) containers.h
	gcc -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -shared -o $(LIBDIR)/libcontainers.so $(OBJ_FILES)
	cp containers.h $(LIBDIR)

containers.h: $(HEADERS)
	echo "#ifndef containers_h_" > containers.h
	echo "#define containers_h_" >> containers.h
	cat $(HEADERS) >> containers.h
	echo "#endif" >> containers.h

check: containers containers.h
	gcc -g -c -Wall -Werror -fpic $(SRC_FILES)
	gcc -g -c -Wall -Werror check_containers.c
	gcc -g -c -Wall -Werror check_hashtable.c
	gcc -g $(OBJ_FILES) check_containers.o $(CHECK_ARGS) -o check_containers
	gcc -g $(OBJ_FILES) check_hashtable.o $(CHECK_ARGS) -o check_hashtable
	./check_containers
	./check_hashtable

memcheck: check
	CK_FORK=no valgrind -s --leak-check=full ./check_containers
	CK_FORK=no valgrind -s --leak-check=full ./check_hashtable

clean:
	rm *.o containers.h
