/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: driver.h
 * Driver of parser.
 */

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <string>
#include <sstream>
#include "symtable.h"

class Driver {

  public:
  FILE *file;
  std::string *filename;
  
  SymbolTable symtable;
  
  list<Variable*> *varList;
  list<VariableTable*> *varTable;
  InstructionList *instList;
  int tempCount;
  
  Driver();
  ~Driver();
  
  void addFunction(string &id, Type type);
  void addVariable(string &id, Type type);
  void addParam(string &id, Type type);
  void addType(Type type);
  
  Function* getFunction(string &id);
  Variable* getVariable(string &id);
  Variable* getTempVariable(Type type);

  void enterFunc(string &id, Type type);
  void enterBlock();
  void leaveBlock();


};

#endif
