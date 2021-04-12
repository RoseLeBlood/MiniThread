<img title="Mini Thread" alt="Mini Thread" src="images/Logo mnThread.png" width="50%" align="left"></img>
Mini Thread is a library for creating safe tasks, queues and other useful things for the esp32, in c ++.

Many of the standard STL containers are replaced with their own optimized versions. Recognizable by its own namespace mn.
All containers, such as vector, list, queue, rb-tree  can be found in the namespace mn::container.

Various types of association classes are provided for simplicity. This is just the exchange of the used allocator from the system to the Spiram allocator, whereby certain regions can be swapped out to a different storage capacity without having to rewrite the entire program.
Certain allocation in addition to a limit, the bytes used to be limited.
All allocator find you under the namespace mn::memory.

The containers and pointers use as default the mn::memory::default_allocator_t.

For network programming see mn::net. (net/mn_socket.hpp) they contains a asy to use network socket wrapper for lwip,
IPv4 and IPv6 support. Currently only dgram_socket_ip{4,6} and  stream_socket_ip{4,6} support, raw socket are follow in version 2.30, with class for wifi AP and Wifi STA support

For tickhooks please activated CONFIG_FREERTOS_LEGACY_HOOKS in your sdkconfig !!

![Maintaner](https://img.shields.io/badge/maintainer-theMaintainer-blue) [![GPL license](https://img.shields.io/badge/License-LGPL-blue.svg)](LICENSE.md)
[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/RoseLeblood/mnthread/) ![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/roseleblood/MiniThread) ![Lines of code](https://img.shields.io/tokei/lines/github/roseleblood/MiniThread) ![GitHub repo size](https://img.shields.io/github/repo-size/roseleblood/MiniThread)
![Build with PlatformIO](https://img.shields.io/badge/build%20with-PlatformIO-orange?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=)

## Folder Strutur
- include and src:
  - / : all basics: task, semaphores. timer ...
  - memory: mempool handling
  - queue: FreeRTOS queue's and workqueue-engines
  - slock: ystem interrupt, schedular and ...  autolock helper
- doc: Files to create the docu with doxygen
  - The online pre builded version: [https://roseleblood.github.io/mnthread-docs/](https://roseleblood.github.io/mnthread-docs/)
- example; The basic's example, and for more see extra repository: [mnthread-examples](https://github.com/RoseLeBlood/mnthread-examples)

## Using
Build from git from
1. ```sh ./configure or ./configure --prefix=<path> # without prefix then install to /opt```
2. ```sh make build ```
3. ```sh sudo or doas make install ```
4. add  "lib_deps = /opt/miniThread/miniThread-2.*.tar.gz"  to your platformio.ini

### Example
```ini
[env:esp-wrover-kit]
platform = espressif32
board = esp-wrover-kit
framework = espidf
lib_deps = /opt/miniThread/miniThread-2.*.tar.gz

```
## Using from platformio
```ini
# platformio.ini – project configuration file

[env:my_build_env]
platform = espressif32
framework = espidf
lib_deps =
  # RECOMMENDED
  # Accept new functionality in a backwards compatible manner and patches
  roseleblood/mini Thread @ ^2.21.0

  # Accept only backwards compatible bug fixes
  # (any version with the same major and minor versions, and an equal or greater patch version)
  roseleblood/mini Thread @ ~2.21.0

  # The exact version
  roseleblood/mini Thread @ 2.21.0

```

## ChangeLog

For more see [ChangeLog](ChangeLog.md)


## Contributing

We would love for you to contribute to Mini Thread and help make it even better than it is today! See our [Contributing Guidelines](CONTRIBUTING.md) for more information.

Contributors are required to abide by our [Code of Conduct](CODE_OF_CONDUCT.md) and [Coding Conventions and Style Guide](STYLE_GUIDE.md).

# Versioning

Mini Thread follows the [Semantic Versioning guidelines](http://semver.org/) for release cycle transparency and to maintain backwards compatibility.

## License

Mini Thread is released under the [LGPL](LICENSE) file for more information.
