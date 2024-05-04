SOURCES := engine.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := engine.hpp

EXE := jknit.out
#EXE := jknit.exe

CC := clang++ -std=c++20 -o3
#CC := x86_64-w64-mingw32-g++ --static
LFLAGS :=
CFLAGS := -Wall -pedantic -g #-Werror

all: $(SOURCES) $(EXE)

install: $(EXE)
	sudo cp ./jknit.out /usr/bin/jknit
	sudo cp -r compilation-drivers /usr/include
	sudo cp gui.py /usr/bin/jknit-gui

	sudo chmod +x /usr/bin/jknit-gui \
		/usr/include/compilation-drivers/*

uninstall:
	sudo rm -f /usr/bin/jknit /usr/bin/jknit-gui
	sudo rm -rf /usr/include/compilation-drivers

reinstall:
	$(MAKE) clean
	$(MAKE) install

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(OBJECTS) $(HEADERS)
	clang-format -i *.cpp *.hpp
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

remake:
	$(MAKE) clean
	$(MAKE)

clean:
	rm -rf *.pdf *.aux *.log *.tex *.out *.o *.zst *.tar \
		*.listing pkg jknit src