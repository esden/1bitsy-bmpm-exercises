#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ticks)
{
	int i;

	for (;ticks!=0; ticks--) {
		for (i=0; i<10000; i++) {
			asm("nop");
		}
	}
}

int main(void)
{
	int blink_count = 0;

	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);

	gpio_set(GPIOA, GPIO8);

	while(1) {
		blink_count += 1;
		if ((blink_count ^ 4) == 0) {
			delay(100);
		}
		delay(10);
		gpio_toggle(GPIOA, GPIO8);
	}

	return 0;
}
