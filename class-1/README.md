# Class 1 - First Steps

Follow the instructions here to learn the basics of using Black Magic Probe and GDB.

Note: We assume you already have
[gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded) and some form of a
[GIT](https://git-scm.com/) client installed on your computer.

## How it all works.

We are starting with setting up our environment and uploading our first small
program. Follow the following few steps:

Clone [libopencm3](https://github.com/libopencm3/libopencm3) into a convenient
location.
```
git clone https://github.com/libopencm3/libopencm3.git
```

Compile the needed STM32F4 hardware library:
```
cd libopencm3
make lib TARGETS=stm32/f4 # this will limit the compilation to only the one library type we need
cd ..
```

Write an example program called excersize-1.c with your favorite text editor:
```
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
```

Note: Here is an assorted, short list, of a few selected text editors that you
can use. All of them have C syntax highlighting capability making code writing
easy:
* [gedit](https://wiki.gnome.org/Apps/Gedit)
* [Notepad++](https://notepad-plus-plus.org/)
* [Sublime Text](https://www.sublimetext.com/)
* [Atom](https://atom.io/)
* [Vim](http://www.vim.org/)
* [Emacs](https://www.gnu.org/software/emacs/)
* [Nano](https://www.nano-editor.org/)

Create a linker script file `1bitsy.ld` for the 1Bitsy dev platform:
```
/* Define memory regions. */
MEMORY
{
        rom (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
        ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Include the common ld script. */
INCLUDE libopencm3_stm32f4.ld
```

Note: this linker script tells the GCC linker how many resources there are
so that if your program becomes too large it can let you know.

To compile the example code execute the following command:
```
arm-none-eabi-gcc -g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
  --specs=rdimon.specs -nostartfiles -Llibopencm3/lib -lopencm3_stm32f4 \
  -Wl,-T,1bitsy.ld excersize-1.c -o excersize-1.elf
```

To load and debug the program connect your Black Magic Probe to the computer
using a micro USB cable. And then connect the 1Bitsy to your Black Magic Probe
using the gray ribbon cable. Make sure you plugged in the ribbon cable the right
way around into your 1Bitsy!

Make sure the Black Magic Probe has been detected:
```
lsusb
dmesg
```

Note: On Windows or Mac OS these commands don't exist, you have to use your
"Hardware Manager" and "About This Mac -> System Report... -> USB" respectively.

Start gdb:
```
arm-none-eabi-gdb excersize-1.elf
```

Connect GDB to the Black Magic Probe:
```
target extended-remote /dev/ttyACM0
monitor version
monitor help
```

Note: If this does not work make sure you are connecting to the correct serial
port. Unplug and plug in your BMP again checking the output of `dmesg` and
`lsusb`, if you still have problems ask the instructor.

The monitor command executes functions built into the Black Magic Probe. The
`monitor version` command should report the firmware and hardware version of
the Black Magic Probe you are using. The `monitor help` command lists the
available commands provided by the Black Magic Probe.

When you now run the `monitor jtag_scan` command, you should get an error
saying that the scan was not successful and that the target is not powered. You
can remedy that by either plugging in your 1Bitsy to your computer using a
second USB cable or by letting the Black Magic Probe power your target using
the `monitor tpwr enable` command.

Now when you run `monitor jtag_scan` you should see one available target
corresponding to the target CPU type.

You can now attach to the target cpu using the `attach 1` GDB command. As a
result you should see the GDB reporting that it has interrupted the cpu and the
address where it got halted at.

Now you can load your newly compiled binary running the `load` GDB command.

Starting the program using the `run` GDB command should result in the following
output:
```
run
Starting program: ${YOUR_WORK_PATH}/excersize-1.elf
The result is: 2
```

You can interrupt and inspect the program by pressing `<ctrl>-c` on your
keyboard.

Here are a few very basic executuion control commands:
* `run` will start or restart the program
* `start` will set a temporary breakpoint at the beginning of the `main`
function, then run the program and stop at the beginning of the `main`
function. It will also restart the program if it is already running.
* `continue` will resume the program at the point where it got interrupted.
* `exit` will quit GDB you can also do that by pressing the `<ctrl>-d` buttons
on your keyboard.

## Excersizes:

1. Restart the program using the `start` command. Try to see how variables are
being assigned and called using the `next` and `print <var>` GDB commands. If
you are lost and don't know where inside your program you are run the `list` or
`list <line number>` command.
2. While running the program try to inspect the environment variables using the
`info locals`.
3. Try modifying the program output using the `set variable <var>=<number>`
command.
4. Try setting your own breakpoint in the program using the
`break <line number>`, now when you restart or resume your program GDB will
show you a prompt as soon as the execution of your program reaches the line you
requested.
5. List the breakpoints you have with `info breakpoints`, you can disable them
or delete them using `disable breakpoints` or `delete breakpoints`. You can
also choose which one to disable or delete if you provide the breakpoint number
at the end of the commands. Try to set a breakpoint just before the `printf`
command is executed and see how you can quickly get to the place you want to
be.
