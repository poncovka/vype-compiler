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
#include <fstream>  
#include <cstdio>
#include "parser.tab.hh"
#include "driver.h"
#include "error.h"
#include "generator.h"

int main(int argc, char **argv) {
  
  // check params
  if (argc < 2 || argc > 3) {
    ERROR(Error::INTERN, "Wrong number of parameters. " << argv[1])
  }
  else {

    const char *inname = argv[1];
    const char *outname = (argc == 3) ? argv[2] : "out.asm";
  
    // open input file
    FILE *in = fopen(inname, "r");
    
    if (!in) {
       ERROR(Error::INTERN, "Cannot open file " << inname << ".")
    }
    else {
    
      // parsing
      Driver driver;
      driver.parse(in, string(inname));
      fclose(in);
      
      if (!Error::error) {
      
        // open output file          
        FILE *out = fopen(outname, "w");
        
        if (!out) {
          ERROR(Error::INTERN, "Cannot open file " << outname << ".")
        }
        else {      
        
          // generating
          Generator generator;
          
          string mips = generator.run(driver.symtable.functions);
          
		      DEBUG(mips);    
		      fprintf(out, "%s", mips.c_str());
		      
          fclose(out);
        } 
      }    
    }
  }

  return Error::error;
}
