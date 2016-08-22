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
#include "parser.tab.hh"
#include "error.h"
#include "symtable.h"
#include "instruction.h"

class Driver {

  public:
  std::string filename;
  
  yy::parser parser;
  SymbolTable symtable;
  
  list<Variable*> variables;
  int tempCount;
  
  Driver();
  ~Driver();
  
  void parse(FILE *f, const string &fname);
  
  void init();
  void check();
  
  void errorLex(yy::location const &loc, const string &msg);
  void errorSyn(yy::location const &loc, const string &msg);
  
  void addDeclaration(string *id, Symtable::Type type);
  void addParam(string *id, Symtable::Type type);
  void addType(Symtable::Type type);
  void addId(string *id);

  Variable* getTempVariable(Symtable::Type type);

  void enterFunc(string *id, Symtable::Type type);
  void leaveFunc(string *id, InstructionList *l);
  void enterBlock();
  void leaveBlock();  

  Expression* genExprInt(int ival);
  Expression* genExprChar(string *sval);
  Expression* genExprStr(string *sval);
  Expression* genExprVar(string *id);
  Expression* genExprFce(string *id, ExpressionList *lexpr);
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
  InstructionList* genReturn();
  InstructionList* genWhile(Expression *expr, InstructionList *l);
  InstructionList* genCondition(Expression *expr, InstructionList *l1, InstructionList *l2);

  void debug();

};

#endif
