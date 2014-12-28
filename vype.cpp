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

#include <iostream>
#include <cstdio>
#include "parser.tab.hh"
#include "driver.h"

int main(int argc, char **argv) {

  extern FILE *yyin;
  
  if (argc >= 2) {
    if (!(yyin = fopen(argv[1], "r"))) {
      std::cerr << "cannot open input file: " << argv[1] << std::endl;
      exit(0);
    }
  } 

  Driver driver;
  yy::parser parser(driver);
  int result = parser.parse();

  return result;
  
}
