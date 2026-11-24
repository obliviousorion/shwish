# shwish

`shwish` is a small POSIX-ish shell written in C. The project has completed its first
four milestones:

- interactive REPL with a tty-only prompt and clean EOF handling
- whitespace tokenization with single quotes, double quotes, and backslash escapes
- external command execution via `fork`/`execvp`/`waitpid`, including exit status handling
- the in-process builtins `cd`, `pwd`, `export`, and `exit`

Redirection, pipelines, `;`/`&&`/`||`, grouping, job control, and `$VAR` expansion
are not implemented yet.

## Requirements

- A C compiler such as GCC or Clang
- GNU Make
- A POSIX-compatible environment

## Build and run

```sh
make
./shwish
```

The prompt is printed only on a terminal; Ctrl-D exits.

## Test

```sh
make test
```

This builds and runs the tokenizer unit tests with AddressSanitizer and UndefinedBehaviorSanitizer,
then compares a set of shell scripts against `dash` (or `sh` as a fallback).

Use `make clean` to remove generated files.

## Project layout

```text
src/       C implementation (REPL, tokenizer, command execution)
include/   shared declarations
 tests/     tokenizer unit tests and dash comparison script
 docs/      roadmap, design notes, and learning log
```

## Current milestone

M5 Redirection: implement `<`, `>`, `>>`, `2>`, and `2>&1` using `open`/`dup2`.

The development direction is tokenization, parsing, built-ins, process execution,
redirection, pipelines, and job control. Keep POSIX shell behavior and portability
explicit as features are introduced.

## References

- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html)
- [dash source repository](https://git.kernel.org/pub/scm/utils/dash/dash.git/)
- [BusyBox ash source](https://git.busybox.net/busybox/tree/shell/ash.c)
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)