# TIVIDY_Project

## Team Members


## Design Overview


## Build, Run and Test

### With Docker

```bash
docker compose run tividy # run the program
docker compose run tests # run the test suite
```

First run takes ~2 minutes while cmake is installed in the image

### Locally

Requires `g++` (C++17), `cmake` (3.14+) and `make`. CMake fetches FTXUI (the TUI library) and GoogleTest on the first build; don't install them yourself.

```bash
make # build
make run # build and run
make test # build and run tests
make clean # remove build output
```

### Using the TUI

`make run` opens the full-screen TUI. See [tui/README.md](tui/README.md) for the keys, a demo script, how it is built and the engine changes it needed.

### Documentation

```bash
doxygen Doxyfile # output in docs/html/index.html
```