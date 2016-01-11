# Project name
PROJ_NAME = gp_daq

# Source files to add to the build
C_FILES = main.c

# Assembly files to add to the buid
ASM_FILES = 

# Project folders
INC_PATH = inc
SRC_PATH = src
OBJ_PATH = obj
OUT_PATH = out
RES_PATH = res

# Placeholders for executables
CC = avr-gcc
LD = avr-gcc
AS = avr-as
OD = avr-objdump
CP = avr-objcopy
SZ = avr-size
 
# Build flags
CC_FLAGS = -I./$(INC_PATH)/ -Wall -Wextra -Wno-main -std=gnu99 -pedantic -Os -c -g \
							-mmcu=atmega328p -fno-builtin -DF_CPU=18432000u \
							-fno-inline-small-functions -fno-move-loop-invariants \
							-fno-tree-scev-cprop
LD_FLAGS = -Wl,--relax,--gc-sections -Wl,--section-start=.text=0000 -mmcu=atmega328p -DF_CPU=18432000u
AS_FLAGS = 
OD_FLAGS = -S
CP_FLAGS = -O ihex 
SZ_FLAGS = 
				 
# Make a list with all required objects
C_OBJ_LIST = $(C_FILES:.c=.o)
ASM_OBJ_LIST = $(ASM_FILES:.s=.o)

# Add paths to objects
C_OBJ = $(patsubst %, $(OBJ_PATH)/%, $(C_OBJ_LIST))
ASM_OBJ = $(patsubst %, $(OBJ_PATH)/%, $(ASM_OBJ_LIST))

# Generic make all o's from c files
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@echo -n Compiling $<...
	@$(CC) $(CC_FLAGS) -c $< -o $@
	@echo [Done]

# Generic make all o's from asm files
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.s
	@echo -n Assembling $<...
	@$(AS) $(AS_FLAGS) -o $@ $<	
	@echo [Done]

# Do all operations including flashing
all: build size

# Do only a rebuild
build: clean rebuild

# Build obj files and link
rebuild: make_c make_asm link disasm make_bin

# Invoke linker
link:
	@echo -n Linking files...
	@$(LD) $(LD_FLAGS) -o $(OUT_PATH)/$(PROJ_NAME).elf $(C_OBJ) $(ASM_OBJ)
	@echo [Done]

# Invoke compiler	
make_c: $(C_OBJ)
		
# Invoke assembler
make_asm: $(ASM_OBJ)

# Generate binary
make_bin:
	@echo -n Making binary...
	@$(CP) $(CP_FLAGS) $(OUT_PATH)/$(PROJ_NAME).elf $(OUT_PATH)/$(PROJ_NAME).hex
	@echo [Done]

# Run disassembler on the elf file
disasm:	
	@echo -n Disassembling source...
	@$(OD) $(OD_FLAGS) $(OUT_PATH)/$(PROJ_NAME).elf > $(OUT_PATH)/$(PROJ_NAME).lst
	@echo [Done]

# Write flash contents to target
flash:
	@echo -n Flashing target...
	#@avrdude -c avr910 -P com21 -b 115200 -p m328p -F -U flash:w:$(OUT_PATH)/$(PROJ_NAME).hex -v
	@echo [Done] 	
	
# Write fuse values	
fuse:
	@echo -n Writing fuse values...
	#@avrdude -c buspirate -P com19 -p m168 -F -U hfuse:w:0xa4:m -U lfuse:w:0xa7:m -U efuse:w:0xf8:m
	@echo [Done] 	
	
# Print size
size:
	@echo -n Computing size...
	@$(SZ) $(SZ_FLAGS) $(OUT_PATH)/$(PROJ_NAME).elf
	@echo [Done] 	

# Clean existing obj files
clean:
	@echo -n Cleaning existing files...
	@rm -f $(OBJ_PATH)/*
	@rm -f $(OUT_PATH)/*
	@echo [Done]
	