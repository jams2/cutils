#include <stdlib.h>
#include "containers.h"


void init_int_stack(IntStack *s, int size)
{
	s->sp = 0;
	s->size = size;
	s->stack = calloc(size, sizeof(int));
}

void free_int_stack(IntStack *s)
{
	free(s->stack);
	free(s);
}

void push_int(IntStack *s, int val, int *err)
{
	if (s->sp == s->size) {
		*err = STACK_OVERFLOW;
		return;
	}
	s->stack[s->sp++] = val;
}

int pop_int(IntStack *s, int *err)
{
	if (s->sp == 0) {
		*err = STACK_UNDERFLOW;
		return -1;
	}
	return s->stack[--s->sp];
}
