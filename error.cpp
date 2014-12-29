/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: error.cpp
 * Handle errors.
 */

#include "error.h"

namespace Error {

  Code error = EOK;

  void set(Code code) {
    if (error == Error::EOK) {
      error = code;
    }
  }
  
  Code get() {
    return error;
  }
}



