# Class 2 - Hardware and Assembly

Based on the previous class we will be expanding and instead of reproducing a
native `printf` program we will be manipulating Memory Mapped Input and Output
(MMIO), and starting to interact with the physical world.

## Preparations

If you created a new directory for this class make sure to copy over the
`libopencm3`:
```
cp -a ../class-1/libopencm3 .
```

and the linker script:
```
cp ../class-1/1bitsy.ld .
```

Now let us create a new program that blinks a led instead:
```
include <libopencm3/stm32/rcc.h>
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
```

You can compile the program with a similar command as in the previous class:
```
arm-none-eabi-gcc -g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
  --specs=rdimon.specs -nostartfiles -Llibopencm3/lib \
  -Wl,-T,1bitsy.ld program-2.c -lopencm3_stm32f4 -o program-2.elf
```

## Optional - Create a Makefile

If you have GNU Make installed on your computer you can write a small `Makefile`
to recompile your code without having to remember the whole `gcc` command.

Just create a file named `Makefile` with the following content:
```
CC=arm-none-eabi-gcc
CFLAGS=-g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=rdimon.specs -Ilibopencm3/include -DSTM32F4
LDFLAGS=-nostartfiles -Llibopencm3/lib -Wl,-T,1bitsy.ld -lopencm3_stm32f4

all: program-2.elf

%.elf: %.o
        ${CC} $< ${LDFLAGS} -o $@

clean:
        rm -f *.o *.elf
```

Warning: make sure to have a <TAB> at the beginning of line 8 and 11!

Now you can recompile your program running the `make` command in your terminal.
Which should result in the following output:
```
arm-none-eabi-gcc -g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=rdimon.specs -Ilibopencm3/include -DSTM32F4   -c -o program-2.o program-2.c
arm-none-eabi-gcc program-2.o -nostartfiles -Llibopencm3/lib -Wl,-T,1bitsy.ld -lopencm3_stm32f4 -o program-2.elf
rm program-2.o
```

## Load and start the new program

Launch GDB:
```
arm-none-eabi-gdb program-2.elf
```
and repeat the same commands as in the last class:
```
target extended-remote /dev/ttyACM0
monitor tpwr enable
monitor jtag_scan
attach 1
load
start
```

You should be presented with the GDB command line and your application halted
at the beginning of the `main` function.

## Optional - Automating GDB launch

So that you don't have to always type the same things in GDB you can create a
`.gdbinit` file either in your home directory or in your local directory. GDB
will look for a file of this name and execute all the commands in there before
you are presented with a prompt.

A typical GDB Init file looks like this. The following example file has a few
more options set making day to day use more convenient.
```
set history file ~/.gdb_history
set history save on
set target-async on
set confirm off
set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
monitor version
monitor tpwr enable
monitor jtag_scan
attach 1
```

## Exercises

1. Step through the code and see how you can either step over functions using
the `next` command or step into a function using the `step` command.
2. See how you can leave a function by using the `finish` command.
3. Set a watchpoint on the `blink_count` variable using the `watch blink_count`
command. Then let the program run using the `continue` command and see how it
acts similar to a breakpoint that interrupts the code execution every time the
variable value changes.
4. See how you can make complex expressions using the `print <expression>`
command. For example `print blink_count ^ 4`, and then format your output
`printf "my formatting %d", blink_count ^ 4`.
5. Now combine the two previous exercises with the `commands` feature. You can
watch a variable and print the condition output by writing the following
sequence of commands:
```
watch blink_count
commands
printf "blink extend condition %d\n", blink_count ^ 4
end
```
6. Try using all you learned so far to find out why the led blink delay
extension triggers only once instead of every 4th cycles. After finding the
solution edit the program source code in a separate window without exiting GDB.
If you did the optional step of creating the makefile you can run the `make`
command from within GDB followed by `load` and `run` to see if your fix was
successful.
