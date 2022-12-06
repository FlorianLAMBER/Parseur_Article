UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	install_cmd := sudo apt install g++ git pdftotext
	COLOR := "\\e"
endif
ifeq ($(UNAME_S),Darwin)
	install_cmd := brew install gcc git poppler
	COLOR := "\\x1b"
endif
all:	build
clean:
	@echo "\n$(COLOR)[32m--------| $(COLOR)[1;32mCLEANING$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m"
	rm -f *.o *.s *.out *.gch Article_handler utils/*.gch Article_handler_menu *.xml *.txt
	rm -rf FTXUI/build
	mkdir FTXUI/build
	@echo "$(COLOR)[32m\tDONE$(COLOR)[0m"

build: FTXUI/build
	@echo "$(COLOR)[32m--------| $(COLOR)[1mBuild Article_handler binary:$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m"
	cd FTXUI/build && cmake .. && make 
	g++ -std=c++17 main.cpp utils/appInfo.cpp utils/appInfo.hpp utils/folder_info.cpp utils/folder_info.hpp
	mv FTXUI/build/ftxui-starter Article_handler_menu
	mv a.out Article_handler
	@echo "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

install: 
	@echo "$(COLOR)[32m--------| $(COLOR)[1mInstallation of dependencies :$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m"
	$(install_cmd)
	@echo "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

clone: install
	git clone https://github.com/Fliche/Parseur_Article
	mkdir FTXUI/build
	@echo "$(COLOR)[32m\tEVERYTHINK IS DONE$(COLOR)[0m\n"

.PHONY: all build
