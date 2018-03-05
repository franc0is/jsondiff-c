CC := g++-7
LD := g++-7
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

SOURCES := src/jsondiff.cpp

INCLUDES := . src 3rdparty/diff-match-patch-cpp-stl-master
CFLAGS := -g
LDFLAGS :=

# Include libs here

OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
INC := $(addprefix -I,$(INCLUDES))
DEFS := $(addprefix -D,$(DEFINES))

#
# Rules
#

$(BUILD_DIR)/%.o: %.cpp
	@echo Compiling $(notdir $<)
	$(NO_ECHO)$(MKDIR) -p $(dir $@)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@

#
# Targets
#

$(BUILD_DIR)/libjsondiff.a: $(OBJS)
	@echo Packaging $@
	$(NO_ECHO)$(AR) $@ $(OBJS)

test: $(BUILD_DIR)/libjsondiff.a
	$(NO_ECHO)$(MAKE) -C test

clean:
	$(NO_ECHO)$(IGNORE_ERRORS)$(RM) -r $(BUILD_DIR)
	$(NO_ECHO)$(IGNORE_ERRORS)$(MAKE) clean -C test
	$(NO_ECHO)$(IGNORE_ERRORS)$(MAKE) clean -C example/jsondiff-tool


.PHONY: clean test
