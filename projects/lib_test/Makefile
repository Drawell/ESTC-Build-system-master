BUILD_ROOT = ../..
include $(BUILD_ROOT)/common_defs.mk

LIB = libstmled.a

all: $(LIB)
	@echo "Done"

SRC_DIR = ./src
HDR_DIR = ./include

SRCS = $(wildcard  $(addsuffix /*.c, $(SRC_DIR)))

OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

$(LIB): $(STARTUP) $(OBJS)
	@echo "Linking library $@"
	$(MAKE_LIBRARY)

clean:
	@echo "Cleaning directory"
	@rm -f $(OBJS) $(DEPS) $(LIB) $(STARTUP)
