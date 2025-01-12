LD=ld
ASM=nasm -f elf64
CC=gcc

BUILD_DIR=build
SRC_DIR=src

SRC = $(wildcard $(SRC_DIR)/*.s)
OBJ = $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(SRC))

.PHONY: all run clean

all: $(BUILD_DIR)/os-image.bin

run: $(BUILD_DIR)/os-image.bin
	qemu-system-x86_64 -no-reboot -drive file=$<,format=raw,index=0,media=disk

$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/linked.o
	objcopy -O binary $< $@


$(BUILD_DIR)/linked.o: $(OBJ)
	$(LD) -T linker.ld -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(ASM) $< -o $@

clean:
	$(RM) -f *.bin *.o *.dis
	$(RM) -rf $(BUILD_DIR)
