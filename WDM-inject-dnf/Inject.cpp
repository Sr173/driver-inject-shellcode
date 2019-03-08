#include "Inject.h"

namespace inject {
	void* black_memory = nullptr;
	void* shell_code = nullptr;
	int shell_code_size;
	int alloc_shell_code_size;

	void set_black_memory(void* black_mem) {
		black_memory = black_mem;
	}

	void alloc_shell_code(int size) {
		if (shell_code) {
			ExFreePool(shell_code);
		}
		alloc_shell_code_size = size;
	}

	void set_shell_code_size() {

	}

	void* get_shell_code_ptr() {
		
	}
}