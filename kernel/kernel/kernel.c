#include <stdio.h>
#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("hey gangsters... %s %d\n", "pop me in der real quick", 2025);
}
