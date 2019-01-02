BUILD_ROOT=../..
OBJS = main.o

INCLUDE_DIRS += -I$(BUILD_ROOT)/projects/lib_test/src
LIB_DIRS  += $(BUILD_ROOT)/projects/lib_test
LDLIBS += -lstmled

BINARY_NAME = main

include $(BUILD_ROOT)/common_defs.mk

.PHONY: $(BINARY_NAME)

$(BINARY_NAME): $(BINARY_NAME).bin

clean:
	@rm -f $(OBJS) main.elf main.bin main.elf.map

flash: $(BINARY_NAME)
	$(FLASH_BINARY)
