/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#include <stdnoreturn.h>
typedef enum {
  BARE_OK = 0,
  BARE_ENOMEM,
  BARE_EINVAL,
  BARE_EIO,
  BARE_ENOENT,
  BARE_EPERM,
  BARE_EEXIST,
  BARE_EOVERFLOW,
  BARE_EUNKNOWN,
} bare_err_t;

const char *bare_strerror(bare_err_t err);
noreturn void bare_die(const char *prog, const char *msg);
noreturn void bare_die_err(const char *prog, bare_err_t err);
noreturn void bare_die_errno(const char *prog, const char *msg);
