CC := g++-7
LD := g++-7
RM := rm
MKDIR := mkdir
AR := ar crv

BUILD_DIR := build
PROJECT := jsondiff_tests

ifneq ($(VERBOSE),)
  NO_ECHO=
else
  NO_ECHO=@
endif

IGNORE_ERRORS = >/dev/null 2>&1 || true

SOURCES := test_diff.cpp \
           test_apply.cpp \
           test_main.cpp

INCLUDES := .. \
            ../3rdparty/doctest-1.2.7/doctest

# FIXME why doesn't the compiler add that?
DEFINES := __MAC_OS_X_VERSION_MIN_REQUIRED

LIBRARIES := ../build/libjsondiff.a

CFLAGS := -g3 -fpermissive
LDFLAGS := -ljansson

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

all: $(BUILD_DIR)/$(PROJECT)
	@echo Running $^
	$(NO_ECHO)./$^

$(BUILD_DIR)/$(PROJECT): $(OBJS) $(LIBRARIES)
	@echo Linking $@
	$(NO_ECHO)$(LD) $(LDFLAGS) -o $@ $^

clean:
	$(NO_ECHO)$(RM) -r $(BUILD_DIR)

.PHONY: clean all

