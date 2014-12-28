%require "2.5"
%language "C++"
%skeleton "lalr1.cc"
%defines
%debug

%define parser_class_name "parser"
%define parse.error verbose

%code requires{
  # include "driver.h"
  # include "symtable.h"
}

%param { Driver& driver }

%{
# include <iostream>
# include <cstdio>
# include "driver.h"
# include "symtable.h"
%}

%union {
  int             ival;
  std::string    *sval;
  Variable       *vval;
  Symtable::Type  tval;
};

/* TOKENS */
%token <ival> NUM
%token <sval> CHAR
%token <sval> STRING
%token <sval> ID

%token ';' '{' '}' '='
%token VOID TINT TCHAR TSTR
%token IF ELSE WHILE RETURN

%left OR
%left AND
%left EQ NEQ
%left LT LTE GT GTE
%left ADD SUB
%left MUL DIV MOD
%nonassoc NEG
%nonassoc '(' ')'

%type <tval> type datatype VOID TINT TCHAR TSTR
%type <vval> expr

%start program

%{
  extern int yylex(yy::parser::semantic_type *yylval,
                   yy::parser::location_type *yylloc,
                   Driver& driver);

  extern FILE *yyin;
%}

%locations
%initial-action {
 @$.begin.filename = @$.end.filename = driver.filename; /* set proper filename!!! */
}
%%

program: functions;

functions:
  | fce_declaration functions
  | fce_definition functions
  ;

fce_declaration: 
    type ID '(' VOID ')' ';'              {driver.addFunction($2, $1);}
  | type ID '(' datatype_list ')' ';'     {driver.addFunction($2, $1);}
  ;

fce_definition:  
    type ID '(' VOID ')'       
    '{'                                   {driver.enterFunc($2, $1);} 
      stmt_list                           {driver.leaveBlock();} 
    '}'                                   { /* add stmts to fce */ } 
    
  | type ID '(' param_list ')' 
    '{'                                   {driver.enterFunc($2, $1);} 
      stmt_list                           {driver.leaveBlock();} 
    '}'                                   { /* add stmts to fce */ } 
  ;

stmt_list:
  | '{'                                   {driver.enterBlock();} 
      stmt_list                           {driver.leaveBlock();}
    '}'                                   { /* return $3 */ } 
     
  | stmt stmt_list                        { /* add $1 to $2 */ } 
  ;

stmt:
    datatype id_list ';'                  {driver.addVariables($1);}  
  | ID '=' expr ';'                       {driver.genAssignment($1, $3);}    
  | ID '(' argument_list ')' ';'          {driver.genCall($1);}
  
  | IF '(' expr ')' 
    '{'                                   {driver.enterBlock();} 
      stmt_list                           {driver.leaveBlock();} 
    '}' 
    ELSE 
    '{'                                   {driver.enterBlock();} 
      stmt_list                           {driver.leaveBlock();} 
    '}'                                   { /* generate condition */ } 
    
  | WHILE '(' expr ')' 
    '{'                                   {driver.enterBlock();} 
      stmt_list                           {driver.leaveBlock();} 
    '}'                                   { /* generate whileloop */ } 
    
  | RETURN expr ';'                       {driver.genReturn($2);}
  ;

expr:
    NUM                                   {$$ = driver.genExprInt($1);}
  | CHAR                                  {$$ = driver.genExprChar($1);}
  | STRING                                {$$ = driver.genExprStr($1);} 
  | ID                                    {$$ = driver.genExprVar($1);}
  | ID '(' argument_list ')'              {$$ = driver.genExprFce($1);}
  | expr ADD expr                         {$$ = driver.genExprOp($1, $3, Symtable::ADD);}
  | expr SUB expr                         {$$ = driver.genExprOp($1, $3, Symtable::SUB);}
  | expr MUL expr                         {$$ = driver.genExprOp($1, $3, Symtable::MUL);}
  | expr DIV expr                         {$$ = driver.genExprOp($1, $3, Symtable::DIV);}
  | expr MOD expr                         {$$ = driver.genExprOp($1, $3, Symtable::MOD);}
  | expr LT expr                          {$$ = driver.genExprOp($1, $3, Symtable::LT);}
  | expr LTE expr                         {$$ = driver.genExprOp($1, $3, Symtable::LTE);}
  | expr GT expr                          {$$ = driver.genExprOp($1, $3, Symtable::GT);}
  | expr GTE expr                         {$$ = driver.genExprOp($1, $3, Symtable::GTE);}
  | expr EQ expr                          {$$ = driver.genExprOp($1, $3, Symtable::EQ);}
  | expr NEQ expr                         {$$ = driver.genExprOp($1, $3, Symtable::NEQ);}
  | expr AND expr                         {$$ = driver.genExprOp($1, $3, Symtable::AND);}
  | expr OR expr                          {$$ = driver.genExprOp($1, $3, Symtable::OR);}
  | NEG expr                              {$$ = driver.genExprOp($2, NULL, Symtable::NEG);}
  | '(' expr ')'                          {$$ = $2;}
  | '(' datatype ')' expr                 {$$ = driver.genExprCast($4, $2);}
  ;

type: 
    VOID                                  { $$ = Symtable::VOID; }
  | datatype                              { $$ = $1; }
  ;
  
datatype: 
    TINT                                  { $$ = Symtable::TINT; }
  | TCHAR                                 { $$ = Symtable::TCHAR; }
  | TSTR                                  { $$ = Symtable::TSTRING; }
  ;

datatype_list: 
    datatype                              { driver.addType($1); }
  | datatype ',' datatype_list            { driver.addType($1); }
  ;
  
param_list: 
    datatype ID                           { driver.addParam($2, $1); }
  | datatype ID ',' param_list            { driver.addParam($2, $1); }
  ;
  
argument_list: | expr_list ;

id_list: 
    ID                                    { driver.addId($1); }
  | ID ',' id_list                        { driver.addId($1); }
  ;
  
expr_list: 
    expr                                  { driver.addExpression($1); }
  | expr ',' expr_list                    { driver.addExpression($1); }
  ;

%%

namespace yy {
  void parser::error(location const &loc, const std::string& s)
  {
    std::cerr << "error at " << loc << ": " << s << std::endl;
  }
}

