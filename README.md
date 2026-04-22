# Unix Shell

A fully functional Unix shell built from scratch in C++ with no 
external libraries. Supports pipes, I/O redirection, background 
jobs, and built-in commands.

## Features

- **Command execution** — fork/exec pattern to run any Unix command
- **Pipes** — connect commands with | (ls | grep .cpp)
- **Output redirection** — write output to files (ls > output.txt)
- **Input redirection** — read input from files (cat < input.txt)
- **Background jobs** — run commands without waiting (sleep 5 &)
- **Built-in commands** — cd and exit run directly in shell process

## Build and Run

make
./shell

## Usage

mysh> ls -la
mysh> ls | grep .cpp
mysh> ls > output.txt
mysh> cat < output.txt
mysh> sleep 5 &
mysh> cd /tmp
mysh> exit

## Key Concepts Learned

- **fork/exec pattern** — why Unix separates process creation from
  program loading, and why execvp never returns on success
- **File descriptors** — everything is a file (0=stdin, 1=stdout, 
  2=stderr), all I/O is just integers pointing to kernel objects
- **dup2** — rewiring file descriptors before exec to transparently
  redirect any program's input and output
- **Pipes** — kernel managed buffers connecting stdout of one process
  to stdin of another, synchronized automatically
- **I/O redirection** — pointing fd 0 and fd 1 at files instead of
  keyboard and terminal
- **Background jobs** — skipping wait() so shell returns immediately
  while child runs concurrently
- **Built-in commands** — why cd and exit must modify shell process
  state directly and cannot be implemented with fork/exec
- **Copy-on-write** — how fork() efficiently duplicates a process
  without copying all memory immediately