#
# Projekt: ITY - proj1
# Autor:   Vendula Poncová
# Datum:   23.02.2012
# 
 
NAME=xponco00_xsifra00_abstract

all: $(NAME).pdf

$(NAME).pdf:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

pdf:
	$(NAME).tex
	pdflatex $(NAME).tex

vlnka:
	vlna $(NAME).tex

clean:
	rm -rf $(NAME).aux $(NAME).log $(NAME).out $(NAME).toc $(NAME).dvi $(NAME).ps $(NAME).pdf *~
