/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: vype.cpp
 * Main program.
 */

#include "parser.tab.hh"
#include "symtable.h"
#include <iostream>
#include <cstdio>

int main(int argc, char **argv) {

  extern FILE *yyin;
  
  if (argc >= 2) {
    if (!(yyin = fopen(argv[1], "r"))) {
      std::cerr << "cannot open input file: " << argv[1] << std::endl;
      exit(0);
    }
  } 

  yy::parser parser;
  int result = parser.parse();

  return result;
  
}
