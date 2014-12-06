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

class Variable {

public:
  int type;
  string id;
  
  Variable(int type);
  Variable(int type, string &id);
  
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

class Function {

  typedef list<Variable*> tvarlist;
  typedef list<VariableTable*> ttablelist;

public:
  int type;
  string id;
  tvarlist params;
  ttablelist variables;

  Function(int type, string &id);
  ~Function();
  
  VariableTable* createVariableTable();
  void addParameter(Variable *var);

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


#endif
