/*
 * Project: VYPe 2014
 *
 * Authors: 
 *   Vendula Poncová, xponco00
 *   Martin Šifra, xsifra00
 * 
 * File: generator.cpp
 * Generate assembly.
 */

#include "generator.h"

Stack::Stack(unsigned size) {
	this->size = size;
	sp = size;
	fp = size;
}

void Stack::push(unsigned i) {
	sp -= i;
}

void Stack::pop(unsigned i) {
	sp += i; 
}

Generator::Generator() : stack(8192) {}

string Generator::run(FunctionTable functions) {
	string mips = ".text\n";

	for (map<string, Function*>::iterator i = functions.symtable.begin(); i != functions.symtable.end(); ++i) {
		Function &f = *(i->second);

		for (list<Instruction*>::iterator l = f.instructions.begin(); l != f.instructions.end(); ++l) {
		  Instruction *p = *l;
		  Instruction &inst = *p;
			generate(*l);
		}
	}

	return mips;
}

string Generator::generate(Label &i) {
	return i.str() + ":\n";
}

string Generator::generate(ExpressionInst &i) { // result = a op b
	string rd, rs, rt;
	string mips = "";

	rd = address_table.find(i.result)->second;
	rs = address_table.find(i.var1)->second;
	rt = address_table.find(i.var2)->second;

	if (i.op == Symtable::ADD) { // rd ← rs + rt ~ ADD rd, rs, rt
		mips = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"ADD $3,$4,$5\n" +
				"SW $3," + rd + "\n";

	} else if (i.op == Symtable::SUB) { // rd ← rs - rt ~ SUB rd, rs, rt
		mips = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"SUB $3,$4,$5\n" +
				"SW $3," + rd + "\n";

	} else if (i.op == Symtable::MUL) { // rd ← rs × rt ~ MUL rd, rs, rt
		mips = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"MUL $3,$4,$5\n" +
				"SW $3," + rd + "\n";

	} else if (i.op == Symtable::DIV || i.op == Symtable::MOD) { // (HI, LO) ← rs / rt ~ DIV rs, rt
		mips = "LW 4$," + rs + "\n" +
				"LW 5$," + rt + "\n" +
				"DIV $4,$5\n";

		if (i.op == Symtable::DIV) {
			mips += "MFLO $3\n" +
					"SW $3," + rd + "\n";
		} else if (i.op == Symtable::MOD) {
			mips += "MFHI $3\n" +
					"SW $3," + rd + "\n";
		}

	} else if (i.op == Symtable::LT) {
		mips = "LT\n";

	} else if (i.op == Symtable::LTE) {
		mips = "LTE\n";

	} else if (i.op == Symtable::GT) {
		mips = "GT\n";

	} else if (i.op == Symtable::GTE) {
		mips = "GTE\n";

	} else if (i.op == Symtable::EQ) {
		mips = "EQ\n";

	} else if (i.op == Symtable::NEQ) {
		mips = "NEQ\n";

	} else if (i.op == Symtable::AND) {
		mips = "AND\n";

	} else if (i.op == Symtable::OR) {
		mips = "OR\n";

	} else if (i.op == Symtable::NEG) {
		mips = "NEG\n";
	}

	return mips;
}

string Generator::generate(CastInst &i) { // result = (type) a
	string mips = "CAST\n";
	return mips;
}

string Generator::generate(LoadInst &i) { // a = value
	string mips = "LOAD\n";
	return mips;
}

string Generator::generate(JumpInst &i) { // goto a
	return "J " + i.str() + "\n";
}

string Generator::generate(JumpFalseInst &i) { // if not a then goto b
	string mips = "JF\n";
	return mips;
}

string Generator::generate(CallInst &i) { // a = func(b,c)
	string mips = "";
	// Jump s tím, že musím uložit na zásobník SP a FP a takové ty srandy.
	mips = "CALL " + i.str() + "\n";
	return mips;
}

string Generator::generate(ReturnInst &i) { // return a
	string mips = "RET\n";
	return mips;
}

//Generator::isAddressable(Instruction i) {
//	if (address_table.find(i) == address_table.end()) {
//
//		return "";
//	}
//}

string Generator::allocateVariables(list<VariableTable*> variables) {

	for (list<VariableTable*>::iterator i = variables.begin(); i != variables.end(); ++i) {
		VariableTable &variables = **i;

		for (map<string, Variable*>::iterator j = variables.symtable.begin(); j != variables.symtable.end(); ++j) {
			Variable &variable = *(j->second);
			address_table.insert(std::pair<Variable*, string>(variable, stack.sp)); // ToDo: Adresa by měla být $fp + ofset (asi)!

			if (variable.type == Symtable::TINT || variable.type == Symtable::TSTRING) {
				stack.push(4);
			} else if (variable.type == Symtable::TCHAR) {
				stack.push(1);
			}
		}
	}

	return "a";
}
