TARGET := jknit.out
CPP := g++ -std=c++20 -O3 -pedantic -Wall -g
GLOBAL_DEPS := engine.hpp md_engine.hpp

.PHONY:	install
install:	$(TARGET)
	sudo cp $(TARGET) /usr/bin/$(TARGET:.out=)
	sudo cp -r compilation-drivers /usr/include
	sudo chmod +x /usr/include/compilation-drivers/*

.PHONY:	uninstall
uninstall:
	sudo rm -rf /usr/bin/$(TARGET:.out=) \
		/usr/include/compilation-drivers

.PHONY:	clean
clean:
	rm -f *.o *.out *.log *.png *.aux *.pdf a.*

.PHONY:	format
format:
	find . -type f \( -iname "*.cpp" -or -iname "*.hpp" \) \
		-exec clang-format -i "{}" \;

.PHONY:	test
test:
	cd demos && jknit demo.jmd -o demo.tex && pdflatex demo.tex
	cd demos && jknit demo2.jmd -o demo2.tex \
		&& pdflatex demo2.tex

$(TARGET):	main.o engine.o md_engine.o tex_engine.o
	$(CPP) -o $@ $^

%.o:	%.cpp $(GLOBAL_DEPS)
	$(CPP) -c -o $@ $<
