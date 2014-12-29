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

//////////////////////////////////// Driver.add

void Driver::addFunction(string *id, Symtable::Type type) {
  
  Function *func = new Function(*id, varList, type);
  if (!symtable.insert(func)) {
    // ERROR
  }
  
  delete id;
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
        break;
      }
      
      var = func->params.front();    
      if (var->type != (*i)->type) {
        // ERROR
      }
      
      func->params.pop_front();
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
}

void Driver::leaveFunc(string *id, InstructionList *l) {
  
  Function *func = symtable.actualFunction;
  func->instructions.splice(func->instructions.begin(), *l);
  
  symtable.leaveBlock();
  
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

  Variable *var = getTempVariable(Symtable::TINT);
  var->ival = ival;
  
  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);
   
  return new Expression(var, inst);

}

Expression* Driver::genExprChar(string *sval) {

  Variable *var = getTempVariable(Symtable::TCHAR);
  var->sval = *sval;

  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);
   
  delete sval; 
  return new Expression(var, inst);

}

Expression* Driver::genExprStr(string *sval) {

  Variable *var = getTempVariable(Symtable::TSTRING);
  var->sval = *sval;
  
  LoadInst *i = new LoadInst();
  i->result = var;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);

  delete sval;   
  return new Expression(var, inst);

}
  
Expression* Driver::genExprVar(string *id) {

  Variable *var = symtable.lookupVariable(*id);
  if (var == NULL) {
    // ERROR
  }
  
  Variable *tvar = getTempVariable(var->type);
  
  AssignmentInst *i = new AssignmentInst();
  i->var = var;
  i->result = tvar;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(i);
   
  delete id; 
  return new Expression(var, inst);
}

Expression* Driver::genExprFce(string *id, ExpressionList *lexpr) {

  Function *func = symtable.lookupFunction(*id);
  if (func == NULL) {
    // ERROR
  }
  
  Variable *tvar = getTempVariable(func->type);
   
  CallInst *i = new CallInst();
  i->fce = func;
  i->result = tvar;
 
  InstructionList *inst = new InstructionList();
  
    while (!lexpr->empty()) {
   
      // add var 
      i->args.push_back(lexpr->front()->var);
    
      // join inst
      inst = genInstJoin(inst, &lexpr->front()->inst);
    
      // pop
      lexpr->pop_front();
    }
  
  inst->push_back(i);
  
  delete id;
  delete lexpr; 
   
  return new Expression(tvar, inst);
}

Expression* Driver::genExprCast(Expression *expr, Symtable::Type type) {

  bool ctos = (expr->var->type == Symtable::TCHAR && type == Symtable::TSTRING);
  bool ctoi = (expr->var->type == Symtable::TCHAR && type == Symtable::TINT);
  bool itoc = (expr->var->type == Symtable::TINT  && type == Symtable::TCHAR);
  
  if (!ctos && !ctoi && !itoc) {
    // ERROR
  }
  
  Variable *tvar = getTempVariable(type);
  
  CastInst *i = new CastInst();
  i->var = expr->var;
  i->result = tvar;
  i->type = type;
  
  InstructionList *inst = genInstJoin(new InstructionList(), &expr->inst);
  inst->push_back(i);
  
  delete expr;
  return new Expression(tvar, inst);
}

Expression* Driver::genExprOp(Expression *expr1, Expression *expr2, Symtable::Operator op) {
  
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
    // ERROR
  }


  Variable *tvar = getTempVariable(Symtable::TINT);
  
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

  delete expr1;
  delete expr2;
  
  return new Expression(tvar, inst);

}

ExpressionList* Driver::genExprEmpty() {

  return new ExpressionList();;
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
  
  while (!varList.empty()) {
    var = varList.front();    
    var->type = type;

    if (!symtable.insert(var)) {
      // ERROR
    }    
    
    varList.pop_front();
  } 
  
  return genInstEmpty();
}

InstructionList* Driver::genAssignment(string *id, Expression *expr) {

  Variable *var = getVariable(id);
  
  if (var == NULL || var->type != expr->var->type) {
    // ERROR
  }
  
  AssignmentInst *i = new AssignmentInst();
  i->var = expr->var;
  i->result = var;
  
  InstructionList *inst = genInstJoin(new InstructionList(), &expr->inst);
  inst->push_back(i);
  
  delete id;
  delete expr;
  
  return inst;
}

InstructionList* Driver::genCall(string *id, ExpressionList *lexpr) {

  Function *fce = getFunction(id);
  
  if (fce == NULL) {
    // ERROR
  }
  
  // TODO check params
 
 CallInst *i = new CallInst();
 i->fce = fce;
 i->result = NULL;
 
 InstructionList *inst = new InstructionList();
  
  while (!lexpr->empty()) {
   
    // add var 
    i->args.push_back(lexpr->front()->var);
    
    // join inst
    inst = genInstJoin(inst, &lexpr->front()->inst);
    
    // pop
    lexpr->pop_front();
  }
  
  inst->push_back(i);
  
  delete id;
  delete lexpr;
  
  return inst;
}

InstructionList* Driver::genReturn(Expression *expr) {

  ReturnInst *i = new ReturnInst();
  i->result = expr->var;
  
  InstructionList *inst = genInstJoin(new InstructionList(), &expr->inst);
  inst->push_back(i);
  
  delete expr;

  return inst;
}

InstructionList* Driver::genWhile(Expression *expr, InstructionList *l) {

  if (expr->var->type != Symtable::TINT) {
    // ERROR
  }
  
  Label *start = new Label();
  Label *end = new Label();
  
  JumpFalseInst *jumpif = new JumpFalseInst();
  jumpif->cond = expr->var;
  jumpif->label = end;
  
  JumpInst *jump = new JumpInst(); 
  jump->label = start;
  
  InstructionList *inst = new InstructionList();
  inst->push_back(start);
  inst = genInstJoin(inst, &expr->inst);
  inst->push_back(jumpif);
  inst = genInstJoin(inst, l);
  inst->push_back(jump);
  inst->push_back(end);
  
  delete expr;
  delete l;
  
  return inst;
}

InstructionList* Driver::genCondition(Expression *expr, InstructionList *l1, InstructionList *l2) {

  if (expr->var->type != Symtable::TINT) {
    // ERROR
  }
  
  Label *middle = new Label();
  Label *end   = new Label();
  
  JumpFalseInst *jumpif = new JumpFalseInst();
  jumpif->cond = expr->var;
  jumpif->label = middle;
  
  JumpInst *jump = new JumpInst(); 
  jump->label = end;
  
  InstructionList *inst = new InstructionList();
  inst = genInstJoin(inst, &expr->inst);
  inst->push_back(jumpif);
  inst = genInstJoin(inst, l1);
  inst->push_back(jump);
  inst->push_back(middle);
  inst = genInstJoin(inst, l2);
  inst->push_back(end);
  
  delete expr;
  delete l1;
  delete l2;
  
  return inst;
}

  
/* end of file */
