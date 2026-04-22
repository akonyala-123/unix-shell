# Unix Shell

A Unix shell built from scratch in C++ supporting pipes, 
I/O redirection, background jobs, and built-in commands.

## Features
- Command execution via fork/exec
- Pipes (ls | grep foo)
- Output redirection (ls > file.txt)
- Input redirection (cat < file.txt)
- Built-in commands (cd, exit)
- Background jobs (sleep 5 &)

## Build and Run
make
./shell

## Key Concepts Learned
- fork/exec pattern — how Unix creates and runs processes
- File descriptors — everything is a file (0=stdin, 1=stdout, 2=stderr)
- dup2 — rewiring file descriptors before exec
- Pipes — kernel-managed buffers connecting process I/O
- I/O redirection — pointing fd 0 and fd 1 at files
- Background jobs — skipping wait() to return immediately
- Built-in commands — why cd and exit must run in the parent process