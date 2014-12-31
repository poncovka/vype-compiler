/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */

#include "Generator.h"
#include "symtable.h"
#include "generator/instructions/Instruction.h"
#include "Stack.h"

Generator::Generator() {
	stack = new Stack(4096);
}

Generator::run() {

	for (Instruction* i : InstructionList) {
		generate(i);
	}
}

Generator::generate(Label i) {
}

Generator::generate(ExpressionInst i) { // result = a op b

	string rd, rs, rt;
	string assembler = "";

	rd = address_table.find(i.result)->second;
	rs = address_table.find(i.var1)->second;
	rt = address_table.find(i.var2)->second;

	if (i.op == Symtable::ADD) {
		// rd ← rs + rt ~ ADD rd, rs, rt
		assembler = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"ADD $3,$4,$5\n" +
				"SW $3," + rd + "\n";
	} else if (i.op == Symtable::SUB) {
		// rd ← rs - rt ~ SUB rd, rs, rt
		assembler = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"SUB $3,$4,$5\n" +
				"SW $3," + rd + "\n";
	} else if (i.op == Symtable::MUL) {
		// rd ← rs × rt ~ MUL rd, rs, rt
		assembler = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"MUL $3,$4,$5\n" +
				"SW $3," + rd + "\n";
	} else if (i.op == Symtable::DIV || i.op == Symtable::MOD) {
		// (HI, LO) ← rs / rt ~ DIV rs, rt
		assembler = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"DIV $4,$5\n";

		if (i.op == Symtable::DIV) {
			assembler += "MFLO $3\n" +
					"SW $3," + rd + "\n";
		} else if (i.op == Symtable::MOD) {
			assembler += "MFHI $3\n" +
					"SW $3," + rd + "\n";
		}
	} else if (i.op == Symtable::LT) {

	} else if (i.op == Symtable::LTE) {

	} else if (i.op == Symtable::GT) {

	} else if (i.op == Symtable::GTE) {

	} else if (i.op == Symtable::EQ) {

	} else if (i.op == Symtable::NEQ) {

	} else if (i.op == Symtable::AND) {

	} else if (i.op == Symtable::OR) {

	} else if (i.op == Symtable::NEG) {

	}

	return assembler;
}

Generator::generate(CastInst i) { // result = (type) a

}

Generator::generate(LoadInst i) { // a = value

}

Generator::generate(JumpInst i) { // goto a

}

Generator::generate(JumpFalseInst i) { // if not a then goto b

}

Generator::generate(CallInst i) { // a = func(b,c)

}

Generator::generate(ReturnInst i) { // return a

}

Generator::isAddressable(Instruction i) {
	if (address_table.find(i) == address_table.end()) {

		return "";
	}
}

Generator::allocateVariables(list<VariableTable*> variables) {

	for (list<VariableTable*>::iterator i = variables.begin(); i != variables.end(); ++i) {
		VariableTable &variables = **i;

		for (map<string, Variable*>::iterator j = variables.symtable.begin(); j != variables.symtable.end(); ++j) {
			Variable &variable = *(j->second);
			address_table.insert(std::pair<Variable*, string>(variable, stack.sp));

			if (variable.type == Symtable::TINT || variable.type == Symtable::TSTRING) {
				stack.push(4);
			} else if (variable.type == Symtable::TCHAR) {
				stack.push(1);
			}
		}
	}
}
