SlimcoinMiner-OpenCL
==============

This is a multi-threaded slimcoin solo OpenCL/CPU miner,
fork of [pooler](//github.com/pooler)'s cpuminer.

#### Table of contents

* [Algorithms](#algorithms)
* [Dependencies](#dependencies)
* [Download](#download)
* [Build](#build)
* [Usage instructions](#usage-instructions)
* [License](#license)

Algorithms
==========
 * ✓ __dcrypt__ (Slimcoin [SLM])

Dependencies
============
* libcurl			http://curl.haxx.se/libcurl/
* jansson			http://www.digip.org/jansson/ (jansson is included in-tree)
* openssl           https://www.openssl.org/
* opencl

Build
=====

Important: edit 1st line of Makefile.am if you have OpenCL headers in different directory.

#### Basic *nix build instructions:
Install OpenCL SDK (AMD or NVidia)

```sh
./autogen.sh	# only needed if building from git repo
./nomacro.pl	# only needed if building on Mac OS X or with Clang
./configure CFLAGS="-march=native"
# Use -march=native if building for a single machine
make
```

#### Notes for AIX users:
 * To build a 64-bit binary, `export OBJECT_MODE=64`
 * GNU-style long options are not supported, but are accessible via configuration file

#### Basic Windows build instructions, using MinGW:
 * Install MinGW and the MSYS Developer Tool Kit (http://www.mingw.org/)
   * Make sure you have mstcpip.h in MinGW\include
 * If using MinGW-w64, install pthreads-w64
 * Install libcurl devel (http://curl.haxx.se/download.html)
   * Make sure you have libcurl.m4 in MinGW\share\aclocal
   * Make sure you have curl-config in MinGW\bin
 * Install openssl devel (https://www.openssl.org/related/binaries.html)
 * In the MSYS shell, run:
```sh
./autogen.sh	# only needed if building from git repo
LIBCURL="-lcurldll" ./configure CFLAGS="-march=native"
# Use -march=native if building for a single machine
make
```

#### Basic cross-compile instructions, compiling for win64 on Linux Fedora:
```sh
yum install mingw\*
./autogen.sh    # only needed if building from git repo
./configure CC=x86_64-w64-mingw32-gcc RANLIB=x86_64-w64-mingw32-ranlib --target x86_64-w64-mingw32 
make
```

Usage instructions
==================

Copy dcrypt.cl kernel file to executable's directory.

To mine on all available GPUs:
```
minerd.exe -G -o http://localhost:41683 -u x -p x
```

Run "minerd --help" to see all options.

License
=======
GPLv2.  See COPYING for details.
