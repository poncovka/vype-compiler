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


//////////////////////////////////// Symtable

string Symtable::str(Symtable::Type type) {
  static const string STR[] = {"void", "int", "char", "string"};
  return STR[type];
}

string Symtable::str(Symtable::Operator op) {
  static const string STR[] = {"+", "-", "*", "/", "%", "<", "<=", ">", ">=", "==", "!=", "&&", "||", "!"};
  return STR[op];
}

//////////////////////////////////// Variable

Variable::Variable(Symtable::Type type) {
  this->type = type;
}

Variable::Variable(string &id, Symtable::Type type) {
  this->type = type;
  this->id = id;
}

string Variable::str() {

  std::stringstream stream;
  stream <<  Symtable::str(type) << " " << id;  
  return stream.str();
}

//////////////////////////////////// VariableTable

bool VariableTable::insert(Variable *var) {

  treturn ret;
  ret = symtable.insert(tpair(var->id, var));
  return ret.second;
  
}

Variable* VariableTable::lookup(string &id) {

  tmap::iterator i;
  i = symtable.find(id);
  
  if (i == symtable.end()) {
    return NULL;
  }
  
  return i->second;
}

VariableTable::~VariableTable() {

  for(tmap::iterator i=symtable.begin(); i != symtable.end(); ++i) {
    delete i->second;
  }
}

//////////////////////////////////// Function

Function::Function(string &id, list<Variable*> params, Symtable::Type type) {

  this->type = type;
  this->id = id;
  this->params.splice(this->params.begin(), params);
  
  this->isdef = true;
  this->isbuiltin = false;
  
}

Function::~Function() {
  clear();
}

Function* Function::createDeclaration(string &id, list<Variable*> params, Symtable::Type type) {

  Function *f = new Function(id, params, type);
  f->isdef = false;
  return f;
  
}

Function* Function::createBuiltin(string &id, list<Variable*> params, Symtable::Type type) {

  Function *f = new Function(id, params, type);
  f->isdef = false;
  f->isbuiltin = true;
  return f;

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

string Function::str() {

  std::stringstream stream;
  stream <<  Symtable::str(type) << " " << id << "(";
  
  bool first = true;
  for(list<Variable*>::iterator i = params.begin(); i != params.end(); ++i) {
  
    if (!first) {
      stream << ", ";  
    }
  
    stream << (*i)->str();
    first = false;
  }
  
  stream << ")";
  
  return stream.str();
}

void Function::clear() {

  if (!isdef) {
    freeVariables(params);
  }
  
  params.clear();
  freeInstructions(instructions);
  freeVariableTables(variables);
  
}

//////////////////////////////////// UnlimitedBuiltinFunction

UnlimitedBuiltinFunction::UnlimitedBuiltinFunction (string &id, int minparams, Symtable::Type type) : Function() {

  this->type = type;
  this->id = id;
  this->minparams = minparams;
  
  this->isdef = false;
  this->isbuiltin = true;

}

bool UnlimitedBuiltinFunction::checkParameters(list<Variable*> variables) {
  return (variables.size() >= minparams);
}

bool UnlimitedBuiltinFunction::checkParameters(list<Expression*> expressions) {
  return (expressions.size() >= minparams);
}

//////////////////////////////////// FunctionTable

bool FunctionTable::insert(Function *func) {

  treturn ret;
  ret = symtable.insert(tpair(func->id, func));
  return ret.second;
  
}

Function* FunctionTable::lookup(string &id) {

  tmap::iterator i;
  i = symtable.find(id);
  
  if (i == symtable.end()) {
    return NULL;
  }
  
  return i->second;
}

FunctionTable::~FunctionTable() {

  for(tmap::iterator i=symtable.begin(); i != symtable.end(); ++i) {
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
  return functions.insert(func);
}


bool SymbolTable::insert(Variable *var) {
  if (stack.empty()) return false;
  
  VariableTable *table = stack.front();
  return table->insert(var);
}

Function* SymbolTable::lookupFunction(string &id) {
  return functions.lookup(id);
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

Expression::Expression() {
  this->var = NULL;
}

Expression::Expression(Variable *var, InstructionList *l) {

  this->var = var;
  this->inst.splice(this->inst.begin(), *l);

}

Expression::~Expression() {
  freeInstructions(inst);
}

//////////////////////////////////// Instructions

int Label::maxid = 0;

Label::Label() {
  std::stringstream stream;
  stream << "L" << Label::maxid++ ;
  stream.str(this->id);
}

Label::Label(const Label &label) {
  this->id = label.id;
}

string Label::str() {
  std::stringstream stream;
  stream << "label " << id;
  return stream.str();
}

string ExpressionInst::str() {
  std::stringstream stream;
  
  if (op == Symtable::NEG) {
    stream << result->id << " = " << Symtable::str(op) << " " << var1->id;
  }
  else {
    stream << result->id << " = " << var1->id << " " << Symtable::str(op) << " " << var2->id;
  }
  
  return stream.str();
}

string CastInst::str() {
  std::stringstream stream;
  stream << result->id << " = (" << Symtable::str(type) << ") " << var->id;
  return stream.str();
}

string LoadInst::str() {
  std::stringstream stream;
  stream << result->id << " = " << Symtable::str(result->type);
  return stream.str();
}

string AssignmentInst::str() {
  std::stringstream stream;
  stream << result->id << " = " << var->id;
  return stream.str();
}

string JumpInst::str() {
  std::stringstream stream;
  stream << "jump to " << label->id;
  return stream.str();
}

string JumpFalseInst::str() {
  std::stringstream stream;
  stream << "if not " << cond->id << " jump to " << label->id;
  return stream.str();
}

string CallInst::str() {
  std::stringstream stream;
  
  if (result) {
    stream << result->id << " = " ;
  }
  
  stream << fce->id << "(";
  
  bool first = true;
  for (list<Variable*>::iterator i = args.begin(); i != args.end(); ++i) {
    
    if (!first) {
      stream << ", ";
    }
    
    stream << (*i)->id;
    first = false;
  }
  
  stream << ")";
  return stream.str();
}

CallInst::~CallInst() {
  args.clear();
}

string ReturnInst::str() {
  std::stringstream stream;
  stream << "return " << result->id;
  return stream.str();
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
