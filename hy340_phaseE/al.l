%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

#define MAX_STR_CONST 6000

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;
int counter=0;
int comment_nesting = 0;
int string_line_no = 0;
int comment_line_no = 0;

%}

%option yylineno
%option noyywrap
%option case-sensitive

letter			[a-zA-Z]
digit 			[0-9]

integer     	{digit}+
float       	{digit}*\.{digit}+


escapeseq		(\\(['"\?\\abfnrtv]|[0-7]{1,3}))

tab 			"\t"
newline			"\n"
space			" "
ret				"\r"
verttab			"\v"

assignment		"="
plus			"\+"
minus			"-"
mul				"\*"
div				"\/"
mod				"%"
equal			"=="
not_equal		"!="
plus_plus		"++"
minus_minus		"--"
greater			">"
lesser			"<"
greater_equal	">="
lesser_equal	"<="

if				"if"
else			"else"
while			"while"
for				"for"
function		"function"
return			"return"
break			"break"
continue		"continue"
and				"and"
not				"not"
or				"or"
local			"local"
true			"true"
false			"false"
nil				"nil"

leftbr			"\{"
rightbr			"\}"
leftsqbr		"\["
rightsqbr		"\]"
leftpar			"\("
rightpar		"\)"
semicolon		";"
comma			","
col				":"
ccol			"::"
dot				"\."
doubledot		".."

comment1		"//".*

identifier      [a-zA-Z][a-zA-Z_0-9]*

%x COMMENT STRING

%%
"/*"             	BEGIN(COMMENT); ++comment_nesting; comment_line_no = yylineno; 
<COMMENT>[^*/]* 	/* Eat non-comment delimiters */
<COMMENT>"/*"   	++comment_nesting;
<COMMENT>"*/"   	if (--comment_nesting == 0) { 
						BEGIN(INITIAL);
						return (COMMENT2);
					}else{
						return(COMMENT3);
					}
<COMMENT>[*/]   	/* Eat a / or * if it doesn't match comment sequence */
<COMMENT><<EOF>>	{fprintf(stderr, "Error: <line: %u> Unfinished comment.\n", comment_line_no);yyterminate();}



\"				{string_line_no = yylineno; string_buf_ptr = string_buf; *string_buf_ptr++ = '\"'; BEGIN(STRING);}
<STRING>\"		{*string_buf_ptr++ = '\"'; *string_buf_ptr = '\0'; BEGIN(INITIAL); yylval.strValue = strdup(string_buf);  return STRING_DEF;   }
<STRING><<EOF>>	{*string_buf_ptr = '\0'; fprintf(stderr, "Error: <line: %u> Unfinished string.\n", string_line_no); yyterminate();}
<STRING>{
             "\\n"   *string_buf_ptr++ = '\n';
             "\\r"   *string_buf_ptr++ = '\r';
             "\\f"   *string_buf_ptr++ = '\f';
             "\\0"   *string_buf_ptr++ = '\0';
			 "\\t"	 *string_buf_ptr++ = '\t';
			 "\\b"	 *string_buf_ptr++ = '\b';
			 "\\v"	 *string_buf_ptr++ = '\v';
			 "\\\\"  *string_buf_ptr++ = '\\';
			 "\\\""  *string_buf_ptr++ = '\"';
			 "\\'"   *string_buf_ptr++ = '\'';
			 "\\?"   *string_buf_ptr++ = '\?';
			 "\\["   *string_buf_ptr++ = '\[';
			 
}
<STRING>\\(.|\n)  fprintf(stderr, "Warning: <line: %u> Cannot match  escape sequence character\n", string_line_no );*string_buf_ptr++ = yytext[0];*string_buf_ptr++ = yytext[1];
<STRING>[^\\."\']+        {
                 char *yptr = yytext;
     
                 while ( *yptr )
                         *string_buf_ptr++ = *yptr++;
                 }



{escapeseq}			{}	
{space}				{}
{tab} 				{}
{newline}			{}
{ret}				{}
{verttab}			{}

{assignment}    {return '=';}
{plus}         	{return '+';}
{minus}			{return '-';}
{mul}          	{return '*';}
{div}	        {return '/';}
{mod}			{return '%';}
{equal}			{return EQUAL;}
{not_equal}     {return NOTEQUAL;}
{plus_plus}     {return PLUS_PLUS;}
{minus_minus}   {return MINUS_MINUS;}
{greater}       {return '>';}
{lesser}        {return '<';}
{lesser_equal}  {return LESSER_EQ;}
{greater_equal} {return GREATER_EQ;}

{leftbr}		{return '{';}
{rightbr}		{return '}';}
{leftsqbr}		{return '[';}
{rightsqbr} 	{return ']';}
{leftpar}		{return '(';}
{rightpar}		{return ')';}
{semicolon}		{return ';';}
{comma}			{return ',';}
{col}			{return ':';}
{ccol}			{return DOUBLE_COLON;}
{dot}			{return '.';}
{doubledot}		{return DOUBLE_DOT;}

{if}			{yylval.strValue= strdup (yytext);return(IF);}
{else}			{yylval.strValue= strdup (yytext);return(ELSE);}
{while}			{yylval.strValue= strdup (yytext);return(WHILE);}
{for}			{yylval.strValue= strdup (yytext);return(FOR);}
{function}		{yylval.strValue= strdup (yytext);return (FUNCTION);}
{return}		{yylval.strValue= strdup (yytext);return(RETURN);}
{break}			{yylval.strValue= strdup (yytext);return(BREAK);}
{continue}		{yylval.strValue= strdup (yytext);return(CONTINUE);}
{and}			{yylval.strValue= strdup (yytext);return(AND);}
{not}			{yylval.strValue= strdup (yytext);return(NOT);}
{or}			{yylval.strValue= strdup (yytext);return(OR);}
{local}			{yylval.strValue= strdup (yytext);return(LOCAL);}
{true} 			{yylval.strValue= strdup (yytext);return(TRUE);}
{false}			{yylval.strValue= strdup (yytext);return(FALSE);}
{nil}			{yylval.strValue= strdup (yytext);return(NIL);}

{integer}       {yylval.intValue = atoi(yytext);return(INTEGER);}
{float}         {yylval.floatValue = atof(yytext);return(FLOAT);}

{comment1}		{return (COMMENT1);}

{identifier}    {yylval.strValue = strdup(yytext);return (IDENTIFIER);}

.				{fprintf(stderr, "Error: <line: %u> Cannot match character %s with any rule\n", yylineno, yytext); }


