#include "shwish.h"

#include <stdlib.h>
#include <string.h>

struct vec {
    char **v;
    size_t n, cap;
};

static int vec_push(struct vec *a, char *s)
{
    if (a->n + 2 > a->cap) { /* room for s and the final NULL */
        size_t cap = a->cap ? a->cap * 2 : 8;
        char **p = realloc(a->v, cap * sizeof *p);
        if (!p)
            return -1;
        a->v = p;
        a->cap = cap;
    }
    a->v[a->n++] = s;
    a->v[a->n] = NULL;
    return 0;
}

void tokens_free(char **argv)
{
    if (!argv)
        return;
    for (char **p = argv; *p; p++)
        free(*p);
    free(argv);
}

int tokenize(const char *line, char ***out)
{
    struct vec words = {0};
    /* A word is never longer than the whole line, so one buffer suffices. */
    char *buf = malloc(strlen(line) + 1);
    size_t len = 0;
    int in_word = 0;
    enum { PLAIN, SQ, DQ } st = PLAIN;

    *out = NULL;
    if (!buf)
        return -1;

    for (const char *c = line;; c++) {
        if (st == PLAIN && (*c == '\0' || *c == ' ' || *c == '\t' || *c == '\n')) {
            if (in_word) {
                char *w = malloc(len + 1);
                if (!w)
                    goto fail;
                memcpy(w, buf, len);
                w[len] = '\0';
                if (vec_push(&words, w) < 0) {
                    free(w);
                    goto fail;
                }
                len = 0;
                in_word = 0;
            }
            if (*c == '\0')
                break;
            continue;
        }
        if (*c == '\0') /* EOF inside a quote */
            goto fail;

        switch (st) {
        case PLAIN:
            in_word = 1;
            if (*c == '\'') {
                st = SQ;
            } else if (*c == '"') {
                st = DQ;
            } else if (*c == '\\') {
                if (!c[1])
                    goto fail; /* trailing backslash: would need a continuation line */
                buf[len++] = *++c;
            } else {
                buf[len++] = *c;
            }
            break;
        case SQ:
            if (*c == '\'')
                st = PLAIN;
            else
                buf[len++] = *c;
            break;
        case DQ:
            if (*c == '"') {
                st = PLAIN;
            } else if (*c == '\\' && c[1] && strchr("\\\"$`", c[1])) {
                buf[len++] = *++c;
            } else {
                buf[len++] = *c;
            }
            break;
        }
    }

    free(buf);
    if (!words.v) { /* blank line: empty, NULL-terminated array */
        words.v = calloc(1, sizeof *words.v);
        if (!words.v)
            return -1;
    }
    *out = words.v;
    return 0;

fail:
    free(buf);
    tokens_free(words.v);
    return -1;
}
