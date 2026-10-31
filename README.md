# TIVIDY_Project

## Team Members


## Design Overview


## Build, Run and Test

### With Docker

```bash
docker compose build # build the image
docker compose run --rm tividy # run the program (opens the TUI)
docker compose run --rm tests # run the test suite
```

The first build takes a few minutes while CMake, FTXUI and GoogleTest are set up in the image. `run` doesn't rebuild the image, so run `docker compose build` again after changing code.

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