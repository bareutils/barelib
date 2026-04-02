/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#define _POSIX_C_SOURCE 200809L
#include "bare/cli.h"
#include "bare/err.h"
#include "bare/mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bare_cli_init(bare_cli_t *cli, const char *prog, const char *usage,
                   const char *desc) {
  cli->prog = prog;
  cli->usage = usage;
  cli->desc = desc;
  cli->opts = NULL;
  cli->nopts = 0;
  cli->positional = NULL;
  cli->npositional = 0;
}

void bare_cli_add_opt(bare_cli_t *cli, bare_opt_t opt) {
  cli->opts = bare_realloc(cli->opts, (cli->nopts + 1) * sizeof(bare_opt_t));
  cli->opts[cli->nopts++] = opt;
}

static bare_opt_t *find_short(bare_cli_t *cli, char c) {
  for (size_t i = 0; i < cli->nopts; i++)
    if (cli->opts[i].short_name == c)
      return &cli->opts[i];
  return NULL;
}

static bare_opt_t *find_long(bare_cli_t *cli, const char *name) {
  for (size_t i = 0; i < cli->nopts; i++)
    if (cli->opts[i].long_name && strcmp(cli->opts[i].long_name, name) == 0)
      return &cli->opts[i];
  return NULL;
}

static void apply_opt(bare_cli_t *cli, bare_opt_t *opt, const char *val) {
  switch (opt->type) {
  case BARE_OPT_BOOL:
    *opt->out.bval = !*opt->out.bval;
    break;
  case BARE_OPT_STRING:
    if (!val) {
      fprintf(stderr, "%s: option requires an argument: %s\n", cli->prog,
              opt->long_name ? opt->long_name : "(short)");
      exit(1);
    }
    *opt->out.sval = bare_strdup(val);
    break;
  case BARE_OPT_INT:
    if (!val) {
      fprintf(stderr, "%s: option requires an argument: %s\n", cli->prog,
              opt->long_name ? opt->long_name : "(short)");
      exit(1);
    }
    *opt->out.ival = strtol(val, NULL, 10);
    break;
  }
}

int bare_cli_parse(bare_cli_t *cli, int argc, char **argv) {
  int i = 1;
  cli->positional = bare_malloc(sizeof(char *) * (size_t)argc);
  cli->npositional = 0;

  while (i < argc) {
    char *arg = argv[i];
    if (strcmp(arg, "--") == 0) {
      i++;
      while (i < argc)
        cli->positional[cli->npositional++] = argv[i++];
      break;
    }

    if (arg[0] == '-' && arg[1] == '-') {
      char *eq = strchr(arg + 2, '=');
      char *val = NULL;
      char name_buf[256];
      if (eq) {
        size_t nlen = (size_t)(eq - (arg + 2));
        if (nlen >= sizeof(name_buf))
          nlen = sizeof(name_buf) - 1;
        memcpy(name_buf, arg + 2, nlen);
        name_buf[nlen] = '\0';
        val = eq + 1;
      } else {
        strncpy(name_buf, arg + 2, sizeof(name_buf) - 1);
        name_buf[sizeof(name_buf) - 1] = '\0';
      }
      bare_opt_t *opt = find_long(cli, name_buf);
      if (!opt) {
        fprintf(stderr, "%s: unknown option: --%s\n", cli->prog, name_buf);
        exit(1);
      }
      if (!val && opt->type != BARE_OPT_BOOL && i + 1 < argc)
        val = argv[++i];
      apply_opt(cli, opt, val);

    } else if (arg[0] == '-' && arg[1] != '\0') {
      for (int j = 1; arg[j]; j++) {
        bare_opt_t *opt = find_short(cli, arg[j]);
        if (!opt) {
          fprintf(stderr, "%s: unknown option: -%c\n", cli->prog, arg[j]);
          exit(1);
        }
        if (opt->type != BARE_OPT_BOOL) {
          const char *val =
              arg[j + 1] ? &arg[j + 1] : (i + 1 < argc ? argv[++i] : NULL);
          apply_opt(cli, opt, val);
          break;
        }
        apply_opt(cli, opt, NULL);
      }
    } else {
      cli->positional[cli->npositional++] = arg;
    }
    i++;
  }

  for (size_t k = 0; k < cli->nopts; k++) {
    if (cli->opts[k].required) {
      bool present = false;
      if (cli->opts[k].type == BARE_OPT_BOOL)
        present = *cli->opts[k].out.bval;
      else if (cli->opts[k].type == BARE_OPT_STRING)
        present = *cli->opts[k].out.sval != NULL;
      if (!present) {
        fprintf(stderr, "%s: required option missing: --%s\n", cli->prog,
                cli->opts[k].long_name);
        exit(1);
      }
    }
  }

  return 0;
}

void bare_cli_help(const bare_cli_t *cli) {
  fprintf(stdout, "usage: %s\n\n%s\n\noptions:\n", cli->usage, cli->desc);
  for (size_t i = 0; i < cli->nopts; i++) {
    bare_opt_t *o = &cli->opts[i];
    if (o->short_name && o->long_name)
      fprintf(stdout, "  -%c, --%-20s %s\n", o->short_name, o->long_name,
              o->help ? o->help : "");
    else if (o->long_name)
      fprintf(stdout, "      --%-20s %s\n", o->long_name,
              o->help ? o->help : "");
    else
      fprintf(stdout, "  -%c                       %s\n", o->short_name,
              o->help ? o->help : "");
  }

  fprintf(stdout, "\n- barelib v%s\n", BARELIB_VERSION);
}

void bare_cli_free(bare_cli_t *cli) {
  bare_free(cli->opts);
  bare_free(cli->positional);
  cli->opts = NULL;
  cli->positional = NULL;
  cli->nopts = 0;
  cli->npositional = 0;
}
