#include "shwish.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int tty = isatty(STDIN_FILENO);
    char *line = NULL;
    size_t cap = 0;
    int status = 0;

    if (tty)
        signal(SIGINT, SIG_IGN); /* ^C kills the foreground child, not the shell (proper job control: M8) */

    for (;;) {
        if (tty) {
            fputs("shwish$ ", stdout);
            fflush(stdout);
        }
        if (getline(&line, &cap, stdin) < 0)
            break; /* EOF (Ctrl-D) or read error */

        char **argv;
        if (tokenize(line, &argv) < 0) {
            fprintf(stderr, "shwish: syntax error: unterminated quote or trailing backslash\n");
            status = 2;
            continue;
        }
        if (argv[0])
            status = run_command(argv);
        tokens_free(argv);
    }
    if (tty)
        putchar('\n');
    free(line);
    return status;
}
