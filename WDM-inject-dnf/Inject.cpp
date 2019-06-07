#include "Inject.h"

void* black_memory;

void set_black_memory(void* black_mem) {
	black_memory = black_mem;
}