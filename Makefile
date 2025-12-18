WX_CONFIG := $(shell brew --prefix wxwidgets@3.2)/bin/wx-config-3.2

.PHONY: all configure build link compile_commands run clean

all: build link

configure:
	cmake -S . -B build \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DWX_CONFIG="$(WX_CONFIG)"

build: configure
	cmake --build build

link: build
	ln -sf build/compile_commands.json compile_commands.json

compile_commands: link

run: build
	c++ -o hello main.cpp `wx-config-3.2 --cxxflags --libs`
	./hello

clean:
	rm -rf build
	rm -f compile_commands.json
