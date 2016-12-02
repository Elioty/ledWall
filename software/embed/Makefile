CC=avr-g++
SIZE=avr-size
OBJCOPY=avr-objcopy

MMCU=atmega2560
FREQ=16000000UL
DEV=/dev/ttyACM0

CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -Os -mmcu=$(MMCU) -DF_CPU=$(FREQ)
LDFLAGS=-mmcu=$(MMCU) -Os

OBJS=main.o drivers/led_driver.o animations/wave.o common.o
EXEC=leds.elf

.PHONY: flash

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

drivers/led_driver.o: drivers/led_driver.hpp \
                      common.hpp \
                      colors.hpp \
                      drivers/drivers.hpp
animations/wave.o: animations/wave.hpp \
                   animation.hpp \
                   colors.hpp \
                   common.hpp \
                   drivers/led_driver.hpp
main.o: common.hpp \
        drivers/drivers.hpp \
        drivers/led_driver.hpp \
        animation.hpp \
        animations/wave.hpp \
        colors.hpp

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(EXEC:.elf=.hex): $(EXEC)
	$(OBJCOPY) -O ihex $^ $@

flash: $(EXEC:.elf=.hex)
	avrdude -cwiring -p$(MMCU) -P $(DEV) -U flash:w:$< -D

clean:
	rm -rf -- $(OBJS)

mrproper:
	rm -rf -- $(OBJS) $(EXEC) $(EXEC:.elf=.hex)
