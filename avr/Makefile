# Output file name
TARG     = %{ProjectName}

# MCU name and frequency
MCU      = %{MCU}
F_CPU    = %{F_CPU}

# Source files
SRCS     = $(wildcard *.c)

# Build directory
BUILDDIR = build

# Compiler options
OPTIMIZE = -Os -mcall-prologues -fshort-enums -ffunction-sections -fdata-sections
#DEBUG    = -g -Wall -Werror
DEPS     = -MMD -MP -MT $(BUILDDIR)/$(*F).o -MF $(BUILDDIR)/$(*D)/$(*F).d
CFLAGS   = $(DEBUG) -lm $(OPTIMIZE) $(DEPS) -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS  = $(DEBUG) -mmcu=$(MCU) -Wl,-gc-sections -mrelax

# AVR toolchain and flasher
CC       = avr-gcc
OBJCOPY  = avr-objcopy
OBJDUMP  = avr-objdump

# AVRDude parameters
AVRDUDE  = avrdude
AD_MCU   = -p $(MCU)
#AD_PROG = -c stk500v2
#AD_PORT = -P avrdoper

AD_CMD   = $(AD_MCU) $(AD_PROG) $(AD_PORT) -V

# Build objects
OBJS     = $(addprefix $(BUILDDIR)/, $(SRCS:.c=.o))
ELF      = $(BUILDDIR)/$(TARG).elf

# Dependencies
-include $(OBJS:.o=.d)

all: $(ELF) size

$(ELF): $(OBJS)
	@mkdir -p $(BUILDDIR) flash
	$(CC) $(LDFLAGS) -o $(ELF) $(OBJS) -lm
	# Creating flash image (ihex)
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram -R .fuse -R .lock -R .signature $(ELF) flash/$(TARG).hex
	# Creating listing
	$(OBJDUMP) -h -S $(ELF) > $(BUILDDIR)/$(TARG).lss

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

binary: $(ELF)
	# Creating flash image (binary)
	$(OBJCOPY) -O binary -R .eeprom -R .nwram -R .fuse -R .lock -R .signature $(ELF) flash/$(TARG).bin

eeprom: $(ELF)
	# Creating EEPROM image (ihex)
	$(OBJCOPY) -O ihex -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 $(ELF) flash/$(TARG).eep

fuses: $(ELF)
	avr-objcopy -j .fuse -O ihex $(RLF) flash/fuses.hex --change-section-lma .fuse=0
	srec_cat fuses.hex -Intel -crop 0x00 0x01 -offset  0x00 -O flash/lfuse.hex -Intel
	srec_cat fuses.hex -Intel -crop 0x01 0x02 -offset -0x01 -O flash/hfuse.hex -Intel
	srec_cat fuses.hex -Intel -crop 0x02 0x03 -offset -0x02 -O flash/efuse.hex -Intel

size: $(ELF)
	avr-size --format=avr --mcu=${MCU} ${ELF}

clean:
	rm -rf $(BUILDDIR)

flash: $(ELF)
	$(AVRDUDE) $(AD_CMD) -U flash:w:flash/$(TARG).hex:i

fuse:
	$(AVRDUDE) $(AD_CMD) -U lfuse:w:flash/lfuse.hex:i -U hfuse:w:flash/hfuse.hex:i -U efuse:w:flash/efuse.hex:i

.PHONY: all binary eeprom fuses size clean flash fuse
