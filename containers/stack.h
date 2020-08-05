#ifndef stack_h_
#define stack_h_

typedef struct _intstack {
	int sp;
	int size;
	int *stack;
} IntStack;

void init_int_stack(IntStack *s, int size);
void free_int_stack(IntStack *s);
void push_int(IntStack *s, int val, int *err);
int pop_int(IntStack *s, int *err);

typedef struct _gstack {
	int sp;
	int size;
	int item_size;
	void **stack;
} Stack;

void init_stack(Stack *s, int initial_size, int item_size);
int resize_stack(Stack *s, int size);
void free_stack(Stack *s);
void push_generic(Stack *s, void *val, int *err);
void pop_generic(Stack *s, int *err);

#endif
