# VYPe 2014
# Authors: Vendula Poncová, xponco00
#          xxx

CFLAGS = -g
.PHONY: clean test

all:	vype

vype:	lexer.o parser.tab.o symtable.o vype.o
	c++ -g -o vype vype.o symtable.o parser.tab.o lexer.o -lm

vype.o: vype.cpp parser.tab.hh symtable.h
	c++ -c $<

symtable.o: symtable.cpp symtable.h
	c++ -c $<

lexer.o: lexer.c parser.tab.hh
	c++ -c $<

parser.tab.o: parser.tab.cc

lexer.c: lexer.l
	flex -olexer.c lexer.l

parser.tab.cc parser.tab.hh: parser.yy
	bison -vd --report='all' parser.yy

clean:
	rm -f vype \
	lexer.c parser.tab.c parser.tab.cc parser.tab.hh \
	lexer.o parser.tab.o parser.output \
	location.hh position.hh stack.hh

test:
	(cd test; 	python3.2 -m unittest discover;)

