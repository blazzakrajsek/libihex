# README

`libihex` is a C++ implementation of an Intel Hex record manipulation library.

## Table of Contents

- [README](#readme)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Limitations](#limitations)
  - [Getting `libihex`](#getting-libihex)
  - [Compile and build](#compile-and-build)
  - [Installation](#installation)
  - [Documentation](#documentation)
  - [Localization](#localization)
  - [Usage](#usage)
  - [Contributing](#contributing)
  - [Bug Reports](#bug-reports)
  - [Contact the Authors](#contact-the-authors)
  - [Copyright notice](#copyright-notice)

## Features

`libihex` main features are:

  * C++ 17 implementation
  * Cross-platform support (tested on Linux via `gcc` and on Windows via `mingw-w64`)
  * Unified clean class interface for multi level usage
    * `HexRecord` class for the manipulation of Hex data on the Hex record level
    * `HexSection` class for a group of Hex records
    * `HexGroup` class for a group of Hex sections
    * `HexFile` class extends the HexGroup class with stream and file manipulation functions
    * `HexAddress` class for Hex address calculation and manipulation helper functions
  * Uses `cmake` build generator for build and install
  * Extensive tests via `gtest` framework
  * High line, branch and function code coverage (tested 100% on Linux platform)

## Limitations

The `libihex` library should be feature complete, since it handles and implements
all the official Hex record types. However, it can be extended with any custom or
additional Hex Record types as needed. Everybody is welcome to contribute to `libihex`.
Please, see [`CONTRIBUTING`](CONTRIBUTING.md) for further information.

## Getting `libihex`

Currently no release channels are provisioned.

## Compile and build

Anyone wishing to compile and build `libihex` should clone the git repository at:
  * `https://github.com/blazzakrajsek/libihex`

The project should be configured by the `cmake` build generator where build configuration for
various compilers can be generated. With the generated build configuration it is possible
to compile, build and install `libihex`.

On a Linux system, it can be build and installed using:
```sh
$ make
$ sudo make install
```

On a Windows system, it can be build and installed using:
```sh
$ mingw32-make.exe
$ mingw32-make.exe install
```

## Installation

See the [`INSTALL`](INSTALL.md) file for more information about how to configure
and install `libihex`.

## Documentation

`libihex` currently provides no official documentation besides the documentation
contained in this document and any other documents contained in the repository.
However, C++ headers of the library provide a simple yet effective documentation
about the classes, enums and functions. Tests implemented on the `gtest` framework also
provide an insight on how to use the library and class implementations.

## Localization

`libihex` currenly provides no localization facilities.

## Usage

Compiled and built library can be linked with the application or binary using the library.

## Contributing

Everybody is welcome to contribute to `libihex`. Please, see
[`CONTRIBUTING`](CONTRIBUTING.md) for further information.

## Bug Reports

Bug reports can be sent directly to the author(s) via email.

## Contact the Authors

The author can be contacted via email at zakrajsek dot blaz at gmail com.

## Copyright notice

Copyright (c) 2021-2023  Blaž Zakrajšek

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this library.  If not, see <http://www.gnu.org/licenses/>.
