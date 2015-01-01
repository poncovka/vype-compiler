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
#include "error.h"

using namespace std;

class Stack {
public:
	int sp;
	int fp;
	
	Stack(unsigned size);
	string push(unsigned i);
	string pop(unsigned i);

private:
	int size;
};

class Generator {
public:
	Generator();
	string run(FunctionTable &functions);
	Stack stack;
	map<Variable*, string> address_table;
	string allocateVariables(list<VariableTable*> var_table);

//	isAddressable(Instruction i);

};

#endif	/* GENERATOR_H */

