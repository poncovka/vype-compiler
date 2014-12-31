/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */

#ifndef GENERATOR_H
#define	GENERATOR_H

#include <string>
#include <typeinfo>

#include "symtable.h"
#include "Stack.h"

using namespace std;

class Generator {
public:
	Generator();
	string run(FunctionTable functions);
	string generate(Label i);
	string generate(ExpressionInst i); // result = a op b
	string generate(CastInst i); // result = (type) a
	string generate(LoadInst i); // a = value
	string generate(JumpInst i); // goto a
	string generate(JumpFalseInst i); // if not a then goto b
	string generate(CallInst i); // a = func(b,c)
	string generate(ReturnInst i); // return a
	string allocateVariables(list<VariableTable*> var_table);
//	isAddressable(Instruction i);
private:
	Stack stack;
	map<Variable*, string> address_table;

};

#endif	/* GENERATOR_H */

