/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#include "bare/err.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *bare_strerror(bare_err_t err) {
  switch (err) {
  case BARE_OK:
    return "success";
  case BARE_ENOMEM:
    return "out of memory";
  case BARE_EINVAL:
    return "invalid argument";
  case BARE_EIO:
    return "I/O error";
  case BARE_ENOENT:
    return "no such file or directory";
  case BARE_EPERM:
    return "permission denied";
  case BARE_EEXIST:
    return "file exists";
  case BARE_EOVERFLOW:
    return "overflow";
  default:
    return "unknown error";
  }
}

noreturn void bare_die(const char *prog, const char *msg) {
  fprintf(stderr, "%s: %s\n", prog, msg);
  exit(1);
}

noreturn void bare_die_err(const char *prog, bare_err_t err) {
  bare_die(prog, bare_strerror(err));
}

noreturn void bare_die_errno(const char *prog, const char *msg) {
  fprintf(stderr, "%s: %s: %s\n", prog, msg, strerror(errno));
  exit(1);
}
