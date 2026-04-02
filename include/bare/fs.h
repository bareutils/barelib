/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#define _POSIX_C_SOURCE 200809L
#include "err.h"
#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

typedef enum {
  BARE_FT_UNKNOWN,
  BARE_FT_REG,
  BARE_FT_DIR,
  BARE_FT_LINK,
  BARE_FT_FIFO,
  BARE_FT_SOCK,
  BARE_FT_BLK,
  BARE_FT_CHR,
} bare_filetype_t;

typedef struct {
  char *name;
  char *path;
  bare_filetype_t type;
  mode_t mode;
  uid_t uid;
  gid_t gid;
  off_t size;
  time_t mtime;
  time_t atime;
  time_t ctime;
  nlink_t nlink;
  ino_t ino;
  dev_t dev;
  bool is_hidden;
} bare_entry_t;

typedef struct {
  bare_entry_t *items;
  size_t count;
  size_t cap;
} bare_dir_t;

bare_err_t bare_dir_read(const char *path, bare_dir_t *out);
void bare_dir_free(bare_dir_t *dir);
void bare_dir_sort_name(bare_dir_t *dir);
void bare_dir_sort_mtime(bare_dir_t *dir);
void bare_dir_sort_size(bare_dir_t *dir);

bare_err_t bare_entry_stat(const char *path, bare_entry_t *out);
void bare_entry_free(bare_entry_t *e);

char *bare_path_join(const char *a, const char *b);
char *bare_path_basename(const char *path);
char *bare_path_dirname(const char *path);
char *bare_path_resolve(const char *path);
bool bare_path_exists(const char *path);
bool bare_path_is_dir(const char *path);

const char *bare_filetype_str(bare_filetype_t type);
char bare_filetype_char(bare_filetype_t type);
