all:	build
clean:
	@echo "\n\x1b[32m--------| \x1b[1;32mCLEANING\x1b[0m\x1b[32m |--------\x1b[0m"
	rm -f *.o *.s *.out *.gch Article_handler utils/*.gch
	@echo "\x1b[32m\tDONE\x1b[0m"
build: 
	@echo "\x1b[32m--------| \x1b[1mTest of compilateur\x1b[0m\x1b[32m |--------\x1b[0m"
	g++ main.cpp utils/appInfo.cpp utils/appInfo.hpp utils/folder_info.cpp utils/folder_info.hpp
	mv a.out Article_handler
	@echo "\x1b[32m\tDONE\x1b[0m\n"
test: build
	@echo "\x1b[32m--------| \x1b[1mTest of binary\x1b[0m\x1b[32m |--------\x1b[0m"
	./Article_handler -v
	@echo "\x1b[32m\-----------\x1b[0m\n"
	./Article_handler -h
	@echo "\x1b[32m\-----------\x1b[0m\n"
	./Article_handler -o testOut
	@echo "\x1b[32m\tDONE\x1b[0m\n"

.PHONY: all test

