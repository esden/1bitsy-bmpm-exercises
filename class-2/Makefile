
CC=arm-none-eabi-gcc
CFLAGS=-g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=rdimon.specs -Ilibopencm3/include -DSTM32F4
LDFLAGS=-nostartfiles -Llibopencm3/lib -Wl,-T,1bitsy.ld -lopencm3_stm32f4

all: program-2.elf

%.elf: %.o
	${CC} $< ${LDFLAGS} -o $@

clean:
	rm -f *.o *.elf
