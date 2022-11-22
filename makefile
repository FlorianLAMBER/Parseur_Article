UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	install_cmd := sudo apt install g++ git
endif
ifeq ($(UNAME_S),Darwin)
	install_cmd := brew install gcc git
endif
all:	build
clean:
	@echo "\n\x1b[32m--------| \x1b[1;32mCLEANING\x1b[0m\x1b[32m |--------\x1b[0m"
	rm -f *.o *.s *.out *.gch Article_handler utils/*.gch
	@echo "\x1b[32m\tDONE\x1b[0m"

build: 
	@echo "\x1b[32m--------| \x1b[1mBuild Article_handler binary:\x1b[0m\x1b[32m |--------\x1b[0m"
	g++ main.cpp utils/appInfo.cpp utils/appInfo.hpp utils/folder_info.cpp utils/folder_info.hpp
	mv a.out Article_handler
	@echo "\x1b[32m\tDONE\x1b[0m\n"

install: 
	@echo "\x1b[32m--------| \x1b[1mInstallation of dependencies :\x1b[0m\x1b[32m |--------\x1b[0m"
	$(install_cmd)
	
	@echo "\x1b[32m\tDONE\x1b[0m\n"

clone: install
	git clone https://github.com/Fliche/Parseur_Article
	@echo "\x1b[32m\tEVERYTHINK IS DONE\x1b[0m\n"

.PHONY: all build