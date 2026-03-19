/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include "bare/io.h"
#include "bare/mem.h"
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

bool bare_isatty(FILE *f) { return isatty(fileno(f)); }

int bare_term_width(void) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
    return ws.ws_col;
  const char *cols = getenv("COLUMNS");
  if (cols)
    return atoi(cols);
  return 80;
}

int bare_term_height(void) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0)
    return ws.ws_row;
  return 24;
}

void bare_color_set(FILE *f, bare_color_t fg, bare_color_t bg) {
  if (!bare_isatty(f))
    return;
  if (bg == BARE_COLOR_RESET)
    fprintf(f, "\033[%dm", (int)fg);
  else
    fprintf(f, "\033[%d;%dm", (int)fg, (int)bg + 10);
}

void bare_attr_set(FILE *f, bare_attr_t attr) {
  if (!bare_isatty(f))
    return;
  fprintf(f, "\033[%dm", (int)attr);
}

void bare_color_reset(FILE *f) {
  if (!bare_isatty(f))
    return;
  fputs("\033[0m", f);
}

void bare_print_columns(FILE *f, const char **items, size_t n, int term_width) {
  if (n == 0)
    return;

  size_t max_len = 0;
  for (size_t i = 0; i < n; i++) {
    size_t l = strlen(items[i]);
    if (l > max_len)
      max_len = l;
  }

  int col_w = (int)max_len + 2;
  int ncols = term_width / col_w;
  if (ncols < 1)
    ncols = 1;
  size_t nrows = (n + (size_t)ncols - 1) / (size_t)ncols;

  for (size_t row = 0; row < nrows; row++) {
    for (int col = 0; col < ncols; col++) {
      size_t idx = (size_t)col * nrows + row;
      if (idx >= n)
        break;
      int is_last = (col == ncols - 1) || (idx + nrows >= n);
      if (is_last)
        fprintf(f, "%s", items[idx]);
      else
        fprintf(f, "%-*s", col_w, items[idx]);
    }
    fputc('\n', f);
  }
}

char *bare_fmt_size(unsigned long long bytes) {
  const char *units[] = {"B", "K", "M", "G", "T", "P"};
  int u = 0;
  double val = (double)bytes;
  while (val >= 1024.0 && u < 5) {
    val /= 1024.0;
    u++;
  }
  char *buf = bare_malloc(16);
  if (u == 0)
    snprintf(buf, 16, "%llu", bytes);
  else
    snprintf(buf, 16, "%.1f%s", val, units[u]);
  return buf;
}

char *bare_fmt_mode(unsigned int mode) {
  char *buf = bare_malloc(11);
  buf[0] = S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' : '-';
  buf[1] = (mode & S_IRUSR) ? 'r' : '-';
  buf[2] = (mode & S_IWUSR) ? 'w' : '-';
  buf[3] = (mode & S_ISUID) ? 's' : (mode & S_IXUSR) ? 'x' : '-';
  buf[4] = (mode & S_IRGRP) ? 'r' : '-';
  buf[5] = (mode & S_IWGRP) ? 'w' : '-';
  buf[6] = (mode & S_ISGID) ? 's' : (mode & S_IXGRP) ? 'x' : '-';
  buf[7] = (mode & S_IROTH) ? 'r' : '-';
  buf[8] = (mode & S_IWOTH) ? 'w' : '-';
  buf[9] = (mode & S_ISVTX) ? 't' : (mode & S_IXOTH) ? 'x' : '-';
  buf[10] = '\0';
  return buf;
}

char *bare_fmt_time(long mtime, const char *fmt) {
  time_t t = (time_t)mtime;
  struct tm tm;
  localtime_r(&t, &tm);
  char *buf = bare_malloc(64);
  if (!fmt)
    fmt = "%b %e %H:%M";
  strftime(buf, 64, fmt, &tm);
  return buf;
}

char *bare_fmt_owner(unsigned int uid, unsigned int gid) {
  struct passwd *pw = getpwuid(uid);
  struct group *gr = getgrgid(gid);
  const char *uname = pw ? pw->pw_name : "?";
  const char *gname = gr ? gr->gr_name : "?";
  size_t len = strlen(uname) + strlen(gname) + 2;
  char *buf = bare_malloc(len);
  snprintf(buf, len, "%s:%s", uname, gname);
  return buf;
}
