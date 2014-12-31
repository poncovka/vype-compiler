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

void Driver::parse(FILE *f, const string &fname) {

  filename = fname;
  extern FILE *yyin;
  yyin = f;

  parser.parse();
  
  IFDEBUG(debug())
  
  // TODO check main function
  // TODO check if all functions are declared
}

void Driver::errorLex(yy::location const &loc, const string &msg) {
  ERROR(Error::LEX, "lexical error, " << msg << " at " << loc)
}

void Driver::errorSyn(yy::location const &loc, const string &msg) {
  ERROR(Error::SYN, msg << " at " << loc)
}

//////////////////////////////////// Driver.add

void Driver::addDeclaration(string *id, Symtable::Type type) {
  
  Function *func = new Function(*id, variables, type, false);
  
  if (!symtable.insert(func)) {
    ERROR(Error::SEM, "semantic error, function " << *id << " already exists")
    delete func;
  }
  else {
    DEBUG("Created function " << *id)
  }
  
  delete id;
  variables.clear();
}

void Driver::addParam(string *id, Symtable::Type type) {
  Variable *var = new Variable(*id, type);
  variables.push_front(var);
  
  delete id;
}
  
void Driver::addType(Symtable::Type type) {
  Variable *var = new Variable(type);
  variables.push_front(var);
}

void Driver::addId(string *id) {
  Variable *var = new Variable(*id, Symtable::VOID);
  variables.push_front(var);
  
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
  
  // find function
  Function *func = symtable.lookupFunction(*id);
  if (func != NULL) {
  
    // function was defined    
    if (func->isdef) {
    
      ERROR(Error::SEM, "semantic error, function " << *id 
      << " was already defined")
      
      func->clear();
      
      func->params.splice(func->params.begin(), variables);
      func->isdef = true;
       
    }
    
    // function was declared
    else {
  
      if (func->type != type) {
        ERROR(Error::SEM, "semantic error, function " << *id 
        << " has declared different return type")
      }
    
      if (!func->checkParameters(variables)) {
        ERROR(Error::SEM, "semantic error, function " << *id 
        << " has declared different parameters")
      }

      freeVariables(func->params);
      func->params.splice(func->params.begin(), variables);
      func->isdef = true;
    }
  }

  // function does not exist
  else {    
    func = new Function(*id, variables, type, true);
    symtable.insert(func);
    DEBUG("Created new function " << *id)
  }
  
  // enter block
  symtable.enterBlock(func);
   
  // add parameters to variable table  
  for (list<Variable*>::iterator i = func->params.begin(); i != func->params.end();) {
    
    if (!symtable.insert(*i)) {
    
      // variable exists
      ERROR(Error::SEM, "semantic error, function parameter " << (*i)->id 
      << " was already defined")
      
      // remove variable from param list
      list<Variable*>::iterator j = i;
      Variable *var = *j;
      ++i;
      func->params.erase(j);
      //delete var;
      
      continue;
    }
    
    ++i;
  }   
  
  variables.clear(); 
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

  Expression *expr = new Expression();

  // create var
  Variable *tvar = getTempVariable(Symtable::TINT);
  tvar->ival = ival;
  
  // create inst
  LoadInst *i = new LoadInst();
  i->result = tvar;
  
  // create expr
  expr->inst.push_back(i);
  expr->var = tvar;
  
  return expr;

}

Expression* Driver::genExprChar(string *sval) {

  Expression *expr = new Expression();

  // create var
  Variable *tvar = getTempVariable(Symtable::TCHAR);
  tvar->sval = *sval;

  // create inst
  LoadInst *i = new LoadInst();
  i->result = tvar;

  // create expr  
  expr->inst.push_back(i);
  expr->var = tvar;
  
  delete sval;   
  return expr;

}

Expression* Driver::genExprStr(string *sval) {

  Expression *expr = new Expression();

  // create temp
  Variable *tvar = getTempVariable(Symtable::TSTRING);
  tvar->sval = *sval;
  
  // create inst
  LoadInst *i = new LoadInst();
  i->result = tvar;
  
  InstructionList inst;
  inst.push_back(i);
  
  // create expr  
  expr->inst.push_back(i);
  expr->var = tvar;
  
  delete sval;  
  return expr;
}
  
Expression* Driver::genExprVar(string *id) {

  Expression *expr = new Expression();
  
  // check var
  Variable *var = symtable.lookupVariable(*id);
  
  if (var == NULL) {
  
    // variable is not defined
    ERROR(Error::SEM, "semantic error, variable " << *id << " is not defined")
        
    // try to guess the type
    Variable *tvar = getTempVariable(Symtable::TINT);    
    expr->var = tvar;
    
  }
  else {
    // create temp
    Variable *tvar = getTempVariable(var->type);
  
    // create inst
    AssignmentInst *i = new AssignmentInst();
    i->var = var;
    i->result = tvar;
    
    // create expr
    expr->inst.push_back(i);
    expr->var = tvar;
  }
  
  delete id; 
  return expr;
}

Expression* Driver::genExprFce(string *id, ExpressionList *lexpr) {

  Expression *expr = new Expression();
  Function *func = symtable.lookupFunction(*id);
  
  if (func == NULL) {
    expr->var = getTempVariable(Symtable::TINT);    
  }
  else {
    expr->var = getTempVariable(func->type);
  }
  
  InstructionList* inst = genCall(id, lexpr);
  
  if (!inst->empty()) {
    CallInst *i = (CallInst*) inst->back();
    i->result = expr->var;
  }
  
  genInstJoin(&expr->inst, inst);
  delete inst;
  return expr;
}

Expression* Driver::genExprCast(Expression *expr, Symtable::Type type) {

  Expression *result = new Expression();
  
  // check cast 
  bool ctos = (expr->var->type == Symtable::TCHAR && type == Symtable::TSTRING);
  bool ctoi = (expr->var->type == Symtable::TCHAR && type == Symtable::TINT);
  bool itoc = (expr->var->type == Symtable::TINT  && type == Symtable::TCHAR);
  
  if (!ctos && !ctoi && !itoc) {
    ERROR(Error::SEM, "semantic error, impossible cast operation from " 
    << Symtable::str(expr->var->type) << " to " << Symtable::str(type));
  }

  // create temp
  Variable *tvar = getTempVariable(type);
  
  // create inst
  CastInst *i = new CastInst();
  i->var = expr->var;
  i->result = tvar;
  i->type = type;
    
  // create expr
  result->var = tvar;

  genInstJoin(&result->inst, &expr->inst);
  result->inst.push_back(i);
  
  delete expr;
  return result;
}

Expression* Driver::genExprOp(Expression *expr1, Expression *expr2, Symtable::Operator op) {
  
  Expression *result = new Expression();
  
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
    ERROR(Error::SEM, "semantic error, undefined expression " 
    << Symtable::str(expr1->var->type) << " " << Symtable::str(op) << " " 
    << ((op != Symtable::NEG) ? Symtable::str(expr2->var->type) : "") ) 
  }

  // create temp
  Variable *tvar = getTempVariable(Symtable::TINT);
  
  // create inst
  ExpressionInst *i = new ExpressionInst();
  i->var1 = expr1->var;
  i->result = tvar;
  i->op = op;

  // create expr  
  result->var = tvar;
  
  genInstJoin(&result->inst, &expr1->inst);
  
  if (op != Symtable::NEG) {
    
    i->var2 = expr2->var;
    genInstJoin(&result->inst, &expr2->inst);
    
  }
  
  result->inst.push_back(i);

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
      ERROR(Error::SEM, "semantic error, variable " << var->id 
      << " is already defined"); 
      
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
    ERROR(Error::SEM, "semantic error, cannot assign to undefined variable " << *id);
  }
  else if (var->type != expr->var->type) {
  
    ERROR(Error::SEM, "semantic error, cannot assign " << Symtable::str(expr->var->type) 
    << " to variable " << *id << " of type " << Symtable::str(var->type));    
  }
  else {
    AssignmentInst *i = new AssignmentInst();
    i->var = expr->var;
    i->result = var;
  
    inst = genInstJoin(inst, &expr->inst);
    inst->push_back(i);
    
  }
  
  delete id;
  delete expr;
  return inst;
}

InstructionList* Driver::genCall(string *id, ExpressionList *lexpr) {

  InstructionList *inst = new InstructionList(); 
  Function *fce = symtable.lookupFunction(*id);
  
  if (fce == NULL) {
    ERROR(Error::SEM, "semantic error, cannot call undefined function " << *id);
  }
  else if (!fce->checkParameters(*lexpr)) {
    ERROR(Error::SEM, "semantic error, cannot call function " << *id 
    << " with these arguments");
  }
  else {
  
    CallInst *i = new CallInst();
    i->fce = fce;
    i->result = NULL;
  
    for (ExpressionList::iterator ie = lexpr->begin(); ie != lexpr->end(); ++ie) {
    
      Expression *e = *ie;
      
       // add var 
      i->args.push_back(e->var);
    
      // join inst
      genInstJoin(inst, &e->inst); 
    }
  
    inst->push_back(i);  
  }
  
  freeExpressions(*lexpr);
  delete lexpr;
  delete id; 
  return inst;
}

InstructionList* Driver::genReturn(Expression *expr) {

  InstructionList *inst = new InstructionList();
  
  if (symtable.actualFunction->type != expr->var->type) {
    ERROR(Error::SEM, "semantic error, function " << symtable.actualFunction->id 
    << " does not return " << Symtable::str(expr->var->type));     
  }
  else {
    ReturnInst *i = new ReturnInst();
    i->result = expr->var;
  
    inst = genInstJoin(inst, &expr->inst);
    inst->push_back(i);
  }
  
  delete expr;
  return inst;
}

InstructionList* Driver::genWhile(Expression *expr, InstructionList *l) {

  InstructionList *inst = new InstructionList();

  if (expr->var->type != Symtable::TINT) {
    ERROR(Error::SEM, "semantic error, while condition cannot be of type " 
    << Symtable::str(expr->var->type)); 
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
  }
  
  freeInstructions(*l);
  delete l;
  delete expr;
  return inst;
}

InstructionList* Driver::genCondition(Expression *expr, InstructionList *l1, InstructionList *l2) {

  InstructionList *inst = new InstructionList();

  if (expr->var->type != Symtable::TINT) {
    ERROR(Error::SEM, "semantic error, if condition cannot be of type "
    << Symtable::str(expr->var->type))
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
    
  }
  
  freeInstructions(*l1);
  freeInstructions(*l2);
  delete l1;
  delete l2;
  delete expr;
  return inst;
}

//////////////////////////////////// Driver.debug

void Driver::debug() {
  
  FunctionTable &functions = symtable.functions;
  
  for (map<string, Function*>::iterator i = functions.symtable.begin(); i != functions.symtable.end(); ++i) {
  
    Function &f = *(i->second);  
    std::cerr << "FUNCTION " << f.str() << std::endl << std::endl;
    
    for (list<VariableTable*>::iterator j = f.variables.begin(); j != f.variables.end(); ++j) {
    
      VariableTable &variables = **j;
      std::cerr << "VARIABLES block" << std::distance(f.variables.begin(), j) << std::endl;   

      for (map<string, Variable*>::iterator k = variables.symtable.begin(); k != variables.symtable.end(); ++k) {
      
        Variable &v = *(k->second);
        std::cerr << v.str() << std::endl;        
      } 
      std::cerr << std::endl;
    }
    
    std::cerr << "INSTRUCTIONS " << std::endl;
    for (list<Instruction*>::iterator l = f.instructions.begin(); l != f.instructions.end(); ++l) {
      std::cerr << (*l)->str()  << std::endl;
    }
    std::cerr << std::endl;
  }
  std::cerr << std::endl;
}

/* end of file */
