# Description

This project provides a dynamically linked shared library that hooks into various system calls and functions to log or
alter their behavior.

## Hooks

- `recvmsg`: Logs and monitors the `recvmsg` system call.
    - **Context**: The `recvmsg` system call is used for receiving messages from a socket, and may be used to receive
      data on a socket.
- `recvfrom`: Logs and monitors the `recvfrom` system call.
    - **Context**: The `recvfrom` system call is used to receive messages from a socket, and may be used to receive data
      on a socket. It is generally used with UDP sockets.
- `memcpy`: Logs and monitors the `memcpy` function.
    - **Context**: The `memcpy` function is a standard library function that copies `n` bytes from the object pointed to
      by `src` to the object pointed to by `dest`.

## Building

To build the project, run the following command:

```bash
make
```

This will generate a dynamically linked shared object file `hooks.so` inside the `./lib` directory.

## Usage

To use all the hooks, you'll need to set the `LD_PRELOAD` environment variable to the path of the `hooks.so` file. This
will instruct the loader to load this shared object before any other.

```bash
export LD_PRELOAD=./lib/hooks.so
```

If you wish to use just one specific hook, you can set `LD_PRELOAD` to the path of that individual `.so` file instead.
For example, to use only the `memcpy` hook:

```bash
export LD_PRELOAD=./lib/memcpy.so
```

After setting `LD_PRELOAD`, any new processes you start from the same shell will have the specified hooks enabled.

## Running Tests

To run the tests, execute:

```bash
make tests
```

If all tests pass, you will see a green "All tests passed" message.

## Cleaning Up

To remove all compiled files, including test executables, run:

```bash
make clean
```
