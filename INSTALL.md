# Prerequisites

The following software packages are required to build cppTango:

- A C++14 compliant compiler like GCC, clang or Visual Studio (2015 or newer)
- [cmake](https://cmake.org), 3.7 or newer
- [tango-idl](https://gitlab.com/tango-controls/tango-idl)
- [omniorb](http://omniorb.sourceforge.net), 4.2.1 or newer
- [libzmq](https://github.com/zeromq/libzmq), 4.0.5 or newer
- [cppzmq](https://github.com/zeromq/cppzmq), 4.7.1 or newer

In the following we assume a linux-based system, see [here](#building-on-windows) for building on Windows.

On current debian systems the dependencies, except tango-idl, are available as distribution packages:

```bash
sudo apt install cmake build-essential git libcos4-dev libomniorb4-dev libomnithread4-dev libzmq3-dev omniidl python3
```

If your linux does not have precompiled packages for these dependencies jump to the
[next](#compiling-the-dependencies) section for compilation instructions.

## Compiling tango

## tango-idl

```bash
git clone https://gitlab.com/tango-controls/tango-idl
cd tango-idl
mkdir build
cd build
cmake ..
// no make required
sudo make install
```

## cppTango

```bash
git clone https://gitlab.com/tango-controls/cppTango
cd cppTango
mkdir build
cd build
cmake ..
make [-j NUMBER_OF_CPUS]
sudo make install
```

## CMake Variables

The following variable can be passed to cmake to tweak compilation. The general syntax is `-D$var=$value`.

On linux-based systems all dependencies are searched via `pkg-config`.
Therefore you can also pass additional search locations via the
`PKG_CONFIG_PATH` environment variable as in

```
PKG_CONFIG_PATH="/usr/local/libzmq:/usr/local/omniORB" cmake ..
```

<!-- Keep the variable list sorted -->

| Variable name                |  Default value                         | Description
|------------------------------|----------------------------------------|--------------------------------------------------------------------------------------------------
| `BUILD_SHARED_LIBS`          | `ON`                                   | Build tango as shared library, `OFF` creates a static library
| `BUILD_TESTING`              | `ON`                                   | Build the test suite
| `CMAKE_BUILD_TYPE`           | `Release`                              | Compilation type, can be `Release` or `Debug`
| `CMAKE_INSTALL_PREFIX`       | `/usr/local` or `C:/Program Files`     | Desired install path
| `CMAKE_VERBOSE_MAKEFILE`     | `OFF`                                  | Allows to increase the verbosity level with `ON`
| `COVERALLS`                  | `OFF`                                  | Generate code coverage data
| `CPPZMQ_BASE`                |                                        | cppzmq installed path
| `IDL_BASE`                   |                                        | tangoidl installed path
| `OMNI_BASE`                  |                                        | omniORB4 installed path
| `TANGO_ENABLE_COVERAGE`      | `OFF`                                  | Instrument code for coverage analysis (Requires CMake 3.13+)
| `TANGO_ENABLE_SANITIZER`     | *empty*                                | Compile with sanitizers, one of: `ASAN`, `TSAN`, `UBSAN` or `MSAN` (Requires CMake 3.13+ and Clang/GCC)
| `TANGO_JPEG_MMX`             | `ON`                                   | Build the jpeg support with MMX extensions (32bit Linux only)
| `TANGO_INSTALL_DEPENDENCIES` | `OFF`                                  | Install dependencies of tango as well (Windows only)
| `TANGO_USE_USING_NAMESPACE`  | `ON`                                   | `ON` will include the `std` namespace in tango headers. Choose `OFF` for modern builds.
| `USE_PCH`                    | `ON`                                   | Use precompiled headers (makes compilation much faster)
| `WARNINGS_AS_ERRORS`         | `OFF`                                  | Treat compiler warnings as errors
| `ZMQ_BASE`                   |                                        | libzmq installed path

cppTango supports unity builds to speed up the compilation. Please see the
[related CMake documentation](https://cmake.org/cmake/help/latest/prop_tgt/UNITY_BUILD.html)
for more details on how to enable and configure this feature (note that CMake
3.16 or better is required).

## Cross-compiling tango

For compiling tango for a different architectures than the host architecture, it is
advisable to cross compile tango. We demonstrate the approach by compiling for
32-bit on a 64-bit linux. See [here](https://wiki.debian.org/Multiarch/HOWTO)
for the generic debian howto.

```bash
sudo dpkg --add-architecture i386
sudo apt update
sudo apt remove libcos4-dev libomniorb4-dev libomnithread4-dev libzmq3-dev
sudo apt install gcc-multilib g++-multilib gcc-i686-linux-gnu
sudo apt install libcos4-dev:i386 libomniorb4-dev:i386 libomnithread4-dev:i386 libzmq3-dev:i386
mkdir build-cross-32bit
cd build-cross-32bit
cmake -DCMAKE_TOOLCHAIN_FILE=..\configure\toolchain-i686.cmake ..
make [-j NUMBER_OF_CPUS]
```

cmake should output `Target platform: Linux 32-bit`. You can also inspect the
created library using `file` to check that you built it correctly.

# Compiling the dependencies

We assume that you have a compiler already and are on a linux based system.
Additionally the tools git, wget, tar and bzip2 are required.

## CMake

```bash
git clone https://github.com/Kitware/CMake cmake
cd cmake
git checkout v3.12.4
mkdir build
cd build
../bootstrap
make [-j NUMBER_OF_CPUS]
sudo make install
```

## libzmq

```bash
git clone https://github.com/zeromq/libzmq
cd libzmq
git checkout v4.2.0
mkdir build
cd build
cmake -DENABLE_DRAFTS=OFF -DWITH_DOC=OFF -DZMQ_BUILD_TESTS=OFF ..
make [-j NUMBER_OF_CPUS]
sudo make install
```

## cppzmq

```bash
git clone https://github.com/zeromq/cppzmq
cd cppzmq
git checkout v4.7.1
mkdir build
cd build
cmake -DCPPZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local ..
make [-j NUMBER_OF_CPUS]
sudo make install
```

## omniORB4

```bash
wget -L https://sourceforge.net/projects/omniorb/files/omniORB/omniORB-4.2.4/omniORB-4.2.4.tar.bz2/download -O omniORB-4.2.4.tar.bz2
tar xjf omniORB-4.2.4.tar.bz2
cd omniORB-4.2.4
./configure
make [-j NUMBER_OF_CPUS]
sudo make install
```

Now with all these dependencies installed the cmake invocation to compile cppTango looks like

```bash
cd cppTango/build
cmake ..
```

if cmake does not find some of the dependencies, you can either add a custom `PKG_CONFIG_PATH` environment variable with

```bash
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig cmake ..
```

or use the the CMAKE variables `ZMQ_BASE`, `CPPZMQ_BASE`, `IDL_BASE`, `OMNI_BASE` from [here](#cmake-variables).

# Using pkg-config in packages requiring tango

Once installed cppTango provides
[pkg-config](https://en.wikipedia.org/wiki/Pkg-config) file `tango.pc`. One can
use it to resolve libtango dependencies:

```cmake
include(FindPkgConfig)
pkg_search_module(TANGO_PKG REQUIRED tango)

if(NOT TANGO_PKG_FOUND)
  message(FATAL_ERROR "Could not find tango")
endif()

link_directories(${TANGO_PKG_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TANGO_PKG_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++11)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${TANGO_PKG_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} PUBLIC ${TANGO_PKG_LIBRARIES})
```

`tango.pc` provides default installation directory for all Tango devices linked against this libtango:

```bash
pkg-config --variable=tangodsdir tango
/usr/bin
```
# Running the tests

Running the tests requires docker. See
[here](https://docs.docker.com/engine/install) for install instructions.

Once you have installed docker, you can run the tests from `build/` via:

```bash
make test
```

## Run individual tests

For debugging a single test execute the following:

```
ctest -R event::per_event --output-on-failure -V
```

Test output and device server logs are collected in `build/cpp_test_suite/test_results`.

For more details on testing with CTest, see [here](https://cmake.org/Wiki/CMake/Testing_With_CTest).

## Setting environment up manually

The test runner automatically starts database and all required
device servers for each test. If you want to set up the environment
manually, from `build/` directory run:

```bash
source ./cpp_test_suite/environment/setup_database.sh  # source to get TANGO_HOST
./cpp_test_suite/environment/setup_devices.sh
# attach the debugger or perform some additional configuration
env TANGO_TEST_CASE_SKIP_FIXTURE=1 ctest -V -R ds_cache
killall DevTest FwdTest
docker stop tango_cs mysql_db
```

## Using Valgrind in tests

It is possible to run the test programs and test device servers under Valgrind.
This is controlled with a number of environment variables:

* `TANGO_TEST_CASE_VALGRIND_CLIENT`: if set, Valgrind will be used to run the
  test program,
* `TANGO_TEST_CASE_VALGRIND_SERVER`: if set, Valgrind will be used to run the
  test device server(s),
* `TANGO_TEST_CASE_VALGRIND_OPTIONS`: if set, overrides default flags passed
  to Valgrind.

For example following command can be used to run device servers under Valgrind:

```bash
env TANGO_TEST_CASE_VALGRIND_SERVER=1 ctest -V -R cxx_attr_conf
```

Following command will run the test program under Valgrind, stop on the first
error and wait for attaching a debugger:

```bash
env \
   TANGO_TEST_CASE_VALGRIND_CLIENT=1 \
   TANGO_TEST_CASE_VALGRIND_OPTIONS="--tool=memcheck --leak-check=full --track-origins=yes --vgdb=yes --vgdb-error=0" \
   ctest -V -R cxx_attr_conf
```

# Building on windows

For the majority of users using the prebuilt binaries from the release page is
easier. The following documentation is targeted for developers.

We assume Windows 10 and a Visual Studio 2017 development environment. In
addition python 3.7 must be installed (this is required by omniidl), get it
from [here](https://www.python.org/downloads/release/python-379). You need the
same bitness as you want to compile tango for. And add python to the path
during installation as well.

- Fetch the required dependencies into `c:\projects`

```bat
SET ARCH=x64-msvc15
SET PYVER=py37
https://github.com/tango-controls/omniorb-windows-ci/releases/download/4.2.1-2/omniorb-4.2.1_%ARCH%_%PYVER%.zip
https://github.com/tango-controls/Pthread_WIN32/releases/download/2.9.1/pthreads-win32-2.9.1_%ARCH%.zip
https://github.com/tango-controls/zmq-windows-ci/releases/download/4.0.5/zmq-4.0.5_%ARCH%.zip
git clone --depth 1 https://gitlab.com/tango-controls/tango-idl tango-idl-source
```

- Open a VS 2017 command prompt

- Install tango-idl

```bat
cd tango-idl-source
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX="c:/projects/tango-idl"
cmake --build . --target install
```

- Switch to cppTango directory
- `mkdir build`
- `cd build`
- Configure with

```
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=install -DCMAKE_BUILD_TYPE=Release           \
      -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" -DIDL_BASE="c:/projects/tango-idl"    \
      -DOMNI_BASE="c:/projects/omniorb" -DZMQ_BASE="c:/projects/zeromq" -DCPPZMQ_BASE="c:/projects/zeromq" \
      -DPTHREAD_WIN="c:/projects/pthreads-win32" -DUSE_PCH=OFF -DBUILD_TESTING=OFF ..
```

- Compile with `cmake --build .`
- Install with `cmake --build . --target install`
- You now have a full tango installation in `build/install`

# Generating code coverage report

> *Note: This requires CMake 3.13 or better.*

> *Note: Code coverage reports are currently supported only with GCC or Clang.*

To generate code coverage report one can follow these steps:

1. Enable code coverage support with CMake flag `TANGO_ENABLE_COVERAGE`:
   ```
   cmake -DTANGO_ENABLE_COVERAGE=ON ...
   ```
2. Run any relevant tests that contribute to the code coverage:
   ```
   ctest ...
   ```
3. Generate the report with a tool of choice, e.g. `gcovr` can generate reports
   in many formats. Note that usually one is interested only in the library
   code and may want to exclude any test code from the report.
   Below command can be run from the project's root directory:
   ```
   gcovr --filter '^cppapi/' --filter '^log4tango/(?!tests/)' --html-details --output coverage.html
   ```

# Using clang-tidy for code quality checks

`clang-tidy` can be used to detect potential bugs and code quality issues.
Below are some comments worth noticing:

* to generate the compilation database one could pass the standard
  `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` option when running CMake.
* when using unity build, one must use `header-filter` option to include
  at least the .cpp files, otherwise all warnings will be ignored,
* it is recomended to use `header-filter='.*'` to still see the warnings which
  are not related to any source file,
* default [configuration file](.clang-tidy) contains a list of checks that
  are verified in the CI pipeline.

Example commands to run clang-tidy on all files (excluding the tests):
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_UNITY_BUILD=ON -DCMAKE_UNITY_BUILD_BATCH_SIZE=0 ...
run-clang-tidy.py -header-filter='.*' 'cppapi/(?!server/idl)' 'log4tango/src'
```

# Execucting gitlab CI jobs locally

For faster development it is possible to execute gitlab CI jobs locally on the development machine.

* Install gitlab runner, see https://docs.gitlab.com/runner/install
* `gitlab-runner exec docker --env "MAKEFLAGS=-j NUMBER_OF_CPUS" JOBNAME`
