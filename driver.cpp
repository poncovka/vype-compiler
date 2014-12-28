/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: driver.cpp
 * Driver of parser.
 */

#include "driver.h"

Driver::Driver() {

  filename = new std::string("filename");
  varList = new list<Variable*>();
  instList = new InstructionList();
  tempCount = 0;
}

Driver::~Driver() {

  delete filename;
  delete varList;
  delete instList;
}

void Driver::addFunction(string &id, Type type) {
  
  // add func
  Function *func = new Function(id, *varList, type);
  if (!symtable.insert(func)) {
    // ERROR
  }
  
}
  
void Driver::addVariable(string &id, Type type) {
 
  Variable *var = new Variable(id, type);
  if (!symtable.insert(var)) {
    // ERROR
  }
    
}

void Driver::addParam(string &id, Type type) {
  Variable *var = new Variable(id, type);
  varList->push_back(var);
}
  
void Driver::addType(Type type) {
  Variable *var = new Variable(type);
  varList->push_back(var);
}

Function* Driver::getFunction(string &id) {
    
  Function *func = symtable.lookupFunction(id);
  if (func == NULL) {
    // ERROR
  }
    
  return func;
}

Variable* Driver::getVariable(string &id) {
  
  Variable *var = symtable.lookupVariable(id);
  if (var == NULL) {
    // ERROR
  }
    
  return var;
}

Variable* Driver::getTempVariable(Type type) {
  
  std::stringstream stream;
  stream << "_temp" << tempCount++ << endl;
  string *id = new string(stream.str());
  
  Variable *var = new Variable(*id, type);
  if (!symtable.insert(var)) {
    // ERROR
  }
  return var;
}

void Driver::enterFunc(string &id, Type type) {
    
  // add func
  Function *func = new Function(id, *varList, type);
  if (!symtable.insert(func)) {
    // ERROR
  }
    
  // enter block
  symtable.enterBlock(func);
   
  // add parameters
  for (list<Variable*>::iterator i = func->params.begin(); i != func->params.end(); ++i) {
    
    if (!symtable.insert(*i)) {
      // ERROR
    }
  }
    
}
  
void Driver::enterBlock() {
  symtable.enterBlock();
}
  
void Driver::leaveBlock() {
  symtable.leaveBlock();
}
