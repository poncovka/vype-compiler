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

Driver::Driver() : parser(*this) {

  tempCount = 0;
}

Driver::~Driver() {
}

int Driver::parse(FILE *f, const string &fname) {

  filename = fname;
  extern FILE *yyin;
  yyin = f;

  return parser.parse();
}

void Driver::errorLex(yy::location const &loc, const string &msg) {

  ERROR(Error::LEX, "lexical error, " << msg << " at " << loc)
  throwError();
}

void Driver::errorSyn(yy::location const &loc, const string &msg) {

  ERROR(Error::SYN, msg << " at " << loc)
  throwError();
}

void Driver::throwError() {
  parser.error(yy::location(), "");
}

//////////////////////////////////// Driver.add

void Driver::addFunction(string *id, Symtable::Type type) {
  
  Function *func = new Function(*id, variables, type);
  
  if (!symtable.insert(func)) {
    ERROR(Error::SEM, "Function " << *id << " already exists.")
    throwError();
    delete func;
  }
  
  DEBUG("Created function " << *id)
  delete id;
  freeVariables(variables);
}

void Driver::addParam(string *id, Symtable::Type type) {
  Variable *var = new Variable(*id, type);
  variables.push_back(var);
  
  delete id;
}
  
void Driver::addType(Symtable::Type type) {
  Variable *var = new Variable(type);
  variables.push_back(var);
}

void Driver::addId(string *id) {
  Variable *var = new Variable(*id, Symtable::VOID);
  variables.push_back(var);
  
  delete id;
}

//////////////////////////////////// Driver.get

Variable* Driver::getTempVariable(Symtable::Type type) {
  
  std::stringstream stream;
  stream << "~temp" << tempCount++;
  string *id = new string(stream.str());
  Variable *var = new Variable(*id, type);
  
  symtable.insert(var);
  
  DEBUG("Created temp variable " << *id)
  
  delete id;
  return var;
}

//////////////////////////////////// Driver.enter

void Driver::enterFunc(string *id, Symtable::Type type) {
  
  Function *func = symtable.lookupFunction(*id);
  
  // function was declared, check headers
  if (func != NULL) {
  
    if (func->type != type) {
      ERROR(Error::SEM, "Function " << *id << " has different return type.")
      throwError();
    }
    
    if (!func->checkParameters(variables)) {
      ERROR(Error::SEM, "Function " << *id << " has different parameters types.")
      throwError();
   }
   freeVariables(func->params);
   func->params.splice(func->params.begin(), variables);

  }

  // function was not declared, add the function  
  else {  
    func = new Function(*id, variables, type);
    symtable.insert(func);
    
    DEBUG("Created new function " << *id)
  }
  
  // enter block
  symtable.enterBlock(func);
   
  // add parameters to variable table
  for (list<Variable*>::iterator i = func->params.begin(); i != func->params.end(); ++i) {
    
    if (!symtable.insert(*i)) {
      // TODO check params before !!!
      ERROR(Error::SEM, "Function parameter " << (*i)->id << " already exists.")
      throwError();
    }
  }    
  
  freeVariables(variables);
}

void Driver::leaveFunc(string *id, InstructionList *l) {
  
  Function *func = symtable.actualFunction;
  func->instructions.splice(func->instructions.begin(), *l);
  
  symtable.leaveBlock();
  
  freeInstructions(*l);
  delete l;
  delete id;
}

  
void Driver::enterBlock() {
  symtable.enterBlock();
}
  
void Driver::leaveBlock() {
  symtable.leaveBlock();
}

//////////////////////////////////// Driver.generateExpression

Expression* Driver::genExprInt(int ival) {

  // create var
  Variable *var = getTempVariable(Symtable::TINT);
  var->ival = ival;
  
  // create inst
  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);
  
  DEBUG("Generate instr " << *i)
  
  // create expr
  return new Expression(var, inst);

}

Expression* Driver::genExprChar(string *sval) {

  // create var
  Variable *var = getTempVariable(Symtable::TCHAR);
  var->sval = *sval;

  // create inst
  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);

  DEBUG("Generate instr " << *i)

  // create expr  
  delete sval;   
  return new Expression(var, inst);

}

Expression* Driver::genExprStr(string *sval) {

  // create temp
  Variable *var = getTempVariable(Symtable::TSTRING);
  var->sval = *sval;
  
  // create inst
  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);
  
  DEBUG("Generate instr " << *i) 
  
  // create expr
  delete sval;  
  return new Expression(var, inst);
}
  
Expression* Driver::genExprVar(string *id) {

  Expression *expr = NULL;
  InstructionList *inst = new InstructionList();
  
  // check var
  Variable *var = symtable.lookupVariable(*id);
  
  if (var == NULL) {
  
    // variable is not defined
    ERROR(Error::SEM, "Variable " << *id << " is not defined.")
    throwError();
    
    // try to guess the type
    Variable *tvar = getTempVariable(Symtable::TINT);    
    expr = new Expression(tvar, inst);
    
  }
  else {
    // create temp
    Variable *tvar = getTempVariable(var->type);
  
    // create inst
    AssignmentInst *i = new AssignmentInst();
    i->var = var;
    i->result = tvar;
  
    InstructionList *inst = new InstructionList();
    inst->push_back(i);
    
    // create expr
    expr = new Expression(var, inst);
    DEBUG("Generate instr " << *i)    
  }
  
  delete id; 
  return expr;
}

Expression* Driver::genExprFce(string *id, ExpressionList *lexpr) {

  Expression *expr = NULL;
  InstructionList *inst = new InstructionList();
  
  // check func
  Function *func = symtable.lookupFunction(*id);
  
  if (func == NULL) {
  
    // function is not defined
    ERROR(Error::SEM, "Function " << *id << " is not defined.")
    throwError();
    
    // try to guess the type
    Variable *tvar = getTempVariable(Symtable::TINT);    
    expr = new Expression(tvar, inst);
  }
  else {
  
    // create temp
    Variable *tvar = getTempVariable(func->type);
   
    // create inst
    CallInst *i = new CallInst();
    i->fce = func;
    i->result = tvar;
  
    while (!lexpr->empty()) {
   
      // add var 
      i->args.push_back(lexpr->front()->var);
    
      // join inst
      inst = genInstJoin(inst, &lexpr->front()->inst);
    
      // pop
      lexpr->pop_front();
    }
  
    inst->push_back(i);
    DEBUG("Generate instr " << *i)
    
    // create expr
    expr = new Expression(tvar, inst);
  }
  
  freeExpressions(*lexpr);
  delete lexpr; 
  delete id;
   
  return expr;
}

Expression* Driver::genExprCast(Expression *expr, Symtable::Type type) {

  Expression *result = NULL;
  
  // check cast 
  bool ctos = (expr->var->type == Symtable::TCHAR && type == Symtable::TSTRING);
  bool ctoi = (expr->var->type == Symtable::TCHAR && type == Symtable::TINT);
  bool itoc = (expr->var->type == Symtable::TINT  && type == Symtable::TCHAR);
  
  if (!ctos && !ctoi && !itoc) {
    ERROR(Error::SEM, "Impossible cast operation.")
    throwError();
  }

  // create temp
  Variable *tvar = getTempVariable(type);
  
  // create inst
  CastInst *i = new CastInst();
  i->var = expr->var;
  i->result = tvar;
  i->type = type;
  
  InstructionList *inst = genInstJoin(new InstructionList(), &expr->inst);
  inst->push_back(i);
  
  DEBUG("Generate instr " << *i)
  
  // create expr
  result = new Expression(tvar, inst);
  
  delete expr;
  return result;
}

Expression* Driver::genExprOp(Expression *expr1, Expression *expr2, Symtable::Operator op) {
  
  Expression *result = NULL;
  
  // check operands
  bool check = false;
  
  switch(op) {
  
    case Symtable::ADD: 
    case Symtable::SUB:
    case Symtable::MUL:
    case Symtable::DIV:
    case Symtable::MOD: 
    case Symtable::AND:
    case Symtable::OR:  check =    expr1->var->type == Symtable::TINT 
                                && expr2->var->type == Symtable::TINT;
                        break;
    case Symtable::LT:
    case Symtable::LTE:
    case Symtable::GT:
    case Symtable::GTE:
    case Symtable::EQ:  
    case Symtable::NEQ: check = expr1->var->type == expr2->var->type;
                        break;
              
    case Symtable::NEG: check = expr1->var->type == Symtable::TINT;
                        break;
  }
  
  if (!check) {
    ERROR(Error::SEM, "Wrong types of operands.")
    throwError();
  }

  // create temp
  Variable *tvar = getTempVariable(Symtable::TINT);
  
  // create inst
  ExpressionInst *i = new ExpressionInst();
  InstructionList *inst = new InstructionList();

  i->var1 = expr1->var;
  i->result = tvar;
  i->op = op;
  
  inst = genInstJoin(inst, &expr1->inst);

  if (op != Symtable::NEG) {
    
    i->var2 = expr2->var;
    inst = genInstJoin(inst, &expr2->inst);
    
  }
  
  inst->push_back(i);
  DEBUG("Generate instr " << *i)

  // create expr  
  result = new Expression(tvar, inst);

  delete expr1;
  delete expr2;
  
  return result;
}

ExpressionList* Driver::genExprEmpty() {
  return new ExpressionList();
}

ExpressionList* Driver::genExprList(Expression *e) {
  
  ExpressionList *l = new ExpressionList();
  l->push_front(e);
  return l;
}

ExpressionList* Driver::genExprJoin(Expression *e, ExpressionList *l) {
  l->push_front(e);
  return l;
}

//////////////////////////////////// Driver.generate

InstructionList* Driver::genInstEmpty() {
  return new InstructionList();
}

InstructionList* Driver::genInstJoin(InstructionList *l1, InstructionList *l2) {
  l1->splice(l1->end(), *l2);
  return l1;
}

InstructionList* Driver::genVariables(Symtable::Type type) {
 
  Variable *var = NULL;
  
  while (!variables.empty()) {
    var = variables.front();    
    var->type = type;

    if (!symtable.insert(var)) {
      ERROR(Error::SEM, "Variable " << var->id << " is already defined.");
      throwError();
      
      delete var;
    }    
    
    variables.pop_front();
  } 
  
  return new InstructionList();
}

InstructionList* Driver::genAssignment(string *id, Expression *expr) {

  InstructionList *inst = new InstructionList();
  Variable *var = symtable.lookupVariable(*id);
  
  if (var == NULL) {
    ERROR(Error::SEM, "Variable " << *id << " is not defined.");
    throwError();
  }
  else if (var->type != expr->var->type) {
    ERROR(Error::SEM, "Variable " << *id << " has unexpected type.");
    throwError();  
  }
  else {
    AssignmentInst *i = new AssignmentInst();
    i->var = expr->var;
    i->result = var;
  
    inst = genInstJoin(new InstructionList(), &expr->inst);
    inst->push_back(i);
    
    DEBUG("Generate instr " << *i)
  }
  
  delete id;
  delete expr;
  
  return inst;
}

InstructionList* Driver::genCall(string *id, ExpressionList *lexpr) {

  InstructionList *inst = new InstructionList(); 
  Function *fce = symtable.lookupFunction(*id);
  
  if (fce == NULL) {
    ERROR(Error::SEM, "Function " << *id << " is not defined.");
    throwError();
  }
  else if (!fce->checkParameters(*lexpr)) {
    ERROR(Error::SEM, "Function " << *id << " expects different parameters.");
    throwError();  
  }
  else {
  
    CallInst *i = new CallInst();
    i->fce = fce;
    i->result = NULL;
  
    while (!lexpr->empty()) {
   
      // add var 
      i->args.push_back(lexpr->front()->var);
    
      // join inst
      inst = genInstJoin(inst, &lexpr->front()->inst);
    
      // pop
      lexpr->pop_front();
    }
  
    inst->push_back(i);  
    DEBUG("Generate instr " << *i)
  }
  
  freeExpressions(*lexpr);
  delete lexpr;
  delete id;
    
  return inst;
}

InstructionList* Driver::genReturn(Expression *expr) {

  InstructionList *inst = new InstructionList();
  
  if (symtable.actualFunction->type != expr->var->type) {
    ERROR(Error::SEM, "Function " << symtable.actualFunction->id << " returns different type.");
    throwError();   
  }
  else {
    ReturnInst *i = new ReturnInst();
    i->result = expr->var;
  
    inst = genInstJoin(inst, &expr->inst);
    inst->push_back(i);
    DEBUG("Generate instr " << *i)
  }
  
  delete expr;
  return inst;
}

InstructionList* Driver::genWhile(Expression *expr, InstructionList *l) {

  InstructionList *inst = new InstructionList();

  if (expr->var->type != Symtable::TINT) {
    ERROR(Error::SEM, "Type of while condition is not integer.");
    throwError(); 
  }
  else {
    Label *start = new Label();
    Label *end = new Label();
  
    JumpFalseInst *jumpif = new JumpFalseInst();
    jumpif->cond = expr->var;
    jumpif->label = end;
  
    JumpInst *jump = new JumpInst(); 
    jump->label = start;
  
    inst->push_back(start);
    inst = genInstJoin(inst, &expr->inst);
    inst->push_back(jumpif);
    inst = genInstJoin(inst, l);
    inst->push_back(jump);
    inst->push_back(end);   
    
    DEBUG("Generate instr " << *start)
    DEBUG("Generate instr " << *jumpif)
    DEBUG("Generate instr " << *jump)
    DEBUG("Generate instr " << *end)
  }
  
  freeInstructions(*l);
  delete l;
  delete expr;

  return inst;
}

InstructionList* Driver::genCondition(Expression *expr, InstructionList *l1, InstructionList *l2) {

  InstructionList *inst = new InstructionList();

  if (expr->var->type != Symtable::TINT) {
    ERROR(Error::SEM, "Type of if condition is not integer.");
    throwError();
  }
  else {
    Label *middle = new Label();
    Label *end   = new Label();
  
    JumpFalseInst *jumpif = new JumpFalseInst();
    jumpif->cond = expr->var;
    jumpif->label = middle;
  
    JumpInst *jump = new JumpInst(); 
    jump->label = end;

    inst = genInstJoin(inst, &expr->inst);
    inst->push_back(jumpif);
    inst = genInstJoin(inst, l1);
    inst->push_back(jump);
    inst->push_back(middle);
    inst = genInstJoin(inst, l2);
    inst->push_back(end);
    
    DEBUG("Generate instr " << *jumpif)
    DEBUG("Generate instr " << *jump)
    DEBUG("Generate instr " << *middle)
    DEBUG("Generate instr " << *end)
  }
  
  freeInstructions(*l1);
  freeInstructions(*l2);
  delete l1;
  delete l2;
  delete expr;
  
  return inst;
}

  
/* end of file */
