TARGET := jknit.out
CPP := clang++ -O3 -pedantic -Wall -g
GLOBAL_DEPS := engine.hpp md_engine.hpp

jknit.out:	main.o engine.o md_engine.o tex_engine.o
	$(CPP) -o $@ $^

%.o:	%.cpp $(GLOBAL_DEPS)
	$(CPP) -c -o $@ $<

.PHONY: install
install:	jknit.out
	cp jknit.out /usr/bin/jknit

.PHONY:	clean
clean:
	rm -f *.o *.out *.log *.png
