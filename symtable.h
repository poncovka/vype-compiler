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
#include <sstream>
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

//////////////////////////////////// Expression

class Expression {

  public:
    Variable *var;
    InstructionList inst;

    Expression(Variable *var, InstructionList *l);
    ~Expression();
};

typedef std::list<Expression*> ExpressionList;
typedef std::list<Expression*>::iterator ExpressionIter;

//////////////////////////////////// Function

class Function {

  typedef list<Variable*> tvarlist;
  typedef list<VariableTable*> ttablelist;

public:
  Symtable::Type type;
  string id;
  tvarlist params;
  bool isdef;
  
  ttablelist variables;
  InstructionList instructions;

  Function(string &id, list<Variable*> params, Symtable::Type type, bool isdef);
  ~Function();
  
  VariableTable* createVariableTable();
  bool checkParameters(list<Variable*> variables);
  bool checkParameters(list<Expression*> expressions);
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
  ~SymbolTable();
  
  Variable* lookupVariable(string &id);
  Function* lookupFunction(string &id);
  
  bool insert(Function *func);
  bool insert(Variable *var);
  
  void enterBlock(Function *func);
  void enterBlock();
  void leaveBlock();

};

//////////////////////////////////// Instructions


class Label : public Instruction {

  private:
  static int maxlabel;

  public:
    string label;
    
  Label();
  Label(const Label &label);
};

// result = a op b
class ExpressionInst : public Instruction {
  
  public:
    Variable *var1;
    Variable *var2;
    Variable *result;
    Symtable::Operator op;
  
};

// result = (type) a
class CastInst : public Instruction {
  
  public:
    Variable *var;
    Variable *result;
    Symtable::Type type;
  
};

// a = value
class LoadInst : public Instruction {

  public:
    Variable *result;
};

// a = b;
class AssignmentInst : public Instruction {

  public:
    Variable *var;
    Variable *result;
};

// goto a
class JumpInst : public Instruction {
  
  public:
    Label *label;
  
};

// if not a then goto b
class JumpFalseInst : public Instruction {
  
  public:
    Variable *cond;
    Label *label;
  
};

// a = func(b,c)
class CallInst : public Instruction {
  
  public:
    Function *fce;
    list<Variable*> args;
    Variable *result;
  
};

// return a
class ReturnInst : public Instruction {
  
  public:
    Variable *result;
  
};

//////////////////////////////////// streaming

std::ostream& operator<< (std::ostream&, const Label&);
std::ostream& operator<< (std::ostream&, const ExpressionInst&);
std::ostream& operator<< (std::ostream&, const CastInst&);
std::ostream& operator<< (std::ostream&, const LoadInst&);
std::ostream& operator<< (std::ostream&, const AssignmentInst&);
std::ostream& operator<< (std::ostream&, const JumpInst&);
std::ostream& operator<< (std::ostream&, const JumpFalseInst&);
std::ostream& operator<< (std::ostream&, const CallInst&);
std::ostream& operator<< (std::ostream&, const ReturnInst&);

//////////////////////////////////// free

void freeVariables(list<Variable*> &l);
void freeVariableTables(list<VariableTable*> &l);
void freeExpressions(list<Expression*> &l);
void freeInstructions(list<Instruction*> &l);

#endif
