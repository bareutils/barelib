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
ifndef ECHO
HIT_TOTAL != ${MAKE} ${MAKECMDGOALS} --dry-run ECHO="HIT_MARK" | grep -c "HIT_MARK"
HIT_COUNT = $(eval HIT_N != expr ${HIT_N} + 1)${HIT_N}
ECHO = "[\033[1;32m`expr ${HIT_COUNT} '*' 100 / ${HIT_TOTAL}`%\033[1;0m]"
endif
