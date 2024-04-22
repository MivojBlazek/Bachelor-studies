# Author: Michal Blažek (xblaze38)

PROJ=proj5

$(PROJ).pdf: $(PROJ).tex
	pdflatex $(PROJ).tex
	bibtex $(PROJ)
	pdflatex $(PROJ).tex
	pdflatex $(PROJ).tex

clean:
	rm -f $(PROJ).aux $(PROJ).dvi $(PROJ).log $(PROJ).ps $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf
