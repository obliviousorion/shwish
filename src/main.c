#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    int interactive = isatty(STDIN_FILENO);

    for (;;) {
        if (interactive) {
            fputs("shwish$ ", stdout);
            fflush(stdout);
        }

        ssize_t length = getline(&line, &capacity, stdin);
        if (length < 0) {
            break;
        }

        /* Command parsing and execution will be added here. */
    }

    free(line);
    return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
}