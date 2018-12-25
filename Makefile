BUILD_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


SPL_PATH = $(BUILD_ROOT)/stm_spl/
COMMON_PATH = $(BUILD_ROOT)/common
MY_LIB_PATH = $(BUILD_ROOT)/projects/lib_test
LABS_DIR = $(BUILD_ROOT)/projects

.PHONY: labs spl common lab1 clean

all: labs

labs: lab1

spl:
	make -C $(SPL_PATH)

common:
	make -C $(COMMON_PATH)

my_lib:
	make -C $(MY_LIB_PATH)

lab1: spl common my_lib
	make -C $(LABS_DIR)/led_test

clean:
	make -C $(SPL_PATH) clean	
	make -C $(COMMON_PATH) clean
	make -C $(MY_LIB_PATH) clean
	make -C $(LABS_DIR)/led_test clean
