# utils

This module contains a collection of utility functions:

* [utils](src/utils.h) - string/number processing
* [dl](src/dl.h) - shared object loading
* [compat](src/compat.h) - platform compatibility
* [common](src/common.h) - some common functions and defines

## Getting Started

To compile the installation:

```bash
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake -DTESTS=ON ../
$ make
$ make install
```

## Running the Tests

To run the unit tests:

```bash
$ make test
```
