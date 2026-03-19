/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#include "err.h"
#include <stddef.h>
void *bare_malloc(size_t size);
void *bare_calloc(size_t nmemb, size_t size);
void *bare_realloc(void *ptr, size_t size);
void bare_free(void *ptr);
char *bare_strdup(const char *s);
char *bare_strndup(const char *s, size_t n);
