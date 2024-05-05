.PHONY:	all
all:	demo.pdf demo2.pdf presDemo.pdf

%.pdf:	%.tex Makefile
	rm -f *.aux *.listing *.log *.out *.aux
	pdflatex $<
	pdflatex $<
	rm -f *.aux *.listing *.log *.out *.aux

%.tex:	%.jmd Makefile /usr/bin/jknit
	jknit $< -o $@

%.tex:	%.jpres Makefile
	jknit $< -po $@

.PHONY:	clean
clean:
	rm -f *.aux *.listing *.log *.aux *.tex *.out *.pdf
