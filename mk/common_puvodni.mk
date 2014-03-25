###############################################################################
# C flags

CFLAGS	+= -Og -ggdb3 -std=gnu99
CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS	+= -ffunction-sections -fdata-sections -ffast-math -fno-common
#additional warnings
CFLAGS	+= -Wmissing-declarations -Wmissing-include-dirs -Wunreachable-code

###############################################################################
# C++ flags

CXXFLAGS	+= -Og -ggdb3 -std=gnu++11
CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls
# -Weffc++
CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections
CXXFLAGS	+= -fno-exceptions -fno-unwind-tables
CXXFLAGS	+= -std=gnu++11

###############################################################################
# C & C++ preprocessor common flags

CPPFLAGS+= -MD -MP -MF $(INTERMEDIATE_DIR)$(*F).d
CPPFLAGS+= -Wall -Wundef
CPPFLAGS+= $(DEFS)
#additional warnings
CPPFLAGS+= -Winline -Winit-self -Wuninitialized -Wfloat-equal -Wcast-qual
CPPFLAGS+= -Wcast-align -Wswitch-enum -Wswitch-default -Wformat=2

###############################################################################
# Linker flags

LDFLAGS	+= --static -nostartfiles
LDFLAGS	+= -Wl,--gc-sections
LDFLAGS	+= --specs=nano.specs
#LDFLAGS += -Wl,--no-warn-mismatch

###############################################################################
# Linker libraries to be built with

LDLIBS	+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group


