all:
	cmake -S . -B build && cmake --build build

docs:
	doxygen Doxyfile

run: all
	./build/tividy

test: all
	cd build && ctest --output-on-failure

clean:
	rm -rf build
