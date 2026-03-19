PROJECT           := barelib
PROJECT_NAMESPACE := bare
VERSION           := 0.0.0
include maketools/config.mk
CFLAGS += -Iinclude -DBARELIB_VERSION=\"$(VERSION)\"
LIB_SRCS      := $(wildcard src/*.c)
LIB_OBJS      := $(patsubst src/%.c,$(OBJDIR)/%.o,$(LIB_SRCS))
EXAMPLE_SRCS  := $(wildcard examples/*.c)
EXAMPLE_BINS  := $(patsubst examples/%.c,$(BINDIR)/%,$(EXAMPLE_SRCS))
BINARIES := $(EXAMPLE_BINS)
.PHONY: all lib examples clean debug release install uninstall
SHARED ?= 0
TARGETS := $(LIBDIR)/lib$(PROJECT).a
ifeq ($(SHARED),1)
    TARGETS += $(LIBDIR)/lib$(PROJECT).so
endif
all: lib examples
lib: $(TARGETS)
ifneq ($(EXAMPLE_SRCS),)
examples: lib $(EXAMPLE_BINS)
else
examples: lib
endif
debug release:
	$(MAKE) BUILD=$@ all
include maketools/build.mk
include maketools/install.mk
install: install-lib install-bin
uninstall: uninstall-lib uninstall-bin
