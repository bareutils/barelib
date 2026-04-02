CC      ?= gcc
AR      ?= ar
ARFLAGS ?= rcs
PREFIX  ?= /usr
BUILD   ?= release
FPIC    ?= -fPIC
STD 	?= c11
OUTDIR  ?= out/$(BUILD)
DESTDIR ?= /
LIBDIR  := $(OUTDIR)/lib
BINDIR  := $(OUTDIR)/bin
OBJDIR  := $(OUTDIR)/obj
BARELIB_PREFIX ?= .
CFLAGS  += -std=$(STD) -Wall -Wextra -Wpedantic -Wshadow -Wstrict-aliasing \
           -Wcast-align -Wpointer-arith -Wno-unused-parameter

CFLAGS_DEBUG   ?= -g3 -O0 -fsanitize=address,undefined -DDEBUG
CFLAGS_RELEASE ?= -O2 -DNDEBUG
ifeq ($(BUILD),debug)
    ACTIVE_CFLAGS += $(CFLAGS_DEBUG)
    ACTIVE_LDFLAGS += -fsanitize=address,undefined $(EXTRA_LD)
else
    ACTIVE_CFLAGS += $(CFLAGS_RELEASE)
	ACTIVE_LDFLAGS += $(EXTRA_LD)
endif
ACTIVE_CFLAGS += $(CFLAGS)
