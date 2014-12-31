/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: instruction.cpp
 * Classes for instructions of three address code.
 */

#include "instruction.h"

//////////////////////////////////// Label

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

//////////////////////////////////// ExpressionInst

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

//////////////////////////////////// CastInst

string CastInst::str() {
  std::stringstream stream;
  stream << result->id << " = (" << Symtable::str(type) << ") " << var->id;
  return stream.str();
}

//////////////////////////////////// LoadInst

string LoadInst::str() {
  std::stringstream stream;
  stream << result->id << " = " << Symtable::str(result->type);
  return stream.str();
}

//////////////////////////////////// AssignmentInst

string AssignmentInst::str() {
  std::stringstream stream;
  stream << result->id << " = " << var->id;
  return stream.str();
}

//////////////////////////////////// JumpInst

string JumpInst::str() {
  std::stringstream stream;
  stream << "jump to " << label->id;
  return stream.str();
}

//////////////////////////////////// JumpFalseInst

string JumpFalseInst::str() {
  std::stringstream stream;
  stream << "if not " << cond->id << " jump to " << label->id;
  return stream.str();
}

//////////////////////////////////// CallInst

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

//////////////////////////////////// ReturnInst

string ReturnInst::str() {
  std::stringstream stream;
  stream << "return " << result->id;
  return stream.str();
}

/* end of file */
