#pragma once

#include <ntddk.h>

namespace inject {
	void set_black_memory(void* black_mem);

	void alloc_shell_code(int size);

	void set_shell_code_size();

	void* get_shell_code_ptr();

}