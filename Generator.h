/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */

#ifndef GENERATOR_H
#define	GENERATOR_H

#include <string>
#include <typeinfo>

#include "symtable.h"

using namespace std;

class Generator {
public:
	Generator();
	run();
	generate(Label i);
	generate(ExpressionInst i); // result = a op b
	generate(CastInst i); // result = (type) a
	generate(LoadInst i); // a = value
	generate(JumpInst i); // goto a
	generate(JumpFalseInst i); // if not a then goto b
	generate(CallInst i); // a = func(b,c)
	generate(ReturnInst i); // return a
private:
	Stack stack;

};

#endif	/* GENERATOR_H */

