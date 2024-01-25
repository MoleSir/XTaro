.PHYON: run clean init compile h v i l

run:
	./out/bin/xtaro-main

clean:
	rm -rf ./build
	rm -rf ./out

init:
	cmake -B build

compile:
	cmake --build build

h:
	./out/bin/xtaro -h

v:
	./out/bin/xtaro -v

i:
	./out/bin/xtaro -i

l:
	./out/bin/xtaro -l ./test/option.json