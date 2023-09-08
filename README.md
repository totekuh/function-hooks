## Description

This project provides a dynamically linked shared library designed to hook into various system calls and functions for logging or altering their behavior.

It is particularly useful for security research, debugging, and monitoring system interactions.

Keep in mind that you can only hook functions that are dynamically linked to your target. 
If the program has been compiled statically, the hooking won't work.

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
- `getenv`: Logs and monitors the `getenv` function.
  - **Context**: Standard library function for fetching environment variables, logs the variable name and its value if it exists.


## Building

To build the project, navigate to the project directory and run:

```bash
make
```

This will generate a `.so` (shared object) file for each hook and a `hooks.so` file containing all hooks, located in
the `./lib` directory.

## Usage

To use a hook, set `LD_PRELOAD` to the path of that individual `.so` file. For example, for `memcpy`:

```bash
export LD_PRELOAD=./lib/memcpy.so
```

After setting `LD_PRELOAD`, any new processes you start from the same shell will have the hooks enabled.

### memcpy Hook

The `memcpy` hook offers the following environment variables that users can utilize to further control its behaviors:

- `BREAK`: When set to `1`, this variable enables a pause in the hook code, allowing the program to halt and wait for user input after specific logging operations. This can be useful for detailed inspection and analysis.
- `EGG`: Setting the `EGG` variable to a specific string will prompt the hook to log and match instances where that string is found in the source or destination memory during `memcpy` operations.

Example of using `BREAK` and `EGG` with the `memcpy` hook:

```bash
export BREAK=1  # Enable pausing
export EGG="password"  # Log when the word "secret" is found during memcpy
export LD_PRELOAD=./lib/memcpy.so  # Load the memcpy hook
```

### getenv Hook

The `getenv` hook logs whenever the `getenv` function is called. It will log the name of the environment variable being fetched and its value if it exists.


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

An example with `curl`:

```bash
$ LD_PRELOAD=./lib/connect.so curl --head example.com
[ :::::::::::::: Start of connect Hook :::::::::::::: ]
HOOK: connect hooked!
HOOK: Connecting to UNIX socket: /run/systemd/resolve/io.systemd.Resolve
HOOK: Connection successful.
[ :::::::::::::: End of connect Hook :::::::::::::: ]
[ :::::::::::::: Start of connect Hook :::::::::::::: ]
HOOK: connect hooked!
HOOK: Connecting to IPv6 address: 2606:2800:220:1:248:1893:25c8:1946, Port: 80
HOOK: Connection in progress (non-blocking).
[ :::::::::::::: End of connect Hook :::::::::::::: ]
HTTP/1.1 200 OK
Content-Encoding: gzip
Accept-Ranges: bytes
Age: 230203
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Date: Fri, 08 Sep 2023 17:28:08 GMT
Etag: "3147526947"
Expires: Fri, 15 Sep 2023 17:28:08 GMT
Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
Server: ECS (dcb/7EA2)
X-Cache: HIT
Content-Length: 648
```

An example with `ssh-add`:

```bash
$ LD_PRELOAD=./lib/connect.so ssh-add -l
[ :::::::::::::: Start of connect Hook :::::::::::::: ]
HOOK: connect hooked!
HOOK: Connecting to UNIX socket: /tmp/ssh-R7FdNcPW1MkZ/agent.86904
HOOK: Connection successful.
[ :::::::::::::: End of connect Hook :::::::::::::: ]
The agent has no identities.
```

An example with `getenv`:

```bash
$ LD_PRELOAD=./lib/getenv.so ssh-add -l
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: OPENSSL_ia32cap
HOOK: Environment variable not found.
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: OPENSSL_ia32cap
HOOK: Environment variable not found.
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: SSH_AUTH_SOCK
HOOK: Value of environment variable: /tmp/ssh-R7FdNcPW1MkZ/agent.86904
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: SSH_SK_PROVIDER
HOOK: Environment variable not found.
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: POSIXLY_CORRECT
HOOK: Environment variable not found.
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
The agent has no identities.
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: SSH_AUTH_SOCK
HOOK: Value of environment variable: /tmp/ssh-R7FdNcPW1MkZ/agent.86904
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
```

An example of using `getenv` hook to bypass `LD_PRELOAD` protections, the hook tries to avoid detection by hiding the presence of the `LD_PRELOAD` variable from other libraries:

```bash
$ cat testapp.c
//
// Created by totekuh on 9/8/23.
//

#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("Doing some important stuff...\n");
    printf("Gonna check if LD_PRELOAD is there...\n");
    char *var = getenv("LD_PRELOAD");
    printf("LD_PRELOAD is: '%s'\n", var);

    printf("Quitting...\n");
    return 0;
}
$ gcc -o testapp testapp.c
$ LD_PRELOAD=./lib/getenv.so ./testapp
Doing some important stuff...
Gonna check if LD_PRELOAD is there...
[ :::::::::::::: Start of getenv Hook :::::::::::::: ]
HOOK: getenv hooked!
HOOK: Fetching environment variable: LD_PRELOAD
HOOK: LD_PRELOAD query detected. Pretending it's not defined.
[ :::::::::::::: End of getenv Hook :::::::::::::: ]
LD_PRELOAD is: '(null)'
Quitting...
$
```

## Building for Different Architectures

This project can be configured to build shared libraries compatible with different architectures. The configuration options are available through a menu-driven interface, which can be accessed by running:

```bash
make menuconfig
```

### Configuring the Build

The make menuconfig command will execute the config.sh script and present you with a menu to choose the architecture for which you want to build the hooks. The available options are:

- **x86_64**: For 64-bit x86 CPUs
- **x86**: For 32-bit x86 CPUs
- **ARM64**: For 64-bit ARM CPUs
- **ARM**: For 32-bit ARM CPUs
- **ARMHF**: For ARM CPUs with hardware floating-point support

The script will also check and install any missing dependencies required for the chosen architecture.
