#!/bin/sh
# Feed the same script to shwish and $REF (default dash); stdout and exit status must match.
# stderr is ignored: error message wording differs between shells.
cd "$(dirname "$0")/.." || exit 1
REF=${REF:-dash}
command -v "$REF" >/dev/null || REF=sh # dash is the real reference; install it for stricter results
fail=0
n=0
while IFS= read -r script; do
    n=$((n + 1))
    want=$(printf '%b\n' "$script" | "$REF" 2>/dev/null; echo "rc=$?")
    got=$(printf '%b\n' "$script" | ./shwish 2>/dev/null; echo "rc=$?")
    if [ "$want" != "$got" ]; then
        echo "DIFF: $script"; echo "  ref:    $want"; echo "  shwish: $got"
        fail=1
    fi
done <<'CASES'
echo hello world
echo 'a   b' "c   d"
echo a\ b
true
false
exit 3
nosuchcmd_xyz
sh -c 'exit 5'
echo "it's"\necho 'say "hi"'
cd /tmp\npwd
cd /nonexistent
export FOO=bar\nsh -c 'echo $FOO'
sh -c 'kill -TERM $$'
/etc/passwd
CASES
[ $fail = 0 ] && echo "compare: $n cases match $REF"
exit $fail
