#include "shwish.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* Builtins run in the shell process: cd/export change the shell's own state,
 * which a forked child could never do. */

static int bi_cd(char **argv)
{
    const char *dir = argv[1] ? argv[1] : getenv("HOME");
    char cwd[PATH_MAX];

    if (argv[1] && argv[2]) {
        fprintf(stderr, "shwish: cd: too many arguments\n");
        return 1;
    }
    if (!dir) {
        fprintf(stderr, "shwish: cd: HOME not set\n");
        return 1;
    }
    if (chdir(dir) < 0) {
        fprintf(stderr, "shwish: cd: %s: %s\n", dir, strerror(errno));
        return 1;
    }
    if (getcwd(cwd, sizeof cwd))
        setenv("PWD", cwd, 1);
    return 0;
}

static int bi_pwd(char **argv)
{
    char cwd[PATH_MAX];
    (void)argv;
    if (!getcwd(cwd, sizeof cwd)) {
        fprintf(stderr, "shwish: pwd: %s\n", strerror(errno));
        return 1;
    }
    puts(cwd);
    return 0;
}

static int bi_export(char **argv)
{
    int rc = 0;
    for (char **a = argv + 1; *a; a++) {
        char *eq = strchr(*a, '=');
        if (eq == *a) {
            fprintf(stderr, "shwish: export: `%s': not a valid identifier\n", *a);
            rc = 1;
        } else if (eq) {
            *eq = '\0';
            if (setenv(*a, eq + 1, 1) < 0) {
                fprintf(stderr, "shwish: export: %s\n", strerror(errno));
                rc = 1;
            }
            *eq = '=';
        }
        /* `export NAME` with no value: nothing to do until shell variables exist */
    }
    return rc;
}

static int run_external(char **argv)
{
    pid_t pid = fork();
    int status;

    if (pid < 0) {
        fprintf(stderr, "shwish: fork: %s\n", strerror(errno));
        return 1;
    }
    if (pid == 0) {
        signal(SIGINT, SIG_DFL); /* the shell ignores it; exec would inherit SIG_IGN */
        execvp(argv[0], argv);
        fprintf(stderr, "shwish: %s: %s\n", argv[0], strerror(errno));
        _exit(errno == ENOENT ? 127 : 126); /* _exit: don't flush the parent's stdio copy */
    }
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) {
            fprintf(stderr, "shwish: waitpid: %s\n", strerror(errno));
            return 1;
        }
    }
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);
    return 1;
}

int run_command(char **argv)
{
    static int last_status; /* what `exit` with no argument returns */

    if (!argv[0])
        return last_status;
    if (strcmp(argv[0], "exit") == 0) {
        int code = last_status;
        if (argv[1]) {
            char *end;
            long n = strtol(argv[1], &end, 10);
            if (*end || end == argv[1]) {
                fprintf(stderr, "shwish: exit: %s: numeric argument required\n", argv[1]);
                code = 2;
            } else {
                code = (int)(n & 0xff);
            }
        }
        tokens_free(argv);
        exit(code);
    }
    if (strcmp(argv[0], "cd") == 0)
        last_status = bi_cd(argv);
    else if (strcmp(argv[0], "pwd") == 0)
        last_status = bi_pwd(argv);
    else if (strcmp(argv[0], "export") == 0)
        last_status = bi_export(argv);
    else
        last_status = run_external(argv);
    return last_status;
}
