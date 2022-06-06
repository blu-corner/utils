# PROJECT MOVED TO [Version1 Org/utils](https://github.com/Version1/utils). THIS REPOSITORY WILL NO LONGER BE MAINTAINED.

# utils [![Build Status](https://travis-ci.com/blu-corner/utils.svg?branch=master)](https://travis-ci.com/blu-corner/utils) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=blu-corner_utils&metric=alert_status)](https://sonarcloud.io/dashboard?id=blu-corner_utils)

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
