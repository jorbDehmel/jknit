# Build for windows from Linux using mingw64
# NOTE: Does not save compilation drivers anywhere!
# If you want the full experience, use linux or WSL

SOURCES := ../engine.cpp ../main.cpp
OBJECTS := engine.o main.o
HEADERS := ../engine.hpp

#EXE := jknit.out
EXE := jknit.exe

#CC := clang++
CC := x86_64-w64-mingw32-g++ --static
LFLAGS :=
CFLAGS := -Wall -pedantic -g #-Werror

all: $(SOURCES) $(EXE)

engine.o:	../engine.cpp
	$(CC) -c $(CFLAGS) ../engine.cpp -o engine.o

main.o:	../main.cpp
	$(CC) -c $(CFLAGS) ../main.cpp -o main.o

$(EXE): $(OBJECTS) $(HEADERS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

remake:
	$(MAKE) clean
	$(MAKE)

clean:
	rm -rf $(OBJECTS) $(EXE) *.pdf *.aux *.log *.tex *.exe *.o
