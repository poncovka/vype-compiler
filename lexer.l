%option noyywrap batch debug nounput

%{

# include <cstdlib>
# include "error.h"
# include "driver.h"
# include "parser.tab.hh"

# define YY_USER_ACTION  yylloc->columns(yyleng);
# define YY_DECL int yylex(yy::parser::semantic_type *yylval,\
                           yy::parser::location_type *yylloc,\
                           Driver& driver)

int oldstate;
typedef yy::parser::token token;

%}
%x COMMENT

CHAR   [\x20-\x21\x23-\x26\x28-\x5b\x5d-\xff] 
ESC    (\\\\|\\n|\\t|\\\"|\\')

%%

%{
  // start where previous token ended
  yylloc->step();
%}

";" |  
"," |  
"{" |
"}" |
"(" |
")" |
"="                     { return yytext[0]; }

"!"                     { return token::NEG; }
"&&"                    { return token::AND; }
"||"                    { return token::OR; }

"=="                    { return token::EQ; }
"!="                    { return token::NEQ; }

"<"                     { return token::LT; }
"<="                    { return token::LTE; }
">"                     { return token::GT; }
">="                    { return token::GTE; }

"+"                     { return token::ADD; }
"-"                     { return token::SUB; }
"*"                     { return token::MUL; }
"/"                     { return token::DIV; }
"%"                     { return token::MOD; }

"void"                  { return token::VOID; }
"int"                   { return token::TINT; }
"char"                  { return token::TCHAR; }
"string"                { return token::TSTR; }

"if"                    { return token::IF; }
"else"                  { return token::ELSE; }
"while"                 { return token::WHILE; }
"return"                { return token::RETURN; }

"break"    |
"continue" |
"for"      |
"short"    |
"unsigned"              { return token::KEYWORD; }

[0-9]+	                { yylval->ival = atoi(yytext); return token::NUM; }
'({CHAR}|{ESC}|\"|\\)'  { yylval->sval = new std::string(yytext); return token::CHAR; }
\"({CHAR}|{ESC})*\"     { yylval->sval = new std::string(yytext); return token::STRING; }
[a-zA-Z_][a-zA-Z0-9_]*	{ yylval->sval = new std::string(yytext); return token::ID; }

"//".*                  ;
"/*"                    { oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN oldstate; } 
<COMMENT>.|[\n]         ;

[ \t\r\n]+              ;

<COMMENT><<EOF>>        { driver.errorLex(*yylloc, "unterminated comment"); yyterminate();}
.                       { driver.errorLex(*yylloc, string("invalid character ") + string(yytext, yyleng)); }

%%
