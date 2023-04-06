SOURCES := engine.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o)
EXE := main.out

CC := clang++
LFLAGS :=
CFLAGS := -Wall -Werror -pedantic -g

all: $(SOURCES) $(EXE)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

remake:
	$(MAKE) clean
	$(MAKE)

clean:
	rm -rf $(OBJECTS) $(EXE) *.pdf *.aux *.log *.tex *.out