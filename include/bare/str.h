/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#include <stdbool.h>
#include <stddef.h>
typedef struct {
  char *data;
  size_t len;
  size_t cap;
} bare_str_t;

typedef struct {
  char **items;
  size_t count;
  size_t cap;
} bare_strlist_t;

bare_str_t bare_str_new(void);
bare_str_t bare_str_from(const char *s);
void bare_str_free(bare_str_t *str);
void bare_str_push(bare_str_t *str, char c);
void bare_str_append(bare_str_t *str, const char *s);
void bare_str_appendn(bare_str_t *str, const char *s, size_t n);
bare_str_t bare_str_slice(const bare_str_t *str, size_t start, size_t end);
bool bare_str_eq(const bare_str_t *a, const bare_str_t *b);
bool bare_str_eqz(const bare_str_t *a, const char *b);
bool bare_str_starts_with(const bare_str_t *str, const char *prefix);
bool bare_str_ends_with(const bare_str_t *str, const char *suffix);
bare_strlist_t bare_str_split(const bare_str_t *str, char delim);
bare_str_t bare_str_join(const bare_strlist_t *list, const char *sep);
void bare_str_trim(bare_str_t *str);

bare_strlist_t bare_strlist_new(void);
void bare_strlist_push(bare_strlist_t *list, const char *s);
void bare_strlist_free(bare_strlist_t *list);
