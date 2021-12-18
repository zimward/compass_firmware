#Variables
MCU	:= attiny85 
F_CPU 	:= 1000000UL #1MHz
PROGRAMMER := avrisp
BAUD	:= 19200
PORT	:= /dev/ttyUSB0

LIBS 	:= libs
INCLUDE := $(addprefix $(LIBS)/,ATTinyCore/avr/variants/tinyX5 ATTinyCore/avr/cores/tiny ATTinyCore/avr/libraries/EEPROM TinyWireM)
SRCDIRS := $(addprefix $(LIBS)/,TinyWireM ATTinyCore/avr/libraries/EEPROM) \
	   src

CFLAGS 	:= -std=gnu++14 $(addprefix -I,$(INCLUDE)) -Os -Wall -Wextra -mmcu=$(MCU) -fdata-sections -ffunction-sections
LFLAGS 	:= -Wl,-gc-sections

#Format Flags
FFLAGS  := -i

CC	:= avr-gcc
CXX	:= avr-g++
OBJCOPY := avr-objcopy
FORMAT  := clang-format


SOURCES := $(shell find $(SRCDIRS) -type f \( -name '*.c' -o -name '*.cpp' \))

OBJECTS := $(shell basename -a $(SOURCES)) 
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)


# Compilation
.PHONY : install, all, clean, format

all : compile
	$(CXX) -mmcu=$(MCU) $(LFLAGS) $(OBJECTS)
	$(OBJCOPY) -O ihex a.out a.hex

compile :
	$(CXX) -c $(CFLAGS) -DF_CPU=$(F_CPU)  $(SOURCES)

a.hex : all

clean :
	rm *.o a.out a.hex

install : a.hex
	avrdude -p $(MCU) -P $(PORT) -c $(PROGRAMMER) -b $(BAUD) -U flash:w:a.hex

format  : .clang-format
	$(FORMAT) -style=file $(FFLAGS) $(shell find src/ -type f \( -name '*.c' -o -name '*.h' \)) 
