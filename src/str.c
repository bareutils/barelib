/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#include "bare/str.h"
#include "bare/mem.h"
#include <ctype.h>
#include <string.h>

#define STR_INIT_CAP 16

bare_str_t bare_str_new(void) {
  bare_str_t s;
  s.cap = STR_INIT_CAP;
  s.len = 0;
  s.data = bare_malloc(s.cap);
  s.data[0] = '\0';
  return s;
}

bare_str_t bare_str_from(const char *src) {
  bare_str_t s = bare_str_new();
  bare_str_append(&s, src);
  return s;
}

void bare_str_free(bare_str_t *str) {
  bare_free(str->data);
  str->data = NULL;
  str->len = 0;
  str->cap = 0;
}

static void str_grow(bare_str_t *str, size_t need) {
  while (str->cap <= need)
    str->cap *= 2;
  str->data = bare_realloc(str->data, str->cap);
}

void bare_str_push(bare_str_t *str, char c) {
  if (str->len + 1 >= str->cap)
    str_grow(str, str->len + 1);
  str->data[str->len++] = c;
  str->data[str->len] = '\0';
}

void bare_str_append(bare_str_t *str, const char *s) {
  bare_str_appendn(str, s, strlen(s));
}

void bare_str_appendn(bare_str_t *str, const char *s, size_t n) {
  if (str->len + n >= str->cap)
    str_grow(str, str->len + n);
  memcpy(str->data + str->len, s, n);
  str->len += n;
  str->data[str->len] = '\0';
}

bare_str_t bare_str_slice(const bare_str_t *str, size_t start, size_t end) {
  if (start > str->len)
    start = str->len;
  if (end > str->len)
    end = str->len;
  if (start > end)
    start = end;
  bare_str_t out = bare_str_new();
  bare_str_appendn(&out, str->data + start, end - start);
  return out;
}

bool bare_str_eq(const bare_str_t *a, const bare_str_t *b) {
  return a->len == b->len && memcmp(a->data, b->data, a->len) == 0;
}

bool bare_str_eqz(const bare_str_t *a, const char *b) {
  return strcmp(a->data, b) == 0;
}

bool bare_str_starts_with(const bare_str_t *str, const char *prefix) {
  size_t n = strlen(prefix);
  return str->len >= n && memcmp(str->data, prefix, n) == 0;
}

bool bare_str_ends_with(const bare_str_t *str, const char *suffix) {
  size_t n = strlen(suffix);
  return str->len >= n && memcmp(str->data + str->len - n, suffix, n) == 0;
}

bare_strlist_t bare_str_split(const bare_str_t *str, char delim) {
  bare_strlist_t list = bare_strlist_new();
  const char *start = str->data;
  const char *p = str->data;
  while (*p) {
    if (*p == delim) {
      bare_strlist_push(&list, "");
      size_t len = (size_t)(p - start);
      char *chunk = bare_strndup(start, len);
      bare_free(list.items[list.count - 1]);
      list.items[list.count - 1] = chunk;
      start = p + 1;
    }
    p++;
  }
  bare_strlist_push(&list, start);
  return list;
}

bare_str_t bare_str_join(const bare_strlist_t *list, const char *sep) {
  bare_str_t out = bare_str_new();
  for (size_t i = 0; i < list->count; i++) {
    if (i > 0)
      bare_str_append(&out, sep);
    bare_str_append(&out, list->items[i]);
  }
  return out;
}

void bare_str_trim(bare_str_t *str) {
  size_t start = 0;
  while (start < str->len && isspace((unsigned char)str->data[start]))
    start++;
  size_t end = str->len;
  while (end > start && isspace((unsigned char)str->data[end - 1]))
    end--;
  if (start > 0)
    memmove(str->data, str->data + start, end - start);
  str->len = end - start;
  str->data[str->len] = '\0';
}

bare_strlist_t bare_strlist_new(void) {
  bare_strlist_t list;
  list.cap = 8;
  list.count = 0;
  list.items = bare_malloc(list.cap * sizeof(char *));
  return list;
}

void bare_strlist_push(bare_strlist_t *list, const char *s) {
  if (list->count >= list->cap) {
    list->cap *= 2;
    list->items = bare_realloc(list->items, list->cap * sizeof(char *));
  }
  list->items[list->count++] = bare_strdup(s);
}

void bare_strlist_free(bare_strlist_t *list) {
  for (size_t i = 0; i < list->count; i++)
    bare_free(list->items[i]);
  bare_free(list->items);
  list->items = NULL;
  list->count = 0;
  list->cap = 0;
}
