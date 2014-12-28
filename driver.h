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
  
  list<Variable*> varList;
  InstructionList instList;
  int tempCount;
  
  Driver();
  ~Driver();
  
  void addFunction(string *id, Symtable::Type type);
  void addVariables(Symtable::Type type);
  void addParam(string *id, Symtable::Type type);
  void addType(Symtable::Type type);
  void addId(string *id);
  void addExpression(Variable *var);
  
  Function* getFunction(string *id);
  Variable* getVariable(string *id);
  Variable* getTempVariable(Symtable::Type type);

  void enterFunc(string *id, Symtable::Type type);
  void enterBlock();
  void leaveBlock();

  void genAssignment(string *id, Variable *var);
  void genCall(string *id);
  void genReturn(Variable *var);

  Variable* genExprInt(int ival);
  Variable* genExprChar(string *sval);
  Variable* genExprStr(string *sval);
  
  Variable* genExprVar(string *id);
  Variable* genExprFce(string *id);
  Variable* genExprCast(Variable *var, Symtable::Type type);
  Variable* genExprOp(Variable *var1, Variable *var2, Symtable::Operator op);  

};

#endif
