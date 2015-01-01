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

Label::Label(const string &prefix)
{
    std::stringstream stream;
    stream << prefix << Label::maxid++;
    this->id = stream.str();
    stream.str("");
}

Label::Label(Function *f)
{
    std::stringstream stream;
    stream << "FUNC_" << f->id;
    this->id = stream.str();
    stream.str("");
}

Label::Label(const Label &label)
{
    this->id = label.id;
}

string Label::str()
{
    std::stringstream stream;
    stream << "label " << id;
    return stream.str();
}

string Label::generate(Generator* g)
{
    stringstream ss;
    ss << "\n" << id << ":\n";
    return ss.str();
}

//////////////////////////////////// ExpressionInst

string ExpressionInst::str()
{
    std::stringstream stream;

    if (op == Symtable::NEG)
    {
        stream << result->id << " = " << Symtable::str(op) << " " << var1->id;
    }
    else
    {
        stream << result->id << " = " << var1->id << " " << Symtable::str(op) << " " << var2->id;
    }

    return stream.str();
}

string ExpressionInst::generate(Generator* g)
{ // result = a op b
    string rd, rs, rt;
    string mips = "";

    rd = (g->address_table.find(result))->second;
    rs = (g->address_table.find(var1))->second;
    rt = (g->address_table.find(var2))->second;

    if (op == Symtable::ADD)
    { // rd ← rs + rt ~ ADD rd, rs, rt
        mips = "LW 4$, " + rs + "\n" +
                "LW 5$, " + rt + "\n" +
                "ADD $3, $4, $5\n" +
                "SW $3, " + rd + "\n";

    }
    else if (op == Symtable::SUB)
    { // rd ← rs - rt ~ SUB rd, rs, rt
        mips = "LW 4$, " + rs + "\n" +
                "LW 5$, " + rt + "\n" +
                "SUB $3, $4, $5\n" +
                "SW $3, " + rd + "\n";

    }
    else if (op == Symtable::MUL)
    { // rd ← rs × rt ~ MUL rd, rs, rt
        mips = "LW 4$, " + rs + "\n" +
                "LW 5$, " + rt + "\n" +
                "MUL $3, $4, $5\n" +
                "SW $3, " + rd + "\n";

    }
    else if (op == Symtable::DIV || op == Symtable::MOD)
    { // (HI, LO) ← rs / rt ~ DIV rs, rt
        mips = "LW 4$, " + rs + "\n" +
                "LW 5$, " + rt + "\n" +
                "DIV $4, $5\n";

        if (op == Symtable::DIV)
        {
            mips += "MFLO $3\n" +
                    string("SW $3, ") + rd + "\n";
        }
        else if (op == Symtable::MOD)
        {
            mips += "MFHI $3\n" +
                    string("SW $3, ") + rd + "\n";
        }

    }
    else if (op == Symtable::LT)
    {
        mips = "LT\n";

    }
    else if (op == Symtable::LTE)
    {
        mips = "LTE\n";

    }
    else if (op == Symtable::GT)
    {
        mips = "GT\n";

    }
    else if (op == Symtable::GTE)
    {
        mips = "GTE\n";

    }
    else if (op == Symtable::EQ)
    {
        mips = "EQ\n";

    }
    else if (op == Symtable::NEQ)
    {
        mips = "NEQ\n";

    }
    else if (op == Symtable::AND)
    {
        mips = "AND\n";

    }
    else if (op == Symtable::OR)
    {
        mips = "OR\n";

    }
    else if (op == Symtable::NEG)
    {
        mips = "NEG\n";
    }

    return mips;
}
//////////////////////////////////// CastInst

string CastInst::str()
{
    std::stringstream stream;
    stream << result->id << " = (" << Symtable::str(type) << ") " << var->id;
    return stream.str();
}

string CastInst::generate(Generator * g)
{ // result = (type) a
    string mips = "CAST\n";
    return mips;
}

//////////////////////////////////// LoadInst

string LoadInst::str()
{
    std::stringstream stream;
    stream << result->id << " = " << Symtable::str(result->type);
    return stream.str();
}

string LoadInst::generate(Generator * g)
{ // a = const
    stringstream data, mips;
    string address = (g->address_table.find(result))->second;

    if (result->type == Symtable::TINT)
    {
        data << "const_" << g->data_counter++ << ":\n" <<
                "\t" << ".word " << result->ival << "\n";

        mips << "LW $3, constr_" << g->data_counter << "\n" <<
                "SW $3, " << address << "\n";
    }
    else if (result->type == Symtable::TCHAR)
    {
        data << "const_" << g->data_counter++ << ":\n" <<
                "\t" << ".byte " << result->sval << "\n";

        mips << "LB $3, constr_" << g->data_counter << "\n" <<
                "SB $3, " << address << "\n";
    }
    else if (result->type == Symtable::TSTRING)
    {
        string sval = result->sval;
        sval.erase(0, 1);
        sval.erase(sval.size() - 1);

        data << "const_" << g->data_counter++ << ":\n" <<
                "\t" << ".asciz " << "\"" << sval << "\\0\"\n";

        mips << "LA $3, constr_" << g->data_counter << "\n" << // Get string address
                "SW $3, " << address << "\n";
    }

    g->data += data.str();
    return mips.str();
}

//////////////////////////////////// AssignmentInst

string AssignmentInst::str()
{
    std::stringstream stream;
    stream << result->id << " = " << var->id;
    return stream.str();
}

string AssignmentInst::generate(Generator * generator)
{ // a = var
    string mips = "ASSIGN\n";
    return mips;
}

//////////////////////////////////// JumpInst

string JumpInst::str()
{
    std::stringstream stream;
    stream << "jump to " << label->id;
    return stream.str();
}

string JumpInst::generate(Generator * generator)
{ // goto a
    return "J " + label->id + "\n";
}

//////////////////////////////////// JumpFalseInst

string JumpFalseInst::str()
{
    std::stringstream stream;
    stream << "if not " << cond->id << " jump to " << label->id;
    return stream.str();
}

string JumpFalseInst::generate(Generator * generator)
{ // if not a then goto b
    string mips = "JFALSE " + label->id + "\n";
    return mips;
}

//////////////////////////////////// CallInst

string CallInst::str()
{
    std::stringstream stream;

    if (result)
    {
        stream << result->id << " = ";
    }

    stream << fce->id << "(";

    bool first = true;
    for (list<Variable*>::iterator i = args.begin(); i != args.end(); ++i)
    {

        if (!first)
        {
            stream << ", ";
        }

        stream << (*i)->id;
        first = false;
    }

    stream << ")";
    return stream.str();
}

string CallInst::generate(Generator * g)
{ // a = func(b,c)
    stringstream mips;
    // Jump s tím, že musím uložit na zásobník SP a FP a takové ty srandy.
    mips << "JAL " << fce->id << "\n";
    mips << g->stack.push(4); // Allocate 4B for $ra
    mips << "SW $ra, ($sp)\n"; // Store $ra to stack

    return mips.str();
}

CallInst::~CallInst()
{
    args.clear();
}

//////////////////////////////////// ReturnInst

string ReturnInst::str()
{
    std::stringstream stream;
    stream << "return";

    if (result != NULL)
    {
        stream << " " << result->id;
    }

    return stream.str();
}

string ReturnInst::generate(Generator * generator)
{ // return a
    string mips = "JR $ra\n";
    return mips;
}

/* end of file */
