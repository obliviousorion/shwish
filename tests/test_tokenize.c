#include "../include/shwish.h"

#include <stdio.h>
#include <string.h>

static int fails;

/* Expected words are passed NULL-terminated; expect == NULL means "must be a syntax error". */
static void check(const char *line, const char *const *expect)
{
    char **got;
    int rc = tokenize(line, &got);
    int ok;

    if (!expect) {
        ok = rc < 0 && got == NULL;
    } else {
        ok = rc == 0;
        size_t i = 0;
        for (; ok && expect[i]; i++)
            ok = got[i] && strcmp(got[i], expect[i]) == 0;
        ok = ok && got && !got[i];
    }
    if (!ok) {
        fails++;
        printf("FAIL: [%s]\n", line);
    }
    tokens_free(got);
}

int main(void)
{
    check("", (const char *[]){NULL});
    check("   \t \n", (const char *[]){NULL});
    check("ls -l  /tmp\n", (const char *[]){"ls", "-l", "/tmp", NULL});
    check("echo 'a  b'", (const char *[]){"echo", "a  b", NULL});
    check("echo \"a  b\"", (const char *[]){"echo", "a  b", NULL});
    check("echo a\\ b", (const char *[]){"echo", "a b", NULL});
    check("echo ab'cd'\"ef\"", (const char *[]){"echo", "abcdef", NULL});
    check("echo \"\" x", (const char *[]){"echo", "", "x", NULL});
    check("echo ''", (const char *[]){"echo", "", NULL});
    check("echo '\\n'", (const char *[]){"echo", "\\n", NULL});
    check("echo \"a\\\"b\"", (const char *[]){"echo", "a\"b", NULL});
    check("echo \"a\\nb\"", (const char *[]){"echo", "a\\nb", NULL});
    check("echo \"it's\"", (const char *[]){"echo", "it's", NULL});
    check("echo 'say \"hi\"'", (const char *[]){"echo", "say \"hi\"", NULL});
    check("echo 'oops", NULL);
    check("echo \"oops", NULL);
    check("echo oops\\", NULL);

    puts(fails ? "tokenizer: FAILED" : "tokenizer: all passed");
    return fails != 0;
}
