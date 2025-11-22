# mt-grep (C, pthreads)

Multi-threaded file search (simple grep) walking directories.

## Build
gcc -o mtgrep src/main.c src/fsearch.c -lpthread

## Run
./mtgrep /path/to/dir "pattern"

