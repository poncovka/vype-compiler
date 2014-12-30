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

  params.clear(); // TODO could have variables that are not in table
  freeInstructions(instructions);
  freeVariableTables(variables);
  
}

VariableTable* Function::createVariableTable() {

  VariableTable *table = new VariableTable;
  variables.push_back(table);
  return table;
}

bool Function::checkParameters(list<Variable*> variables) {

  list<Variable*>::iterator i, j;
  i = params.begin();
  j = variables.begin();
  
  while(i != params.end() && j != variables.end() && (*i)->type == (*j)->type) {
    ++i;
    ++j;
  }
  
  return (i == params.end() && j == variables.end());
}

bool Function::checkParameters(list<Expression*> expressions) {

  list<Variable*>::iterator i = params.begin();
  list<Expression*>::iterator j = expressions.begin();
  
  while(i != params.end() && j != expressions.end() && (*i)->type == (*j)->var->type) {
    ++i;
    ++j;
  }
  
  return (i == params.end() && j == expressions.end());
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

SymbolTable::~SymbolTable() {
  stack.clear();
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
  freeInstructions(inst);
}

//////////////////////////////////// Label

int Label::maxlabel = 0;

Label::Label() {
  std::stringstream stream;
  stream << "L_" << Label::maxlabel++ ;
  this->label = string(stream.str());
}

Label::Label(const Label &label) {
  this->label = label.label;
}

//////////////////////////////////// streaming

std::ostream& operator<< (std::ostream& os, const Variable& v) {
  os << v.id;
  return os;
}

std::ostream& operator<< (std::ostream& os, const Label& i) {
  os << "label " << i.label;
  return os;
}

std::ostream& operator<< (std::ostream& os, const ExpressionInst& i) {
  os << *i.result << " = " << *i.var1 << " op(" << i.op << ") " << *i.var2;
  return os;
}

std::ostream& operator<< (std::ostream& os, const CastInst& i) {
  os << *i.result << " = " << "(" << i.type << ") " << *i.var;
  return os;
}

std::ostream& operator<< (std::ostream& os, const LoadInst& i) {
  os << *i.result << " = value";
  return os;
}

std::ostream& operator<< (std::ostream& os, const AssignmentInst& i) {
  os << *i.result << " = " << *i.var ;
  return os;
}

std::ostream& operator<< (std::ostream& os, const JumpInst& i) {
  os << "goto " << i.label ;
  return os;
}

std::ostream& operator<< (std::ostream& os, const JumpFalseInst& i) {
  os << "if not " << *i.cond << " goto " << i.label ;
  return os;

}

std::ostream& operator<< (std::ostream& os, const CallInst& i) {
  os << *i.result << " = " << i.fce->id << "()";
  return os;
}

std::ostream& operator<< (std::ostream& os, const ReturnInst& i) {
  os << "return " << *i.result;
}

//////////////////////////////////// free

void freeVariables(list<Variable*> &l) {

  while(!l.empty()) {
    Variable *var = l.front();
    l.pop_front();
    delete var;
  }
}

void freeVariableTables(list<VariableTable*> &l) {

  while(!l.empty()) {
    VariableTable *tab = l.front();
    l.pop_front();
    delete tab;
  }
}

void freeExpressions(list<Expression*> &l) {

  while(!l.empty()) {
    Expression *expr = l.front();
    l.pop_front();
    delete expr;
  }
  
}

void freeInstructions(list<Instruction*> &l) {

  while(!l.empty()) {
    Instruction *i = l.front();
    l.pop_front();
    delete i;
  }
}


/* end of file */
