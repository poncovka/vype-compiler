/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: instruction.h
 * Classes for instructions of three address code.
 */

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "symtable.h"
#include "generator.h"

//////////////////////////////////// base class



//////////////////////////////////// label

class Label : public Instruction {
private:
	static int maxid;

public:
	string id;
	string str();

	Label();
	Label(const Label &label);
  virtual Instruction* ptr() {return this;}
};

//////////////////////////////////// result = a op b

class ExpressionInst : public Instruction {
public:
	Variable *var1;
	Variable *var2;
	Variable *result;
	Symtable::Operator op;
	string str();
	
	ExpressionInst* ptr() {return this;}
};

//////////////////////////////////// result = (type) a

class CastInst : public Instruction {
public:
	Variable *var;
	Variable *result;
	Symtable::Type type;
	string str();

	CastInst* ptr() {return this;}
};

//////////////////////////////////// a = value

class LoadInst : public Instruction {
public:
	Variable *result;
	string str();

	LoadInst* ptr() {return this;}
};

//////////////////////////////////// a = b;

class AssignmentInst : public Instruction {
public:
	Variable *var;
	Variable *result;
	string str();
};

//////////////////////////////////// goto a

class JumpInst : public Instruction {
public:
	Label *label;
	string str();

};

//////////////////////////////////// if not a then goto b

class JumpFalseInst : public Instruction {
public:
	Variable *cond;
	Label *label;
	string str();

};

//////////////////////////////////// a = func(b,c)

class CallInst : public Instruction {
public:
	Function *fce;
	list<Variable*> args;
	Variable *result;
	string str();

	~CallInst();
};

//////////////////////////////////// return a

class ReturnInst : public Instruction {
public:
	Variable *result;
	string str();

};

#endif
