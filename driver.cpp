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

//////////////////////////////////// Driver

Driver::Driver() {

  filename = new std::string("filename");
  tempCount = 0;
}

Driver::~Driver() {
  delete filename;
}

//////////////////////////////////// Driver.add

void Driver::addFunction(string *id, Symtable::Type type) {
  
  Function *func = new Function(*id, varList, type);
  if (!symtable.insert(func)) {
    // ERROR
  }
  
  delete id;
}

void Driver::addVariables(Symtable::Type type) {
 
  Variable *var = NULL;
  
  while (!varList.empty()) {
    var = varList.front();    
    var->type = type;

    if (!symtable.insert(var)) {
      // ERROR
    }    
    
    varList.pop_front();
  } 
}

void Driver::addParam(string *id, Symtable::Type type) {
  Variable *var = new Variable(*id, type);
  varList.push_back(var);
  
  delete id;
}
  
void Driver::addType(Symtable::Type type) {
  Variable *var = new Variable(type);
  varList.push_back(var);
}

void Driver::addId(string *id) {
  Variable *var = new Variable(*id, Symtable::VOID);
  varList.push_back(var);
  
  delete id;
}

void Driver::addExpression(Variable *var) { 
  varList.push_back(var);
}

//////////////////////////////////// Driver.get

Function* Driver::getFunction(string *id) {
    
  Function *func = symtable.lookupFunction(*id);
  if (func == NULL) {
    // ERROR
  }
  delete id;  
  return func;
}

Variable* Driver::getVariable(string *id) {
  
  Variable *var = symtable.lookupVariable(*id);
  if (var == NULL) {
    // ERROR
  }
  delete id;  
  return var;
}

Variable* Driver::getTempVariable(Symtable::Type type) {
  
  std::stringstream stream;
  stream << "_temp" << tempCount++ << endl;
  string *id = new string(stream.str());
  
  Variable *var = new Variable(*id, type);
  if (!symtable.insert(var)) {
    // ERROR
  }
  
  delete id;
  return var;
}

//////////////////////////////////// Driver.enter

void Driver::enterFunc(string *id, Symtable::Type type) {
  
  Function *func = symtable.lookupFunction(*id);
  
  // function was declared, check headers
  if (func != NULL) {
  
    if (func->type != type) {
      // ERROR
    }
  
    Variable *var = NULL;
    for(list<Variable*>::iterator i=varList.begin(); i!=varList.end(); ++i) {
    
      if (func->params.empty()) {
        // ERROR
      }
      
      var = func->params.front();    
      if (var->type != (*i)->type) {
        // ERROR
      }
      
      varList.pop_front();
    }
    
    if (!func->params.empty()) {
      //ERROR
    }
    
    func->params.splice(func->params.begin(), varList);
  }

  // function was not declared, add the function  
  else {  

    func = new Function(*id, varList, type);
    if (!symtable.insert(func)) {
      // ERROR
    }
  }
    
  // enter block
  symtable.enterBlock(func);
   
  // add parameters to variable table
  for (list<Variable*>::iterator i = func->params.begin(); i != func->params.end(); ++i) {
    
    if (!symtable.insert(*i)) {
      // ERROR
    }
  }
  
  delete id;    
}
  
void Driver::enterBlock() {
  symtable.enterBlock();
}
  
void Driver::leaveBlock() {
  symtable.leaveBlock();
}

//////////////////////////////////// Driver.generate

void Driver::genAssignment(string *id, Variable *var) {

  // TODO generate instruction for assignment
  
  delete id;
}

void Driver::genCall(string *id) {

  // TODO generate instruction for calling
  
  Variable *var = NULL;
  
  while (!varList.empty()) {
    var = varList.front();    
    varList.pop_front();
  }
  
  delete id;
}

void Driver::genReturn(Variable *var) {

  // TODO generate return
  
}

//////////////////////////////////// Driver.generateExpression

Variable* Driver::genExprInt(int ival) {

  Variable *var = getTempVariable(Symtable::TINT);
  var->ival = ival;
  return var;

}

Variable* Driver::genExprChar(string *sval) {

  Variable *var = getTempVariable(Symtable::TCHAR);
  var->sval = *sval;
  delete sval;
  return var;

}

Variable* Driver::genExprStr(string *sval) {

  Variable *var = getTempVariable(Symtable::TSTRING);
  var->sval = *sval;
  delete sval;
  return var;

}
  
Variable* Driver::genExprVar(string *id) {

  Variable *var = symtable.lookupVariable(*id);
  if (var == NULL) {
    // ERROR
  }
  
  Variable *tvar = getTempVariable(var->type);
  delete id;
  return tvar;

}

Variable* Driver::genExprFce(string *id) {

  Function *func = symtable.lookupFunction(*id);
  if (func == NULL) {
    // ERROR
  }
  
  Variable *tvar = getTempVariable(func->type);
  delete id;
  return tvar;
}

Variable* Driver::genExprCast(Variable *var, Symtable::Type type) {

  // TODO check if cast is possible
  
  Variable *tvar = getTempVariable(type);
  return tvar;

}

Variable* Driver::genExprOp(Variable *var1, Variable *var2, Symtable::Operator op) {
  
  
  // TODO check if operation is possible
  
  /*
  switch(op) {
  
    case ADD: 
    case SUB:
    case MUL:
    case DIV:
    case MOD:
    case LT:
    case LTE:
    case GT:
    case GTE:
    case EQ:
    case NEQ:
    case AND:
    case OR:
    case NEG: break;
    
  }
  */

  Variable *tvar = getTempVariable(Symtable::TINT);
  return tvar;

}
    
/* end of file */
