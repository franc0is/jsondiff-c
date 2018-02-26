CC := gcc-6
LD := gcc-6
RM := rm
MKDIR := mkdir
AR := ar crv

BUILD_DIR := build

ifneq ($(VERBOSE),)
  NO_ECHO=
else
  NO_ECHO=@
endif

ifneq ($(DEBUG),)
  DEFINES = DEBUG
endif

IGNORE_ERRORS = >/dev/null 2>&1 || true

SOURCES := src/jsondiff.c

INCLUDES := . src
CFLAGS :=
LDFLAGS :=

# Include libs here

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

$(BUILD_DIR)/libjsondiff.a: $(OBJS)
	@echo Packaging $@
	$(NO_ECHO)$(AR) $@ $(OBJS)

clean:
	$(NO_ECHO)$(RM) -r $(BUILD_DIR)

.PHONY: clean
