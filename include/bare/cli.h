/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#include <stdbool.h>
#include <stddef.h>
typedef enum {
  BARE_OPT_BOOL,
  BARE_OPT_STRING,
  BARE_OPT_INT,
} bare_opt_type_t;

typedef struct {
  char short_name;
  const char *long_name;
  const char *help;
  bare_opt_type_t type;
  union {
    bool *bval;
    char **sval;
    long *ival;
  } out;
  bool required;
} bare_opt_t;

typedef struct {
  const char *prog;
  const char *usage;
  const char *desc;
  bare_opt_t *opts;
  size_t nopts;
  char **positional;
  size_t npositional;
} bare_cli_t;

void bare_cli_init(bare_cli_t *cli, const char *prog, const char *usage,
                   const char *desc);
void bare_cli_add_opt(bare_cli_t *cli, bare_opt_t opt);
int bare_cli_parse(bare_cli_t *cli, int argc, char **argv);
void bare_cli_help(const bare_cli_t *cli);
void bare_cli_free(bare_cli_t *cli);
