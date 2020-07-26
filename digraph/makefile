LIBDIR = $$HOME/.local/lib/
L_ARGS = -Wl,-rpath=$(LIBDIR)
INCLUDE = -I$(LIBDIR)
CHECK_ARGS = -lcheck -lm -lpthread -lrt $(L_ARGS) -L$(LIBDIR) -lcontainers

digraph: digraph.c digraph.h
	gcc -c -Wall -Werror $(L_ARGS) $(INCLUDE) -fpic digraph.c
	gcc -shared -o $(LIBDIR)/libdigraph.so digraph.o
	cp digraph.h $(LIBDIR)

check: check_digraph.c digraph.c digraph.h
	gcc -c -Wall -Werror -fpic $(L_ARGS) $(INCLUDE) digraph.c
	gcc -c -Wall -Werror $(L_ARGS) $(INCLUDE) check_digraph.c
	gcc digraph.o check_digraph.o $(CHECK_ARGS) -o check_digraph
	./check_digraph

memcheck: check_digraph
	CK_FORK=no valgrind -s --leak-check=full ./check_digraph

clean:
	rm check_digraph check_digraph.o digraph.o
