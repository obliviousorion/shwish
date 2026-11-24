#ifndef SHWISH_H
#define SHWISH_H

/*
 * Split one command line into words following POSIX quoting (XCU 2.2):
 * unquoted whitespace separates words; '...' is literal; "..." keeps
 * everything literal except \\ \" \$ \` (backslash escapes only those);
 * an unquoted backslash escapes the next character. "" yields an empty word.
 *
 * On success returns 0 and sets *out to a NULL-terminated array owned by the
 * caller (release with tokens_free). Returns -1 and sets *out to NULL on an
 * unterminated quote or trailing backslash. *out is never partially owned.
 */
int tokenize(const char *line, char ***out);
void tokens_free(char **argv);

/* Run argv[0] (builtin or external); returns its exit status as in $?. */
int run_command(char **argv);

#endif
