# Arduino makefile
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# The Arduino environment does preliminary processing on a sketch before
# compiling it.  If you're using this makefile instead, you'll need to do
# a few things differently:
#
#   - Give your program's file a .cpp extension (e.g. foo.cpp).
#
#   - Put this line at top of your code: #include <WProgram.h>
#
#   - Write prototypes for all your functions (or define them before you
#     call them).  A prototype declares the types of parameters a
#     function will take and what type of value it will return.  This
#     means that you can have a call to a function before the definition
#     of the function.  A function prototype looks like the first line of
#     the function, with a semi-colon at the end.  For example:
#     int digitalRead(int pin);
#
# Instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch.
#
#  2. Below, modify the line containing "TARGET" to refer to the name of
#     of your program's file without an extension (e.g. TARGET = foo).
#
#  3. Modify the line containg "ARDUINO" to point the directory that
#     contains the Arduino core (for normal Arduino installations, this
#     is the lib/targets/arduino sub-directory).
#
#  4. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.USB*).
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make upload", reset your Arduino board, and press enter  to
#     upload your program to the Arduino board.
#
# $Id$

#PORT = COM3
PORT =  /dev/ttyS3
TARGET = AvrKeyboardToy
ARDUINO = ./arduino
SRC = $(ARDUINO)/wiring.c $(ARDUINO)/wiring_digital.c $(ARDUINO)/wiring_analog.c \
	$(ARDUINO)/hooks.c
CXXSRC = $(TARGET).cpp main.cpp TinyBasicPlus.cpp KeyboardUtil.cpp DisplayBuffer.cpp ColorUtils.cpp \
	Adafruit_GFX.cpp Adafruit_ST7735.cpp \
	PS2KeyAdvanced.cpp \
	$(ARDUINO)/abi.cpp \
	$(ARDUINO)/WMath.cpp $(ARDUINO)/Print.cpp $(ARDUINO)/Tone.cpp \
	$(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/HardwareSerial0.cpp \
	$(ARDUINO)/SPI.cpp 
MCU = atmega328p
F_CPU = 8000000L
# Set below in board targets
#BOARD_TYPE = AVR_KEYBOARD_TOY_RELEASE
#BOARD_TYPE = AVR_KEYBOARD_TOY_TEST
FORMAT = ihex
UPLOAD_RATE = 57600

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
# Using dwarf-2 here so that I can remote-debug using simavr and avr-gdb
DEBUG = dwarf-2

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU) -DARDUINO=10608 -DARDUINO_ARCH_AVR -D$(BOARD_TYPE)
CXXDEFS = -DF_CPU=$(F_CPU) -DARDUINO=10608 -DARDUINO_ARCH_AVR -D$(BOARD_TYPE)

# Place -I options here
CINCS = -I$(ARDUINO)
CXXINCS = -I$(ARDUINO)

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CXXSTANDARD = -std=gnu++98
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wextra -Wreturn-type # -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CSIZEOPT = -ffunction-sections -fdata-sections # -flto -fno-fat-lto-objects
CXXSIZEOPT = $(CSIZEOPT)
# CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
# CXXFIXSTATICS = -fno-threadsafe-statics # use with caution
CXXEXTRA = -fno-exceptions # -fpermissive # -flto

LDSIZEOPT = -Wl,-gc-sections

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CTUNING) $(CSIZEOPT) $(CEXTRA)
CXXFLAGS = $(CDEBUG) $(CXXSTANDARD) $(CDEFS) $(CINCS) -O$(OPT) $(CXXFIXSTATICS) $(CXXSIZEOPT) $(CXXEXTRA)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
LDFLAGS = $(LDSIZEOPT)

# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
  -b $(UPLOAD_RATE)

# Program settings
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: export BOARD_TYPE = AVR_KEYBOARD_TOY_RELEASE
all: boardtypeecho build

# Board-specific targets
alltest: export BOARD_TYPE = AVR_KEYBOARD_TOY_TEST
alltest: boardtypeecho build

# Use the top one if you need the eep file
#build: elf hex eep symlink elfsize
build: elf hex symlink elfsizeavr elfsize

boardtypeecho:
	echo "Building for boardtype: " $(BOARD_TYPE)

elfsize: elf
	$(SIZE) $(TARGET).elf

elfsizeavr: elf
	$(SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf

hexsize: hex
	$(SIZE) $(TARGET).hex

symlink: hex
	ln -s -f $(shell pwd)/$(TARGET).hex ../$(TARGET).hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Program the device.  
upload: $(TARGET).hex # $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)




# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -S -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -S -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@



# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@



# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf ../$(TARGET).hex \
	$(TARGET).map $(TARGET).sym $(TARGET).lss \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

.PHONY:	all alltest build elf hex eep lss sym program coff extcoff clean depend
