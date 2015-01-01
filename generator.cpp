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

Stack::Stack(unsigned size)
{
    this->size = size;
    sp = size;
    fp = size;
}

string Stack::push(unsigned i)
{
    sp -= i;

    stringstream mips;
    mips << "addi $sp,$sp,-" << i << endl;
    return mips.str();
}

string Stack::pop(unsigned i)
{
    sp += i;

    stringstream mips;
    mips << "addi $sp,$sp," << i << endl;
    return mips.str();
}

Generator::Generator() : stack(8192)
{
    stringstream ds;
    ds << endl << ".data" << endl;
    data = ds.str();
    data_counter = 0;
}

string Generator::run(FunctionTable& functions)
{
    stringstream mips, func, header;
    header << ".text" << endl <<
            ".org 0" << endl <<
            "li $sp," << stack.size << endl <<
            "li $fp," << stack.size << endl <<
            "jal main" << endl <<
            "BREAK" << endl;

    for (map<string, Function*>::iterator i = functions.symtable.begin(); i != functions.symtable.end(); ++i)
    {
        stack.fp = stack.size;
        stack.sp = stack.size;

        Function &f = *(i->second);
        allocateVariables(f.variables);

        for (list<Instruction*>::iterator l = f.instructions.begin(); l != f.instructions.end(); ++l)
        {
            func << string((*l)->generate(this));
            
            if (f.id == "main") {
                func << "jr $ra // Return" << endl;
            }
        }
    }

    mips << header.str() << func.str() << data << endl;
    return mips.str();
}

//Generator::isAddressable(Instruction i) {
//	if (address_table.find(i) == address_table.end()) {
//
//		return "";
//	}
//}

string Generator::allocateVariables(list<VariableTable*> variables)
{
    std::stringstream ss;

    for (list<VariableTable*>::iterator i = variables.begin(); i != variables.end(); ++i)
    {
        VariableTable &variables = **i;

        for (map<string, Variable*>::iterator j = variables.symtable.begin(); j != variables.symtable.end(); ++j)
        {
            Variable &variable = *(j->second);

            int offset = stack.sp - stack.fp;
            ss.str("");
            ss << offset << "($fp)";
            address_table.insert(make_pair(&variable, ss.str()));

            if (variable.type == Symtable::TINT || variable.type == Symtable::TSTRING)
            {
                stack.push(4); // 4B
            }
            else if (variable.type == Symtable::TCHAR)
            {
                stack.push(1); // 1B
            }
        }
    }

    return "a";
}
