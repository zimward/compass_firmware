#Variables
MCU	:= attiny85 #Attiny85
F_CPU 	:= 1000000UL
PROGRAMMER := avrisp
BAUD	:= 19200
PORT	:= /dev/ttyUSB0

LIBS 	:= libs
INCLUDE := $(addprefix $(LIBS)/,ATTinyCore/avr/variants/tinyX5 ATTinyCore/avr/cores/tiny ATTinyCore/avr/libraries/EEPROM TinyWireM)
SRCDIRS := $(addprefix $(LIBS)/,TinyWireM ATTinyCore/avr/libraries/EEPROM) \
	   src
CFLAGS 	:= -std=gnu++14 $(addprefix -I,$(INCLUDE)) -Os -Wall -Wextra -mmcu=$(MCU) -fdata-sections -ffunction-sections
LFLAGS 	:= -Wl,-gc-sections


CC	:= avr-gcc
CXX	:= avr-g++
OBJCOPY := avr-objcopy

SOURCES := $(shell find $(SRCDIRS) -name '*.c' -type f) \
	   $(shell find $(SRCDIRS) -name '*.cpp' -type f)

OBJECTS := $(shell basename -a $(SOURCES)) 
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)


# Compilation
 
.PHONY : all
all : compile
	$(CXX) -mmcu=$(MCU) $(LFLAGS) $(OBJECTS)
	$(OBJCOPY) -O ihex a.out a.hex
.PHONY : compile
compile :
	$(CXX) -c $(CFLAGS) -DF_CPU=$(F_CPU)  $(SOURCES)
	$(shell mv  "$(find $(SRCDIRS) -name '*.o' -type f)" ./) 
a.hex : all


.PHONY : clean
clean :
	rm *.o
	rm a.out
	rm a.hex

.PHONY : install
install : a.hex
	avrdude -p $(MCU) -P $(PORT) -c $(PROGRAMMER) -b $(BAUD) -U flash:w:a.hex
