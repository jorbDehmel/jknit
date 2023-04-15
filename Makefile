SOURCES := engine.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := engine.hpp

EXE := jknit.out
#EXE := jknit.exe./j

CC := clang++
#CC := x86_64-w64-mingw32-g++ --static
LFLAGS :=
CFLAGS := -Wall -pedantic -g #-Werror

all: $(SOURCES) $(EXE)

install: $(EXE)
	sudo cp ./jknit.out /usr/bin/jknit
	sudo cp -r compilation-drivers /usr/include
	chmod +x gui.py
	sudo cp gui.py /usr/bin/jknit-gui

uninstall:
	sudo rm -f /usr/bin/jknit /usr/bin/jknit-gui
	sudo rm -rf /usr/include/compilation-drivers

reinstall:
	$(MAKE) clean
	$(MAKE) install

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(OBJECTS) $(HEADERS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

remake:
	$(MAKE) clean
	$(MAKE)

clean:
	rm -rf *.pdf *.aux *.log *.tex *.out *.o