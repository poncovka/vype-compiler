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
  void addParam(string *id, Symtable::Type type);
  void addType(Symtable::Type type);
  void addId(string *id);
  
  Function* getFunction(string *id);
  Variable* getVariable(string *id);
  Variable* getTempVariable(Symtable::Type type);

  void enterFunc(string *id, Symtable::Type type);
  void leaveFunc(string *id, InstructionList *l);
  void enterBlock();
  void leaveBlock();  

  Expression* genExprInt(int ival);
  Expression* genExprChar(string *sval);
  Expression* genExprStr(string *sval);
  Expression* genExprVar(string *id);
  Expression* genExprFce(string *id);
  Expression* genExprCast(Expression *expr, Symtable::Type type);
  Expression* genExprOp(Expression *expr1, Expression *expr2, Symtable::Operator op);  

  ExpressionList* genExprEmpty();
  ExpressionList* genExprList(Expression *e);
  ExpressionList* genExprJoin(Expression *e, ExpressionList *l);
  

  InstructionList* genInstEmpty();  
  InstructionList* genInstJoin(InstructionList *l1, InstructionList *l2);
  InstructionList* genVariables(Symtable::Type type);
  InstructionList* genAssignment(string *id, Expression *expr);
  InstructionList* genCall(string *id, ExpressionList *lexpr);
  InstructionList* genReturn(Expression *expr);
  InstructionList* genWhile(Expression *expr, InstructionList *l);

};

#endif
