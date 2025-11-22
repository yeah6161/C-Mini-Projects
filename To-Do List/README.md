# TODO-CLI (C)

Simple CLI todo manager with plain-text DB.

## Build
gcc -o todo src/main.c src/todo.c

## Run
./todo

Data saved to `todos.db` in text format (`id|title|note|priority|due|done` per line).
