# shwish

`shwish` is a small shell written in C. The project is at its starting point:
it currently reads lines, tokenizes with POSIX quoting, runs external commands
and the builtins `cd`, `pwd`, `export`, `exit`. Redirection, pipelines,
`;`/`&&`/`||`, job control and `$VAR` expansion are not implemented yet.

## Requirements

- A C compiler such as GCC or Clang
- GNU Make
- A POSIX-compatible environment

## Build and run

```sh
make
./shwish
```

Run `make test` for the tokenizer unit tests (ASan/UBSan) and the diff against a
reference shell (`dash` if installed, else `sh`). Run `make clean` to remove generated files.
The prompt is printed only on a terminal; Ctrl-D exits.

## Project layout

```text
src/       C implementation (main.c REPL, tokenize.c, exec.c)
include/   shared header
tests/     tokenizer unit tests + diff-against-reference script
```

## Development direction

Useful milestones for growing the shell are tokenization, parsing, built-ins,
process execution, pipelines, redirection, and job control. Keep POSIX shell
behavior and portability goals explicit as these features are introduced.

## References

- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html)
- [dash source repository](https://git.kernel.org/pub/scm/utils/dash/dash.git/)
- [BusyBox ash source](https://git.busybox.net/busybox/tree/shell/ash.c)
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)