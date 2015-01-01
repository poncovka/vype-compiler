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

    if (f->id != "main")
    {
        stream << "FUNC_";
    }

    stream << f->id;

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
    ss << endl << id << ":" << endl;
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
    stringstream mips;

    rd = (g->address_table.find(result))->second;
    rs = (g->address_table.find(var1))->second;
    rt = (g->address_table.find(var2))->second;

    if (op == Symtable::ADD)
    { // rd ← rs + rt ~ ADD rd, rs, rt
        mips << "lw $4, " << rs << " // ADD" << endl <<
                "lw $5, " << rt << endl <<
                "add $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == Symtable::SUB)
    { // rd ← rs - rt ~ SUB rd, rs, rt
        mips << "lw $4, " << rs << " // SUB" << endl <<
                "lw $5, " << rt << endl <<
                "sub $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == Symtable::MUL)
    { // rd ← rs × rt ~ MUL rd, rs, rt
        mips << "lw $4, " << rs << " // MUL" << endl <<
                "lw $5, " << rt << endl <<
                "mul $3, $4, $5" << endl <<
                "sw $3, " << rd << endl;

    }
    else if (op == Symtable::DIV || op == Symtable::MOD)
    { // (HI, LO) ← rs / rt ~ DIV rs, rt
        mips << "lw $4, " << rs << " // DIV" << endl <<
                "lw $5, " << rt << endl <<
                "div $4, $5" << endl;

        if (op == Symtable::DIV)
        {
            mips << "mflo $3" << endl <<
                    "sw $3, " << rd << endl;
        }
        else if (op == Symtable::MOD)
        {
            mips << "mfhi $3" << endl <<
                    "sw $3, " << rd << endl;
        }

    }
    else if (op == Symtable::LT)
    {
        mips << "LT" << endl;

    }
    else if (op == Symtable::LTE)
    {
        mips << "LTE" << endl;

    }
    else if (op == Symtable::GT)
    {
        mips << "GT" << endl;

    }
    else if (op == Symtable::GTE)
    {
        mips << "GTE" << endl;

    }
    else if (op == Symtable::EQ)
    {
        mips << "EQ" << endl;

    }
    else if (op == Symtable::NEQ)
    {
        mips << "NEQ" << endl;

    }
    else if (op == Symtable::AND)
    {
        mips << "AND" << endl;

    }
    else if (op == Symtable::OR)
    {
        mips << "OR" << endl;

    }
    else if (op == Symtable::NEG)
    {
        mips << "NEG" << endl;
    }

    return mips.str();
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
    stringstream mips;
    mips << "CAST" << endl;
    return mips.str();
}

//////////////////////////////////// LoadInst

string LoadInst::str()
{
    stringstream stream;
    stream << result->id << " = " << Symtable::str(result->type);
    return stream.str();
}

string LoadInst::generate(Generator * g)
{ // a = const
    stringstream data, mips;
    string address = (g->address_table.find(result))->second;

    if (result->type == Symtable::TINT)
    {
        data << "const_" << g->data_counter << ":" <<
                "\t" << ".word " << result->ival << endl;

        mips << "la $7, const_" << g->data_counter << " // Load int" << endl <<
                "lw $3, ($7)" << endl <<
                "sw $3, " << address << endl;
    }
    else if (result->type == Symtable::TCHAR)
    {
        data << "const_" << g->data_counter << ":\t" <<
                ".byte " << result->sval << endl;

        mips << "la $3, const_" << g->data_counter << " // Load char" << endl <<
                "lb $3, ($7)" << endl <<
                "sb $3, " << address << endl;
    }
    else if (result->type == Symtable::TSTRING)
    {
        string sval = result->sval;
        sval.erase(0, 1);
        sval.erase(sval.size() - 1);

        data << "const_" << g->data_counter << ":\t" <<
                ".asciz " << "\"" << sval << "\\0\"" << endl;

        mips << "la $3, const_" << g->data_counter << " // Load string" << endl << // Get string address
                "sw $3, " << address << endl;
    }

    g->data_counter++;

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

string AssignmentInst::generate(Generator * g)
{ // a = var
    stringstream mips;
    string res, var1;
    var1 = (g->address_table.find(var))->second;
    res = (g->address_table.find(var))->second;

    if (result->type == Symtable::TINT || result->type == Symtable::TSTRING)
    {
        mips << "lw $3, " << var1 << " // Assign " << Symtable::str(g->address_table.find(var)->first->type) << endl <<
                "sw $3, " << res << endl;
    }
    else if (result->type == Symtable::TCHAR)
    {
        mips << "lb $3, " << var1 << " // Assign char" << endl <<
                "sb $3, " << res << endl;
    }

    return mips.str();
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
    stringstream mips;
    mips << "j " << label->id << " // Jump" << endl;
    return mips.str();
}

//////////////////////////////////// JumpFalseInst

string JumpFalseInst::str()
{
    std::stringstream stream;
    stream << "if not " << cond->id << " jump to " << label->id;
    return stream.str();
}

string JumpFalseInst::generate(Generator * g)
{ // if not a then goto b
    stringstream mips;
    mips << "JFALSE " << label->id << endl;
    return mips.str();
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

    if (fce->id == "print")
    {
        for (list<Variable*>::iterator i = args.begin(); i != args.end(); ++i)
        {
            Variable* var = *i;
            string address = g->address_table.find(var)->second;

            if (var->type == Symtable::TINT)
            {
                mips << "lw $9, " << address << "" << endl <<
                        "print_int $9" << endl;
            }
            else if (var->type == Symtable::TCHAR)
            {
                mips << "lb $9, " << address << "" << endl <<
                        "print_char $9" << endl;
            }
            else if (var->type == Symtable::TSTRING)
            {
                mips << "lw $9, " << address << "" << endl <<
                        "print_string $9" << endl;
            }
        }
    }
    else if (fce->id == "main")
    {
        mips << g->stack.push(4); // Allocate 4B for $ra
        mips << "sw $ra, ($fp)" << endl; // Store $ra to stack
    }
    else
    {
        // Jump s tím, že musím uložit na zásobník SP a FP a takové ty srandy.
        mips << "jal " << fce->id << " // Call" << endl;
        mips << g->stack.push(4); // Allocate 4B for $ra
        mips << "sw $ra, ($sp)" << endl; // Store $ra to stack
    }

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

string ReturnInst::generate(Generator * g)
{ // return a
    stringstream mips;
    mips << "jr $ra // Return" << endl;
    return mips.str();
}

/* end of file */
