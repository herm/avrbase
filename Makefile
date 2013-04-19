MCU = atmega8
Q =
# # Q = @
OPT = s

SRC = 
CPPSRC = main.cpp test.cpp
ASRC =

PROGRAMMER = lm3s811
###################################################################
BASEFLAGS = -g3 -O$(OPT)
BASEFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
BASEFLAGS += -Wall -Wcast-align -Wcast-qual -Wextra -Wshadow
BASEFLAGS += -Wpointer-arith -Wswitch -Wredundant-decls -Wreturn-type -Wunused
BASEFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<)
BASEFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
BASEFLAGS += -ffunction-sections -funit-at-a-time -mcall-prologues -ffreestanding
BASEFLAGS += -fmerge-all-constants -ffast-math -funsafe-loop-optimizations #Dangerous?

CONLYFLAGS = -Wstrict-prototypes -Wimplicit
CPPONLYFLAGS = -fno-exceptions -fno-default-inline -finline-limit=100 -fno-threadsafe-statics
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),-g3


###################################################################
PREFIX = avr
CC = $(PREFIX)-gcc
CXX = $(PREFIX)-c++
OBJDUMP = $(PREFIX)-objdump
OBJCOPY = $(PREFIX)-objcopy
NM = $(PREFIX)-nm
SIZE = getsize.py


COBJ      = $(SRC:.c=.o)
AOBJ      = $(ASRC:.S=.o)
CPPOBJ    = $(CPPSRC:.cpp=.o) 

# MD = create dependency rules + object file
# MF = dependency output file
# MP = create dummy rules for headers so make doesn't complain about removed header files
BASEFLAGS += -MD -MP -MF .dep/$(@F).d
BASEFLAGS += -mmcu=$(MCU) -I.
CFLAGS = $(BASEFLAGS) $(CONLYFLAGS)
CXXFLAGS = $(BASEFLAGS) $(CPPONLYFLAGS)
ASFLAGS += -mcpu=$(MCU) -I. -x assembler-with-cpp
LDFLAGS = -lc -lgcc -funit-at-a-time -Wl,--gc-sections -Wl,--relax

all: prepare sizebefore build sizeafter mmap
build: debug/main.elf debug/main.lss

prepare:
	@mkdir -p debug
.PHONY: sizebefore
sizebefore:
	@if [ -f debug/main.elf ]; then cp debug/main.elf debug/main.old; fi

sizeafter:
	@$(SIZE) debug/main.elf

program:
	@if ! avrdude -c lm3s811 -p $(MCU) -U flash:v:debug/main.elf -q -q; then \
		avrdude -c lm3s811 -p $(MCU) -U flash:w:debug/main.elf; \
	else \
		echo "Already programmed."; \
	fi

.SECONDARY : debug/main.elf
.PRECIOUS : $(AOBJ) $(COBJ) $(CPPOBJ)
debug/main.elf:  $(AOBJ) $(COBJ) $(CPPOBJ)
	@echo "    LD" $@
	$(Q)$(CXX) $(BASEFLAGS) $(AOBJ) $(COBJ) $(CPPOBJ) --output $@ $(LDFLAGS)

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(Q)$(OBJDUMP) -h -S -C $< > $@

%.bin: %.elf
	@$(OBJCOPY) -O binary $< $@

mmap: debug/main.elf
	@$(NM) -C -S -r --radix=d --size-sort debug/main.elf > debug/mmap.size
	@echo "upper: GLOBAL lower: local, B: BSS, D: Ram, T: code" > debug/size_map
	@cat debug/mmap.size | egrep -i "\ t\ " >> debug/size_map || true
	@cat debug/mmap.size | egrep -i "\ d\ " >> debug/size_map || true
	@cat debug/mmap.size | egrep -i "\ b\ " >> debug/size_map || true
	@rm debug/mmap.size
	@$(NM) -C -S --radix=d --numeric-sort debug/main.elf > debug/mmap.loc
	@cat debug/mmap.loc | egrep -i "\ t\ " >  debug/loc_map || true
	@cat debug/mmap.loc | egrep -i "\ d\ " >> debug/loc_map || true
	@cat debug/mmap.loc | egrep -i "\ b\ " >> debug/loc_map || true

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

clean:
	rm -f debug/main.elf $(COBJ) $(CPPOBJ) $(AOBJ) $(ASRC:.S=.lst) $(SRC:.c=.lst) $(CPPSRC:.cpp=.lst) .dep/* *~