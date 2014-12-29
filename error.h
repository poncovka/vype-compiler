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

  enum Code { EOK = 0, ELEX = 1, ESYN = 2, ESEM = 3, EGEN = 4, EINTERN = 5 };
  
  extern Code error;
  void set(Code code);
  Code get();
  
}

#endif
