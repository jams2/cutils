LIBDIR = $$HOME/.local/lib

digraph: digraph.c digraph.h
	gcc -c -Wall -Werror -fpic digraph.c
	gcc -shared -o $(LIBDIR)/libdigraph.so digraph.o
	cp digraph.h $(LIBDIR)

check: check_digraph.c digraph.c digraph.h
	gcc -c -Wall -Werror -fpic digraph.c
	gcc -c -Wall -Werror check_digraph.c
	gcc digraph.o check_digraph.o -lcheck -lm -lpthread -lrt -o check_digraph_tests
	./check_digraph_tests

clean:
	rm digraph.o
	rm check_digraph_tests check_digraph.o digraph.o
