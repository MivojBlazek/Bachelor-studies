# Author: Michal Blažek (xblaze38)

PROJ=proj5

$(PROJ).pdf: $(PROJ).tex
	latex $(PROJ).tex
	bibtex $(PROJ)
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf -sPAPERSIZE=a4 $(PROJ).ps

clean:
	rm -f $(PROJ).aux $(PROJ).dvi $(PROJ).log $(PROJ).ps $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf