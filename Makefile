SOURCES := engine.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := engine.hpp
EXE := jknit.out

CC := clang++
LFLAGS :=
CFLAGS := -Wall -Werror -pedantic -g

all: $(SOURCES) $(EXE)

install: $(EXE)
	sudo cp ./jknit.out /usr/bin/jknit
	sudo cp -r compilation-drivers /usr/include

uninstall:
	sudo rm -f /usr/bin/jknit
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
	rm -rf $(OBJECTS) $(EXE) *.pdf *.aux *.log *.tex *.out