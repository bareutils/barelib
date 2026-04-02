/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum {
  BARE_COLOR_RESET = 0,
  BARE_COLOR_BLACK = 30,
  BARE_COLOR_RED = 31,
  BARE_COLOR_GREEN = 32,
  BARE_COLOR_YELLOW = 33,
  BARE_COLOR_BLUE = 34,
  BARE_COLOR_MAGENTA = 35,
  BARE_COLOR_CYAN = 36,
  BARE_COLOR_WHITE = 37,
  BARE_COLOR_BBLACK = 90,
  BARE_COLOR_BRED = 91,
  BARE_COLOR_BGREEN = 92,
  BARE_COLOR_BYELLOW = 93,
  BARE_COLOR_BBLUE = 94,
  BARE_COLOR_BMAGENTA = 95,
  BARE_COLOR_BCYAN = 96,
  BARE_COLOR_BWHITE = 97,
} bare_color_t;

typedef enum {
  BARE_ATTR_BOLD = 1,
  BARE_ATTR_DIM = 2,
  BARE_ATTR_ITALIC = 3,
  BARE_ATTR_UNDERLINE = 4,
  BARE_ATTR_BLINK = 5,
  BARE_ATTR_REVERSE = 7,
} bare_attr_t;

bool bare_isatty(FILE *f);
int bare_term_width(void);
int bare_term_height(void);
bool bare_clear(FILE *f);
void bare_color_set(FILE *f, bare_color_t fg, bare_color_t bg);
void bare_attr_set(FILE *f, bare_attr_t attr);
void bare_color_reset(FILE *f);
void bare_print_columns(FILE *f, const char **items, size_t n, int term_width);
char *bare_fmt_size(unsigned long long bytes);
char *bare_fmt_mode(unsigned int mode);
char *bare_fmt_time(long mtime, const char *fmt);
char *bare_fmt_owner(unsigned int uid, unsigned int gid);
