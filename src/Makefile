# VYPe 2014
# Authors: Vendula Poncová, xponco00
#          xxx

CFLAGS = -g 
.PHONY: clean test

all:	vype

vype:	lexer.o parser.tab.o symtable.o driver.o error.o generator.o vype.o instruction.o
	c++ -g -o vype vype.o error.o generator.o driver.o symtable.o instruction.o parser.tab.o lexer.o -lm

vype.o: vype.cpp parser.tab.hh symtable.h error.h
	c++ -c $<

generator.o: generator.cpp generator.h symtable.h
	c++ -c $<

instruction.o: instruction.cpp instruction.h symtable.h
	c++ -c $<

error.o: error.cpp error.h
	c++ -c $<

driver.o: driver.cpp driver.h symtable.h
	c++ -c $<

symtable.o: symtable.cpp symtable.h
	c++ -c $<

lexer.o: lexer.c parser.tab.hh symtable.h
	c++ -c $<

parser.tab.o: parser.tab.cc symtable.h
	c++ -c $<
	
lexer.c: lexer.l
	flex -olexer.c lexer.l

parser.tab.cc parser.tab.hh: parser.yy
	bison -vd --report='all' -Wno-other parser.yy

clean:
	rm -f vype \
	*.o \
	lexer.c parser.tab.c parser.tab.cc parser.tab.hh \
	lexer.o parser.tab.o parser.output \
	location.hh position.hh stack.hh

test:
	(cd test; 	python3 -m unittest discover;)

