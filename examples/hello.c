/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include <barelib.h>
#include <stdio.h>
static bool flag_help = false;
int main(int argc, char **argv) {
  bare_cli_t cli;
  char *name = "World";

  bare_cli_init(&cli, "hello", "hello [options]",
                "A simple hello world example.");
  bare_cli_add_opt(&cli, (bare_opt_t){'n',
                                      "name",
                                      "Name to greet",
                                      BARE_OPT_STRING,
                                      {.sval = &name},
                                      false});
  bare_cli_add_opt(&cli, (bare_opt_t){'?',
                                      "help",
                                      "show this help and exit",
                                      BARE_OPT_BOOL,
                                      {.bval = &flag_help},
                                      false});

  if (bare_cli_parse(&cli, argc, argv) != 0) {
    bare_cli_free(&cli);
    return 1;
  }

  if (flag_help) {
    bare_cli_help(&cli);
    bare_cli_free(&cli);
    return 0;
  }

  printf("Hello, %s!\n", name);
  bare_cli_free(&cli);
  return 0;
}
