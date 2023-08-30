## Description

This project provides a dynamically linked shared library designed to hook into various system calls and functions for
logging or altering their behavior.
It is particularly useful for security research, debugging, and monitoring system interactions.

## Hooks

Currently, the library provides hooks for the following system calls and functions:

- `recvmsg`: Logs and monitors the `recvmsg` system call.
    - **Context**: Commonly used in TCP/IP networking to receive messages from a socket.
- `recvfrom`: Logs and monitors the `recvfrom` system call.
    - **Context**: Generally used with UDP sockets to receive messages from a socket.
- `memcpy`: Logs and monitors the `memcpy` function.
    - **Context**: Standard library function for memory copying.
- `connect`: Logs and monitors the `connect` system call.
    - **Context**: Used for establishing a connection to a network service, logs the socket type and destination.

## Building

To build the project, navigate to the project directory and run:

```bash
make
```

This will generate a `.so` (shared object) file for each hook and a `hooks.so` file containing all hooks, located in
the `./lib` directory.

## Usage

To use all the hooks, set the `LD_PRELOAD` environment variable to the path of the `hooks.so` file:

```bash
export LD_PRELOAD=./lib/hooks.so
```

To use a specific hook, set `LD_PRELOAD` to the path of that individual `.so` file. For example, for `memcpy`:

```bash
export LD_PRELOAD=./lib/memcpy.so
```

After setting `LD_PRELOAD`, any new processes you start from the same shell will have the hooks enabled.

## Running Tests

To run the tests, execute:

```bash
make tests
```

If all tests pass, you will see a green "All tests passed" message.

## Cleaning Up

To remove all compiled files and test executables, run:

```bash
make clean
```

### Testing with Real-world Applications

You can test the hooks with real-world applications that use the hooked system calls. For example:

- Use `curl` to test `recvmsg` and `connect` hooks.
- Use `ssh` or `ssh-agent` to test `recvfrom` hooks.
- Use `docker` commands to test `connect` hooks with Unix Domain Sockets.

An example with `ssh-add`:

```bash
$ LD_PRELOAD=/tmp/connect.so ssh-add -l
[ :::::::::::::: Start of connect Hook :::::::::::::: ]
HOOK: connect hooked!
HOOK: Call stack:
  ./lib/connect.so(connect+0x5f) [0x7f13a5041238]
  ssh-add(+0x1ea96) [0x55a03828ea96]
  ssh-add(+0x8318) [0x55a038278318]
  /lib/x86_64-linux-gnu/libc.so.6(+0x276ca) [0x7f13a48456ca]
  /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0x85) [0x7f13a4845785]
  ssh-add(+0x9581) [0x55a038279581]
HOOK: Connecting to UNIX socket: /tmp/ssh-S7fw3AkAWHr1/agent.708428
HOOK: Connection successful.
[ :::::::::::::: End of connect Hook :::::::::::::: ]
The agent has no identities.
```