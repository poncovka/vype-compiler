/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: symtable.h
 * Classes for the symbols and symbol tables.
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <string>
#include <list>
#include <deque>
#include <map>
#include <utility>

using namespace std;

//////////////////////////////////// Enums

struct Symtable {
  enum Type { VOID, TINT, TCHAR, TSTRING };
  enum Operator { ADD, SUB, MUL, DIV, MOD, LT, LTE, GT, GTE, EQ, NEQ, AND, OR, NEG };
};


//////////////////////////////////// Inst

class Instruction {};
typedef std::list<Instruction*> InstructionList;
typedef std::list<Instruction*>::iterator InstructionIter;

//////////////////////////////////// Variable

class Variable {

public:
  Symtable::Type type;
  string id;
  
  int ival;
  string sval;

  Variable(Symtable::Type type);
  Variable(string &id, Symtable::Type type);
  
};

class VariableTable {

  typedef map<string, Variable*> tmap;
  typedef pair<string, Variable*> tpair;
  typedef pair<tmap::iterator, bool> treturn;

public:
  tmap symbolTable;
  
  ~VariableTable();
  bool insert(Variable *var);
  Variable* lookup(string &id);

};

//////////////////////////////////// Function

class Function {

  typedef list<Variable*> tvarlist;
  typedef list<VariableTable*> ttablelist;

public:
  Symtable::Type type;
  string id;
  tvarlist params;
  ttablelist variables;
  
  InstructionList instructions;

  Function(string &id, list<Variable*> params, Symtable::Type type);
  ~Function();
  
  VariableTable* createVariableTable();

};

class FunctionTable {

  typedef map<string, Function*> tmap;
  typedef pair<string, Function*> tpair;
  typedef pair<tmap::iterator, bool> treturn;


public:
  tmap symbolTable;
  
  ~FunctionTable();
  bool insert(Function *func);
  Function* lookup(string &id);

};

//////////////////////////////////// SymbolTable

class SymbolTable {

  typedef deque<VariableTable*> tstack;

public:
  Function *main;
  FunctionTable functionTable;
  
  Function *actualFunction;
  tstack stack;

  SymbolTable();
  
  Variable* lookupVariable(string &id);
  Function* lookupFunction(string &id);
  
  bool insert(Function *func);
  bool insert(Variable *var);
  
  void enterBlock(Function *func);
  void enterBlock();
  void leaveBlock();

};

//////////////////////////////////// Instructions

// result = a op b
class ExpressionInst : Instruction {
  
  public:
    Variable *a;
    Variable *b;
    Variable *result;
    int op;
  
};

// a = b;
class AssignmentInst : Instruction {

  public:
    Variable *a;
    Variable *result;
};

// goto a
class JumpInst : Instruction {
  
  public:
    InstructionIter jump;
  
};

// if a then goto b
class JumpIfInst : Instruction {
  
  public:
    Variable *cond;
    InstructionIter jump;
  
};

// a = func(b,c)
class CallInst : Instruction {
  
  public:
    Function *fce;
    list<Variable*> params;
    Variable *result;
  
};

// return a
class ReturnInst : Instruction {
  
  public:
    Variable *result;
  
};


#endif
