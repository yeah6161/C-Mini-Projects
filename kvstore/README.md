# KVStore (C)

In-memory key-value store with simple persistence.

## Build
gcc -o kv src/main.c src/kv.c

## Run
./kv
kv> SET foo bar
kv> GET foo
kv> SAVE
kv> EXIT
