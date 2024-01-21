.PHYON: run clean init compile test

run:
	./out/bin/xtaro-main

clean:
	rm -rf ./build
	rm -rf ./out

init:
	cmake -B build

compile:
	cmake --build build

test:
	./out/bin/xtaro-main ./temp/option.json