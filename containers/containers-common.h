#ifndef containers_common_h_
#define containers_common_h_

enum container_err {
	LL_OK,
	LL_TYPE_MISMATCH,
	LL_MALLOC_ERR,
	LL_REMOVE_EMPTY,
	LL_OUT_OF_BOUNDS,
	LL_INSERT_DUPLICATE,
	STACK_OVERFLOW,
	STACK_UNDERFLOW,
	REALLOC_ERR,
};

#endif
