#include <stdio.h>

int main(void)
{
	int a = 0;
	int b = 2;
	int result = a + b;

	initialise_monitor_handles(); /* this allows you to use printf and read through the GDB interface */

	printf("The result is: %d\n", result);

	/* Never return as we have nowhere to go. */
	while (1) {
		asm("nop");
	}
}
