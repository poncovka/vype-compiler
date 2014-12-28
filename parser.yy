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
  int ival;
  std::string *sval;
  Type tval;
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
    type ID '(' VOID ')' ';'                 {driver.addFunction(*$2, $1);}
  | type ID '(' datatype_list ')' ';'        {driver.addFunction(*$2, $1);}
  ;

fce_definition:  
    type ID '(' VOID ')'       '{' {driver.enterFunc(*$2, $1);} stmt_list {driver.leaveBlock();} '}' 
  | type ID '(' param_list ')' '{' {driver.enterFunc(*$2, $1);} stmt_list {driver.leaveBlock();} '}' 
  ;

stmt_list:
  | '{' {driver.enterBlock();} stmt_list {driver.leaveBlock();} '}'
  | stmt stmt_list
  ;

stmt:
    datatype id_list ';'
  | ID '=' expr ';'
  | ID '(' argument_list ')' ';'
  
  | IF '(' expr ')' 
    '{' {driver.enterBlock();} 
      stmt_list {driver.leaveBlock();} 
    '}' 
    ELSE 
    '{' {driver.enterBlock();} 
      stmt_list {driver.leaveBlock();} 
    '}'
    
  | WHILE '(' expr ')' 
    '{' {driver.enterBlock();} 
      stmt_list {driver.leaveBlock();} 
    '}'
    
  | RETURN expr ';'
  ;

expr:
    NUM
  | CHAR
  | STRING
  | ID 
  | ID '(' argument_list ')'
  | expr ADD expr
  | expr SUB expr
  | expr MUL expr
  | expr DIV expr
  | expr MOD expr
  | expr LT expr
  | expr LTE expr
  | expr GT expr
  | expr GTE expr
  | expr EQ expr
  | expr NEQ expr
  | expr AND expr
  | expr OR expr
  | NEG expr
  | '(' expr ')'
  | '(' datatype ')' expr
  ;

type: 
    VOID        { $$ = VOID; }
  | datatype    { $$ = $1; }
  ;
  
datatype: 
    TINT        { $$ = TINT; }
  | TCHAR       { $$ = TCHAR; }
  | TSTR        { $$ = TSTRING; }
  ;

datatype_list: datatype | datatype ',' datatype_list ;
param_list: datatype ID | datatype ID ',' param_list ;
argument_list: | expr_list ;

id_list: ID | ID ',' id_list ;
expr_list: expr | expr ',' expr_list ;

%%

namespace yy {
  void parser::error(location const &loc, const std::string& s)
  {
    std::cerr << "error at " << loc << ": " << s << std::endl;
  }
}

