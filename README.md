![.github/workflows/build.yml](https://github.com/berkeley-abc/abc-zz/workflows/.github/workflows/build.yml/badge.svg)

The ZZ framework uses CMake to build the system.

- Create a new directory 'build' under the abc-zz main directory

  mkdir build
  cd build

- Run CMake

  cmake ..

- Build the target you are interested in (e.g. bip.exe)

  make bip.exe (builds the bip executable)
  make Bip (build the Bip library and its dependencies)
  make Bip-pic (build the Bip library and its dependencies with -fPIC)
  make Bip-exe (build all exectuables in module Bip)

  make zz_all (builds all libraries and executables)
  make zz_pic (build all libraries with -fPIC)
  make zz_static (build all static libraries)


Build ZZ requires the following:

- CMake version 3.8 or above
- Python 2.7s (or later)
- Developer header files and libraries for 'zlib' (e.g. zlib1g-dev package on Ubuntu)

Recommended:

- GNU Readline developer header files and libraries (e.g. readline-dev on Ubuntu)
- libpng developer header files and libraries (e.g. libpng12-dev on Ubuntu)


## Windows

Building on Windows is more complicated. The simplest way is to use `vcpkg` to gather the dependencies.

### Vcpkg

Clone the repository

    git clone https://github.com/Microsoft/vcpkg

Change into the `vcpkg` directory, and bootstrap

    cd vcpkg
    ./bootstrap-vcpkg.bat

Install the relevant pacakges

    ./vcpkg.exe install dirent:x64-windows-static-md zlib:x64-windows-static-md

Create a build directory 

    cd ../
    mkdir build
    cd build

Configure

    cmake \
        -G "Visual Studio 15 2017 Win64" \
        -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DVCPKG_TARGET_TRIPLET=x64-windows-static-md \
        \
        .. 

Build

    cmake --build .

## Rlive Implementation Progress

This repository currently contains a partial implementation of the **rlive**
algorithm described in "Avoiding the Shoals – A New Approach to Liveness
Checking" (Xia *et al.*, CAV 2024).  The following components are available:

* **SAT query helpers**: the `Pdr2` engine now exposes `approxPre` and
  `pruneDead` which over‑approximate successor states and detect dead states by
  querying the solver with primed assumptions.  These are used to build the
  nested DFS required by rlive.
* **Command‑line support**: a new liveness engine `rlive` can be selected from
  the main `bip` executable.  The recursion depth is controlled with the
  `-rlive-depth` option.
* **Examples and tests**: `Main_pdr2_pre_test.cc` demonstrates calling the new
  helpers on an 8‑bit counter and runs a small bounded DFS starting from the
  initial state.
* **Experimental search utilities**: `dfsExploreRlive` and the `rlive` engine
  perform a bounded depth‑first search using these helpers.
* **Verbose tracing**: the DFS now reports its depth, detected cycles, and the
  shoal cubes added during exploration, aiding in debugging.

The implementation now follows Algorithm 2.  Shoals discovered during the DFS
are fed back to the underlying IC3 engine through additional blocking clauses
so that exploration continues until either an inductive invariant is found or a
lasso‑shaped counterexample is produced.

