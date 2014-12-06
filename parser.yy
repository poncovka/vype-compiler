%require "2.5"
%language "C++"
%skeleton "lalr1.cc"
%defines
%debug

%define parser_class_name "parser"
%define parse.error verbose

%{
# include <iostream>
# include <cstdio>
using namespace std;
%}

%union {
  int ival;
  char cval;
  std::string *sval;
};

/* TOKENS */
%token <ival> NUM
%token <cval> CHAR
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

%start program

%{
  extern int yylex(yy::parser::semantic_type *yylval,
       yy::parser::location_type* yylloc);

  extern FILE *yyin;
%}

%locations
%initial-action {
 @$.begin.filename = @$.end.filename = new std::string("filename"); /* set proper filename!!! */
}
%%

program: functions;

functions:
  | fce_declaration functions
  | fce_definition functions
  ;

fce_declaration: 
    type ID '(' VOID ')' ';'  
  | type ID '(' datatype_list ')' ';' 
  ;

fce_definition:  
    type ID '(' VOID ')' '{' stmt_list '}'
  | type ID '(' param_list ')' '{' stmt_list '}' 
  ;

stmt_list:
  | '{' stmt_list '}'
  | stmt stmt_list
  ;

stmt:
    datatype id_list ';'
  | ID '=' expr ';'
  | ID '(' argument_list ')' ';'
  | IF '(' expr ')' '{' stmt_list '}' ELSE '{' stmt_list '}'
  | WHILE '(' expr ')' '{' stmt_list '}'
  | RETURN '(' expr ')' ';'
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

type: VOID | datatype ;
datatype: TINT | TCHAR | TSTR ;

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

