PRG            = random
OBJ            = random.o
MCU_TARGET     = attiny45
AVRDUDE_TARGET = t45
OPTIMIZE       = -Os
PROG	       = dragon_isp
PORT	       = usb

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -L/usr/x86_64-pc-linux-gnu/avr/lib

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: hex

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf $(PRG).hex

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

install: load

load: $(PRG).hex
	avrdude -p $(AVRDUDE_TARGET) -c $(PROG) -P $(PORT) -U flash:w:$< 

