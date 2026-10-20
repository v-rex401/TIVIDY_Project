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

Requires `g++` (C++11), `cmake` and `make`.

```bash
make # build
make run # build and run
make test # build and run tests
make clean # remove build output
```

### Documentation

```bash
doxygen Doxyfile # output in docs/html/index.html
```