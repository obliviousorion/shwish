# shwish

`shwish` is a small shell written in C. The project is at its starting point:
it currently builds an interactive input loop, while parsing and command
execution remain to be implemented.

## Requirements

- A C compiler such as GCC or Clang
- GNU Make
- A POSIX-compatible environment

## Build and run

```sh
make
./shwish
```

Run `make clean` to remove generated files. The starter prints a prompt when
connected to a terminal and reads input until end-of-file. It does not execute
commands yet.

## Project layout

```text
src/       C implementation
```

Add `include/` for shared headers and `tests/` as the implementation grows.

## Development direction

Useful milestones for growing the shell are tokenization, parsing, built-ins,
process execution, pipelines, redirection, and job control. Keep POSIX shell
behavior and portability goals explicit as these features are introduced.

## References

- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html)
- [dash source repository](https://git.kernel.org/pub/scm/utils/dash/dash.git/)
- [BusyBox ash source](https://git.busybox.net/busybox/tree/shell/ash.c)
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)