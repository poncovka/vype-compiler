# VYPe 2014
# Authors: Vendula Poncová, xponco00
#          xxx

CFLAGS = -g
.PHONY: clean test

all:	vype

vype:	lex.o parser.tab.o
	c++ -g -o vype parser.tab.o lex.o -lm

lex.o: lex.c parser.tab.hh
	c++ -c $<

parser.tab.o: parser.tab.cc

lex.c: lex.l
	flex -olex.c lex.l

parser.tab.cc parser.tab.hh: parser.yy
	bison -vd --report='all' parser.yy

clean:
	rm -f vype \
	lex.c parser.tab.c parser.tab.cc parser.tab.hh \
	lex.o parser.tab.o parser.output \
	location.hh position.hh stack.hh

test:
	(cd test; 	python3.2 -m unittest discover;)

