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

namespace Symtable {

	enum Type {
		VOID, TINT, TCHAR, TSTRING
	};

	enum Operator {
		ADD, SUB, MUL, DIV, MOD, LT, LTE, GT, GTE, EQ, NEQ, AND, OR, NEG
	};

	string str(Type type);
	string str(Operator op);
}

//////////////////////////////////// Inst

class Generator;

class Instruction {
public:
	virtual ~Instruction() {};
  virtual string str() {};
	virtual string generate(Generator *g){return "";};
};

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
  string str();
  
};

class VariableTable {
	typedef map<string, Variable*> tmap;
	typedef pair<string, Variable*> tpair;
	typedef pair<tmap::iterator, bool> treturn;

  public:
  tmap symtable;
  
  ~VariableTable();
  bool insert(Variable *var);
  Variable* lookup(string &id);
};

//////////////////////////////////// Expression

class Expression {

  public:
  Variable *var;
  InstructionList inst;

  Expression();
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
  bool isbuiltin;
  
  ttablelist variables;
  InstructionList instructions;

  Function(string &id, list<Variable*> params, Symtable::Type type);
  Function(){};
  ~Function();
  
  static Function* createDeclaration(string &id, list<Variable*> params, Symtable::Type type);
  static Function* createBuiltin(string &id, list<Variable*> params, Symtable::Type type);
  
  VariableTable* createVariableTable();
  
  virtual bool checkParameters(list<Variable*> variables);
  virtual bool checkParameters(list<Expression*> expressions);
  
  string str();
  void clear();
};

class UnlimitedBuiltinFunction : public Function {

  public:
  int minparams;
  
  UnlimitedBuiltinFunction(string &id, int minparams, Symtable::Type type);
  
  bool checkParameters(list<Variable*> variables);
  bool checkParameters(list<Expression*> expressions);  
  
};

class FunctionTable {
	typedef map<string, Function*> tmap;
	typedef pair<string, Function*> tpair;
	typedef pair<tmap::iterator, bool> treturn;


public:
	tmap symtable;

	~FunctionTable();
	bool insert(Function *func);
	Function* lookup(string &id);

};

//////////////////////////////////// SymbolTable

class SymbolTable {
	typedef deque<VariableTable*> tstack;

public:
	Function *main;
	FunctionTable functions;

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

//////////////////////////////////// free

void freeVariables(list<Variable*> &l);
void freeVariableTables(list<VariableTable*> &l);
void freeExpressions(list<Expression*> &l);
void freeInstructions(list<Instruction*> &l);

#endif
