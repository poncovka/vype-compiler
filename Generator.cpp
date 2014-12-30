/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */

#include "Generator.h"
#include "Stack.h"
#include "symtable.h"
#include "generator/instructions/Instruction.h"
#include <typeinfo>

Generator::Generator() {
	this->stack = new Stack();
}

Generator::run() {

	for (Instruction* i : InstructionList) {
		generate(i);
	}
}

Generator::generate(Label i) {
}

// result = a op b
Generator::generate(ExpressionInst i) {

}

// result = (type) a
Generator::generate(CastInst i) {

}

// a = value
Generator::generate(LoadInst i) {

}

// goto a
Generator::generate(JumpInst i) {

}

// if not a then goto b
Generator::generate(JumpFalseInst i) {

}

// a = func(b,c)
Generator::generate(CallInst i) {

}

// return a
Generator::generate(ReturnInst i) {

}
