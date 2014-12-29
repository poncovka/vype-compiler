/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: symtable.cpp
 * Definitions of methods for symbols and symbol tables.
 */

#include "symtable.h"

//////////////////////////////////// Variable

Variable::Variable(Symtable::Type type) {
  this->type = type;
}

Variable::Variable(string &id, Symtable::Type type) {
  this->type = type;
  this->id = id;
}

//////////////////////////////////// VariableTable

bool VariableTable::insert(Variable *var) {

  treturn ret;
  ret = symbolTable.insert(tpair(var->id, var));
  return ret.second;
  
}

Variable* VariableTable::lookup(string &id) {

  tmap::iterator i;
  i = symbolTable.find(id);
  
  if (i == symbolTable.end()) {
    return NULL;
  }
  
  return i->second;
}

VariableTable::~VariableTable() {

  for(tmap::iterator i=symbolTable.begin(); i != symbolTable.end(); ++i) {
    //delete i->second;
  }

}

//////////////////////////////////// Function

Function::Function(string &id, list<Variable*> params, Symtable::Type type) {

  this->type = type;
  this->id = id;
  this->params.splice(this->params.begin(), params);
  
}

Function::~Function() {

  for(InstructionList::iterator i=instructions.begin(); i!=instructions.end(); ++i) {
    delete *i;
  }

  for(ttablelist::iterator i=variables.begin(); i!=variables.end(); ++i) {
    delete *i;
  }
  
}

VariableTable* Function::createVariableTable() {

  VariableTable *table = new VariableTable;
  variables.push_back(table);
  return table;
}

//////////////////////////////////// FunctionTable

bool FunctionTable::insert(Function *func) {

  treturn ret;
  ret = symbolTable.insert(tpair(func->id, func));
  return ret.second;
  
}

Function* FunctionTable::lookup(string &id) {

  tmap::iterator i;
  i = symbolTable.find(id);
  
  if (i == symbolTable.end()) {
    return NULL;
  }
  
  return i->second;
}

FunctionTable::~FunctionTable() {

  for(tmap::iterator i=symbolTable.begin(); i != symbolTable.end(); ++i) {
    delete i->second;
  }

}

//////////////////////////////////// SymbolTable

SymbolTable::SymbolTable() {
  main = NULL;
  actualFunction = NULL;
}

bool SymbolTable::insert(Function *func) {
  return functionTable.insert(func);
}


bool SymbolTable::insert(Variable *var) {
  if (stack.empty()) return false;
  
  VariableTable *table = stack.front();
  return table->insert(var);
}

Function* SymbolTable::lookupFunction(string &id) {
  return functionTable.lookup(id);
}

Variable* SymbolTable::lookupVariable(string &id) {

  VariableTable *table;
  Variable *var = NULL;
  
  for(tstack::iterator i = stack.begin(); i != stack.end(); ++i) {
  
    table = *i;
    var = table->lookup(id);
    if (var != NULL) break; 
  }
  
  return var;
}

void SymbolTable::enterBlock(Function *func) {

  actualFunction = func;
  enterBlock();  
}


void SymbolTable::enterBlock() {

  VariableTable *table = actualFunction->createVariableTable();
  stack.push_front(table);
}

void SymbolTable::leaveBlock() {

  stack.pop_front();
  
  if(stack.empty()) {
    actualFunction = NULL;
  }
}

//////////////////////////////////// Expression

Expression::Expression(Variable *var, InstructionList *l) {

  this->var = var;
  this->inst.splice(this->inst.begin(), *l);

}

Expression::~Expression() {

  while(!inst.empty()) {

    Instruction *i = inst.front();
    inst.pop_front();
    delete i;
  
  }
}

//////////////////////////////////// Label

int Label::maxlabel = 0;

Label::Label() {
  std::stringstream stream;
  stream << "L_" << Label::maxlabel++ << std::endl;
  this->label = string(stream.str());
}

Label::Label(const Label &label) {
  this->label = label.label;
}

/* end of file */
