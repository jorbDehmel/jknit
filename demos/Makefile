.PHONY:	test
test: clean
	for CASE in demo demo2 demo3 ; \
	do \
		jknit $$CASE.jmd -o $$CASE.md ; \
		jknit $$CASE.jmd -o $$CASE.tex ; \
	done
	if marp --version 2>&1 > /dev/null ; then \
		jknit presentation.jmd -o presentation.md ; \
		marp presentation.md --pdf -o presentation.pdf ; \
	fi

.PHONY:	clean
clean:
	rm -f *.o *.out *.log *.png *.aux *.pdf a.* *.listing \
		*.tex *.pptx *.md
