# Author: Michal Blažek (xblaze38)

PROJ=proj2

$(PROJ).pdf: $(PROJ).tex
	pdflatex $(PROJ).tex
	pdflatex $(PROJ).tex

clean:
	rm -f $(PROJ).aux $(PROJ).log $(PROJ).out $(PROJ).pdf $(PROJ).toc $(PROJ).snm $(PROJ).nav $(PROJ).vrb $(PROJ).dvi $(PROJ).ps

clean-all: clean