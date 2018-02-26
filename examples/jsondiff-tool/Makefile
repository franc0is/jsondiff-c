CC := gcc-6
LD := gcc-6
RM := rm
MKDIR := mkdir
AR := gar

PROJECT := jsondiff-tool
BUILD_DIR := build/a/b/c

ifneq ($(VERBOSE),)
  NO_ECHO=
else
  NO_ECHO=@
endif

ifneq ($(DEBUG),)
  DEFINES = DEBUG
endif

IGNORE_ERRORS = >/dev/null 2>&1 || true

SOURCES := jsondiff-tool.c \
           ../../3rdparty/argtable3-3.0.3/argtable3.c

INCLUDES := ../.. \
            ../../3rdparty/argtable3-3.0.3

LIBRARIES := ../../build/libjsondiff.a

CFLAGS :=
LDFLAGS := -ljansson

OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES))
INC := $(addprefix -I,$(INCLUDES))
DEFS := $(addprefix -D,$(DEFINES))

#
# Rules
#

$(BUILD_DIR)/%.o: %.c
	@echo Compiling $(notdir $<)
	$(NO_ECHO)$(MKDIR) -p $(dir $@)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@

#
# Targets
#

$(BUILD_DIR)/$(PROJECT): $(OBJS) $(LIBRARIES)
	@echo Linking $@
	$(NO_ECHO)$(LD) $(LDFLAGS) -o $@ $^

clean:
	$(NO_ECHO)$(RM) -r $(BUILD_DIR)

.PHONY: clean
