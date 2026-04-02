/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include "bare/mem.h"
#include "bare/err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *bare_malloc(size_t size) {
  void *p = malloc(size);
  if (!p) {
    fputs("barelib: out of memory\n", stderr);
    abort();
  }
  return p;
}

void *bare_calloc(size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (!p) {
    fputs("barelib: out of memory\n", stderr);
    abort();
  }
  return p;
}

void *bare_realloc(void *ptr, size_t size) {
  void *p = realloc(ptr, size);
  if (!p) {
    fputs("barelib: out of memory\n", stderr);
    abort();
  }
  return p;
}

void bare_free(void *ptr) { free(ptr); }

char *bare_strdup(const char *s) {
  size_t len = strlen(s);
  char *out = bare_malloc(len + 1);
  memcpy(out, s, len + 1);
  return out;
}

char *bare_strndup(const char *s, size_t n) {
  size_t len = strnlen(s, n);
  char *out = bare_malloc(len + 1);
  memcpy(out, s, len);
  out[len] = '\0';
  return out;
}
