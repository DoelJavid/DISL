DISL
===============================================================================

DISL is a basic display / input library that allows you to create minimal
displays for just about any platform you can think of. The graphics API is
decoupled from DISL, allowing you to effectively bring your own graphics API.

Currently, only X.org and OpenGL is supported, but support for more platforms
is planned. With this in mind, please stay tuned for future updates.

## How to Build

You must have scons installed before you attempt to build DISL. The remaining
libraries you need depends on your current operating system.

* If you're building on Linux or another X-Window system, you will need to have
XLib (libx11) installed on your system.

When all required libraries are installed, simply run scons like so:

```bash
scons -j[threads]
```

Replace `[threads]` with the number of threads within your computer.

### Build Flags

Optionally, you can run scons with these flags below:

* `--examples`: builds all example projects. Great for testing the library.
* `--release`: builds the library in release mode. This will result in a more
optimized library with the catch that all safety asserts will be omitted,
therefore allowing unsafe operations to be performed.

### Credits

This library was created by DoelJavid under the MIT license.

