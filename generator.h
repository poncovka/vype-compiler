/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: generator.h
 * Generate assembly.
 */

#ifndef GENERATOR_H
#define	GENERATOR_H

#include <string>
#include <typeinfo>
#include "symtable.h"

using namespace std;

class Stack {
public:
	int sp;
	int fp;
	
	Stack(unsigned size);
	void push(unsigned i);
	void pop(unsigned i);

private:
	int size;
};

class Generator {
public:
	Generator();
	string run(FunctionTable functions);
	string generate(Label& i);
	string generate(ExpressionInst& i); // result = a op b
	string generate(CastInst& i); // result = (type) a
	string generate(LoadInst& i); // a = value
	string generate(JumpInst& i); // goto a
	string generate(JumpFalseInst& i); // if not a then goto b
	string generate(CallInst& i); // a = func(b,c)
	string generate(ReturnInst& i); // return a
	string allocateVariables(list<VariableTable*> var_table);
//	isAddressable(Instruction i);
private:
	Stack stack;
	map<Variable*, string> address_table;

};




#endif	/* GENERATOR_H */

