/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: error.h
 * Handle errrors.
 */

#ifndef _ERROR_H_
#define _ERROR_H_

#include <string>
#include <iostream>
using namespace std;

#define ERROR(code, args...) std::cerr << "ERROR[" << code << "]: " << args << std::endl; Error::set(code); 
#define DEBUG(args...)       std::cerr << "DEBUG: " << args << std::endl;

namespace Error {

  enum Code { OK = 0, LEX = 1, SYN = 2, SEM = 3, GEN = 4, INTERN = 5 };
  
  extern Code error;
  void set(Code code);
  Code get();
  
}

#endif
