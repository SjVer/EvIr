#include "evir/.common.hpp"

/// shared library ctor (constructor) function
void __attribute__((constructor)) __evir_init()
{
	#ifdef DEBUG
	printf(DEBUG_MARKER " " LIB_NAME_INTERNAL " initialized (ctor).\n");
	#endif
}

/// shared library dtor (destructor) function
void __attribute__((destructor)) __evir_deinit()
{
	#ifdef DEBUG
	printf(DEBUG_MARKER " " LIB_NAME_INTERNAL " deinitialized (dtor).\n");
	#endif
}