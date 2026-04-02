/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include "bare/fs.h"
#include "bare/mem.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static bare_filetype_t mode_to_ft(mode_t m) {
  if (S_ISREG(m))
    return BARE_FT_REG;
  if (S_ISDIR(m))
    return BARE_FT_DIR;
  if (S_ISLNK(m))
    return BARE_FT_LINK;
  if (S_ISFIFO(m))
    return BARE_FT_FIFO;
  if (S_ISSOCK(m))
    return BARE_FT_SOCK;
  if (S_ISBLK(m))
    return BARE_FT_BLK;
  if (S_ISCHR(m))
    return BARE_FT_CHR;
  return BARE_FT_UNKNOWN;
}

bare_err_t bare_entry_stat(const char *path, bare_entry_t *out) {
  struct stat st;
  if (lstat(path, &st) != 0) {
    if (errno == ENOENT)
      return BARE_ENOENT;
    if (errno == EACCES)
      return BARE_EPERM;
    return BARE_EIO;
  }
  out->path = bare_strdup(path);
  out->name = bare_path_basename(path);
  out->type = mode_to_ft(st.st_mode);
  out->mode = st.st_mode;
  out->uid = st.st_uid;
  out->gid = st.st_gid;
  out->size = st.st_size;
  out->mtime = st.st_mtime;
  out->atime = st.st_atime;
  out->ctime = st.st_ctime;
  out->nlink = st.st_nlink;
  out->ino = st.st_ino;
  out->dev = st.st_dev;
  out->is_hidden = out->name[0] == '.';
  return BARE_OK;
}

void bare_entry_free(bare_entry_t *e) {
  bare_free(e->name);
  bare_free(e->path);
  e->name = NULL;
  e->path = NULL;
}

bare_err_t bare_dir_read(const char *path, bare_dir_t *out) {
  DIR *d = opendir(path);
  if (!d) {
    if (errno == ENOENT)
      return BARE_ENOENT;
    if (errno == EACCES)
      return BARE_EPERM;
    return BARE_EIO;
  }
  out->cap = 32;
  out->count = 0;
  out->items = bare_malloc(out->cap * sizeof(bare_entry_t));
  struct dirent *de;
  while ((de = readdir(d)) != NULL) {
    char *full = bare_path_join(path, de->d_name);
    bare_entry_t entry = {0};
    if (bare_entry_stat(full, &entry) == BARE_OK) {
      if (out->count >= out->cap) {
        out->cap *= 2;
        out->items = bare_realloc(out->items, out->cap * sizeof(bare_entry_t));
      }
      out->items[out->count++] = entry;
    }
    bare_free(full);
  }
  closedir(d);
  return BARE_OK;
}

void bare_dir_free(bare_dir_t *dir) {
  for (size_t i = 0; i < dir->count; i++)
    bare_entry_free(&dir->items[i]);
  bare_free(dir->items);
  dir->items = NULL;
  dir->count = 0;
  dir->cap = 0;
}

static int cmp_name(const void *a, const void *b) {
  return strcmp(((const bare_entry_t *)a)->name,
                ((const bare_entry_t *)b)->name);
}

static int cmp_mtime(const void *a, const void *b) {
  time_t ta = ((const bare_entry_t *)a)->mtime;
  time_t tb = ((const bare_entry_t *)b)->mtime;
  return (ta < tb) - (ta > tb);
}

static int cmp_size(const void *a, const void *b) {
  off_t sa = ((const bare_entry_t *)a)->size;
  off_t sb = ((const bare_entry_t *)b)->size;
  return (sa < sb) - (sa > sb);
}

void bare_dir_sort_name(bare_dir_t *dir) {
  qsort(dir->items, dir->count, sizeof(bare_entry_t), cmp_name);
}

void bare_dir_sort_mtime(bare_dir_t *dir) {
  qsort(dir->items, dir->count, sizeof(bare_entry_t), cmp_mtime);
}

void bare_dir_sort_size(bare_dir_t *dir) {
  qsort(dir->items, dir->count, sizeof(bare_entry_t), cmp_size);
}

char *bare_path_join(const char *a, const char *b) {
  size_t la = strlen(a);
  size_t lb = strlen(b);
  int slash = (la > 0 && a[la - 1] != '/') ? 1 : 0;
  char *out = bare_malloc(la + lb + slash + 1);
  memcpy(out, a, la);
  if (slash)
    out[la] = '/';
  memcpy(out + la + slash, b, lb + 1);
  return out;
}

char *bare_path_basename(const char *path) {
  const char *last = strrchr(path, '/');
  return bare_strdup(last ? last + 1 : path);
}

char *bare_path_dirname(const char *path) {
  const char *last = strrchr(path, '/');
  if (!last || last == path)
    return bare_strdup(".");
  return bare_strndup(path, (size_t)(last - path));
}

char *bare_path_resolve(const char *path) {
  char buf[4096];
  char *r = realpath(path, buf);
  return r ? bare_strdup(r) : bare_strdup(path);
}

bool bare_path_exists(const char *path) {
  struct stat st;
  return stat(path, &st) == 0;
}

bool bare_path_is_dir(const char *path) {
  struct stat st;
  return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

const char *bare_filetype_str(bare_filetype_t type) {
  switch (type) {
  case BARE_FT_REG:
    return "regular file";
  case BARE_FT_DIR:
    return "directory";
  case BARE_FT_LINK:
    return "symbolic link";
  case BARE_FT_FIFO:
    return "fifo";
  case BARE_FT_SOCK:
    return "socket";
  case BARE_FT_BLK:
    return "block device";
  case BARE_FT_CHR:
    return "character device";
  default:
    return "unknown";
  }
}

char bare_filetype_char(bare_filetype_t type) {
  switch (type) {
  case BARE_FT_DIR:
    return 'd';
  case BARE_FT_LINK:
    return 'l';
  case BARE_FT_FIFO:
    return 'p';
  case BARE_FT_SOCK:
    return 's';
  case BARE_FT_BLK:
    return 'b';
  case BARE_FT_CHR:
    return 'c';
  default:
    return '-';
  }
}
