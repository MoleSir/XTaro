.PHYON: run clean init compile

run:
	./out/bin/xtaro-main

clean:
	rm -rf ./build
	rm -rf ./out

init:
	cmake -B build

compile:
	cmake --build build