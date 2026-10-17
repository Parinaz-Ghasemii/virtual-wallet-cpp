#!/bin/bash
# Runs every program against the sample inputs in samples/ and compares the output.
# Usage: make test      (or: bash tests/run_tests.sh after `make`)
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/build"; S="$ROOT/samples"
pass=0; fail=0
norm() { awk '{ sub(/[ \t]+$/, ""); print }'; }       # ignore trailing spaces / missing final newline (works on GNU and BSD/macOS)

check() {  # name  actual-output-file  expected-file
    if diff <(norm < "$2") <(norm < "$3") > /dev/null; then echo "PASS  $1"; pass=$((pass+1))
    else echo "FAIL  $1"; diff <(norm < "$2") <(norm < "$3") | head -5; fail=$((fail+1)); fi
}
run() { "$1" < "$2" > /tmp/jibpool_out.$$ 2>&1; check "$3" /tmp/jibpool_out.$$ "$4"; }

for t in s1 s2 s3; do run "$BIN/01_hash_rsa"   "$S/01_hash_rsa/$t.in"   "phase1 / 01 hash+rsa / $t"    "$S/01_hash_rsa/$t.out"; done
for t in s1 s2 s3; do run "$BIN/02_in_account" "$S/02_in_account/$t.in" "phase1 / 02 in-account / $t"  "$S/02_in_account/$t.out"; done
for t in s1 s2; do
    run "$BIN/03_multi_user_transfers" "$S/03_multi_user/$t.in" "phase1 / 03 multi-user (C) / $t"        "$S/03_multi_user/$t.out"
    run "$BIN/oop_wallet"              "$S/03_multi_user/$t.in" "phase2 / oop_wallet (C++) / $t"         "$S/03_multi_user/$t.out"
done

# 04: file persistence (runs in a scratch directory because it writes files)
W=$(mktemp -d); ( cd "$W" && "$BIN/04_file_persistence" < "$S/04_file_persistence/s1.in" > stdout.txt )
check "phase1 / 04 persistence / REPORT output" "$W/stdout.txt" "$S/04_file_persistence/s1.out"
check "phase1 / 04 persistence / report.txt"    "$W/report.txt" "$S/04_file_persistence/s1.report.expected"
rm -rf "$W"

# console app: several separate sessions that share the data files
W=$(mktemp -d); cd "$W"; APP="$BIN/wallet_console_app"
printf 'jibCoin 1\netherion 2\n'                    > currencies.txt
printf 'SARAFI 1000\nalice 100\nbob 200\n'          > jibCoin.txt
printf 'SARAFI 500\nalice 50\nbob 20\n'             > etherion.txt
# hashes of the passwords "admin", "pw", "pw2" (see Hash_func)
printf 'SARAFI JSTHWAAAAAAAAAAA 7 10403\nalice CXAAAAAAAAAAAAAA 7 10403\nbob CXYAAAAAAAAAAAAA 7 10403\n' > users.txt
printf 'Login\nalice\npw\nTRANSFER alice bob jibCoin 20\n'  | "$APP" > /dev/null   # fee 2
printf 'Login\nSARAFI\nadmin\nDEPOSIT alice jibCoin 10\n'    | "$APP" > /dev/null   # allowed
printf 'Login\nalice\npw\nDEPOSIT alice jibCoin 10\n'        | "$APP" > /dev/null   # denied: not the exchange account
printf 'Login\nalice\npw\nWITHDRAW bob jibCoin 5\n'          | "$APP" > /dev/null   # denied: someone else's account
printf 'SARAFI 992\nalice 88\nbob 220\n' > expected.txt
check "phase2 / console app / balances after 4 sessions" jibCoin.txt expected.txt
printf 'alice TRANSFER alice bob jibCoin 20 1\nbob TRANSFER alice bob jibCoin 20 1\nSARAFI DEPOSIT alice jibCoin 10 1\nalice DEPOSIT alice jibCoin 10 0\nalice WITHDRAW bob jibCoin 5 0\n' > expected.txt
check "phase2 / console app / log.txt" log.txt expected.txt
cd "$ROOT"; rm -rf "$W" /tmp/jibpool_out.$$

echo; echo "$pass passed, $fail failed"; [ $fail -eq 0 ]
