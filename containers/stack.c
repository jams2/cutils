#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "containers-common.h"


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

void init_stack(Stack *s, int initial_size, int item_size)
{
	s->sp = 0;
	s->size = initial_size;
	s->item_size = item_size;
	s->stack = calloc(initial_size, item_size);
}

void free_stack(Stack *s)
{
	for (int i = 0; i < s->size; ++i)
		free(s->stack[i]);
	free(s->stack);
	free(s);
}

void push_generic(Stack *s, void *val, int *err)
{
	if (s->sp == s->size)
		resize_stack(s, 2 * s->size);
	memcpy(s->stack[s->sp++], val, s->item_size);
}

int resize_stack(Stack *s, int size)
{
	s->size = size;
	s->stack = reallocarray(s->stack, size, s->item_size);
	return s->stack == NULL ? REALLOC_ERR : 0;
}

void pop_generic(Stack *s, int *err)
{}
