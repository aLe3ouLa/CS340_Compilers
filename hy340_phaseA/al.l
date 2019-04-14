%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define YY_DECL int yylex(void * yylval)
#define MAX_STR_CONST 6000

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;
int counter=0;
int comment_nesting = 0;
int cc = 0;
int string_line_no = 0;
int comment_line_no = 0;
FILE * mode;

typedef struct node {
  unsigned int lineNumber;
  unsigned int serialNumber;
  char *yytextValue;
  char *tokenCategory;
  char *macroType;
  int macroNumber;
  struct node *next; 
} alpha_token_t;

alpha_token_t *yylval = NULL;  

char * str[] = {"KEYWORD", "OPERATOR" , "INTCONST", "FLOATCONST", "STRING", "PUNCTUATION_MARK", "IDENTIFIER", "COMMENTS"};

typedef enum {IF = 0, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, AND, NOT, OR, LOCAL, TRUE, FALSE, NIL} keyword;

typedef enum {ASSIGNMENT = 15, PLUS, MINUS, MUL, DIV, MOD, EQUAL, NOTEQUAL, PLUS_PLUS, MINUS_MINUS, LESSER, GREATER, LESSER_EQ, GREATER_EQ } operators;
			   
typedef enum {INTEGER = 29} integer;

typedef enum {FLOAT = 30} floating_point;

typedef enum {STRING_RIGHT = 31} string;

typedef enum {CURLY_BR_LEFT = 32, CURLY_BR_RIGHT, SQUARE_BR_LEFT, SQUARE_BR_RIGHT, PARENTHESIS_LEFT, PARENTHESIS_RIGHT, SEMICOLON, COMMA, COLON, DOUBLE_COLON, DOT, DOUBLE_DOT} delim;

typedef enum {IDENTIFIER = 44}ids;

typedef enum {COMMENT1 = 45, COMMENT2, COMMENT3}comments;



char * keywStr[] = {"IF", "ELSE", "WHILE", "FOR", "FUNCTION", "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "TRUE", "FALSE", "NIL",
					"ASSIGNMENT","PLUS", "MINUS", "MUL", "DIV", "MOD" ,"EQUAL", "NOT EQUAL", "PLUS_PLUS", "MINUS_MINUS", "LESSER", "GREATER", "LESSER_EQ", "GREATER_EQ" , 
					"int", "float", "str" ,"CURLY_BR_LEFT", "CURLY_BR_RIGHT", "SQUARE_BR_LEFT", "SQUARE_BR_RIGHT", "PARENTHESIS_LEFT", "PARENTHESIS_RIGHT",
                    "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON", "DOT", "DOUBLE_DOT", "id", "SINGLE LINE COMMENT", "MULTI LINE COMMENT", "NESTED COMMENT" };

void insert_in_list(unsigned int lineNumber, unsigned int serialNumber, char * yytextValue, char *tokenCategory, int macroType){
    alpha_token_t *ListRoot = yylval, *newNode = NULL;
	
    newNode = malloc(sizeof(alpha_token_t));
    if(newNode == NULL){
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(0);
    }
    newNode->lineNumber = lineNumber;
    newNode->serialNumber = serialNumber;
    
    newNode->yytextValue = malloc (sizeof(char) * strlen(yytextValue));
	if(newNode->yytextValue == NULL){
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(0);
    }
    strcpy(newNode->yytextValue, yytextValue);
	
    newNode->tokenCategory = malloc (sizeof(char)* strlen(tokenCategory) );
    if(newNode->tokenCategory == NULL){
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(0);
    }
	strcpy(newNode->tokenCategory, tokenCategory);
    
    newNode->macroNumber = macroType;
	
     if (macroType == 29 || macroType == 30 || macroType == 44 || macroType == 31){
		
				newNode->macroType = malloc (sizeof(char) * strlen(yytextValue));
				if(newNode->macroType == NULL){
					fprintf(stderr, "Failed to allocate memory.\n");
					exit(0);
				}
				strcpy(newNode->macroType, yytextValue);
        }else{
			newNode->macroType = malloc (sizeof(char) * strlen(keywStr[macroType]));
			if(newNode->macroType == NULL){
				fprintf(stderr, "Failed to allocate memory.\n");
				exit(0);
			}
            strcpy(newNode->macroType, keywStr[macroType]);
        }
		
	
    newNode->next = NULL;
    
    if (ListRoot == NULL){
        yylval = newNode;
        return;
    }
    else{
        while ( ListRoot->next != 0){
            ListRoot = ListRoot->next;
        }
    }
    
   ListRoot->next = newNode;
  
    
}

void printList(){
    alpha_token_t * temp = yylval;
	
    while (temp != NULL){
	
		if (temp->macroNumber == 29){ /* INTEGER*/
			int intRepr = atoi(temp->macroType);
			fprintf(mode, "%u : #%u\t\"%s\"\t%s  %d\n", temp->lineNumber, temp->serialNumber, temp->yytextValue, temp->tokenCategory, intRepr); 
		}else if(temp->macroNumber == 30){ /* FLOAT*/
			float floatRepr = atof(temp->macroType);
			fprintf(mode, "%u : #%u\t\"%s\"\t%s  %f\n", temp->lineNumber, temp->serialNumber, temp->yytextValue, temp->tokenCategory, floatRepr); 
		}else if(temp->macroNumber == 44){ /* IDENTIFIER */
			char * strIdent = temp->macroType;
			fprintf(mode, "%u : #%u\t\"%s\"\t%s  \"%s\" \n", temp->lineNumber, temp->serialNumber, temp->yytextValue, temp->tokenCategory, strIdent);
		}else if (temp->macroNumber == 45 | temp->macroNumber == 46 | temp->macroNumber == 47){ /* COMMENTS */
			fprintf(mode, "%u : #%u\t%s  %s\n", temp->lineNumber, temp->serialNumber, temp->tokenCategory, temp->macroType); 
		}else{ /* ANYTHING ELSE */
			fprintf(mode, "%u : #%u\t\"%s\"\t%s  %s\n", temp->lineNumber, temp->serialNumber, temp->yytextValue, temp->tokenCategory, temp->macroType); 
		}
		
        temp = temp->next;
    }
}


void deleteList (){

	alpha_token_t * temp = NULL;
	
	while(yylval != NULL){
		temp = yylval->next;
		free(yylval->yytextValue);
		free(yylval->tokenCategory);
		free(yylval->macroType);
		free (yylval);
		yylval = temp;
	}
	yylval = NULL;
}



%}

%option yylineno
%option noyywrap
%option case-sensitive
%option prefix = "alpha_yy"


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


"/*"             	cc = 0;BEGIN(COMMENT); ++comment_nesting; comment_line_no = yylineno; ++cc;
<COMMENT>[^*/]* 	/* Eat non-comment delimiters */
<COMMENT>"/*"   	++comment_nesting; ++cc;
<COMMENT>"*/"   	if (--comment_nesting == 0) { 
						counter++; 
						insert_in_list(comment_line_no, counter, " ", str[7], COMMENT2);
						BEGIN(INITIAL);
					}else{
						counter++; insert_in_list(comment_line_no, counter, " ", str[7], COMMENT3);
					}
<COMMENT>[*/]   	/* Eat a / or * if it doesn't match comment sequence */
<COMMENT><<EOF>>	{fprintf(mode, "Error: <line: %u> Unfinished comment.\n", comment_line_no);yyterminate();}

\"				{string_line_no = yylineno; string_buf_ptr = string_buf; *string_buf_ptr++ = '\"'; BEGIN(STRING);}
<STRING>\"		{*string_buf_ptr++ = '\"'; *string_buf_ptr = '\0'; counter++; insert_in_list(string_line_no, counter, string_buf, str[4], STRING_RIGHT);  BEGIN(INITIAL); }
<STRING><<EOF>>	{*string_buf_ptr = '\0'; fprintf(mode, "Error: <line: %u> Unfinished string.\n", string_line_no); yyterminate();}
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
<STRING>\\(.|\n)  fprintf(mode, "Warning: <line: %u> Cannot match  escape sequence character\n", string_line_no );*string_buf_ptr++ = yytext[0];*string_buf_ptr++ = yytext[1];
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

{assignment}    {counter++; insert_in_list(yylineno, counter, yytext, str[1], ASSIGNMENT);}
{plus}         	{counter++; insert_in_list(yylineno, counter, yytext, str[1], PLUS);}
{minus}			{counter++; insert_in_list(yylineno, counter, yytext, str[1], MINUS);}
{mul}          	{counter++; insert_in_list(yylineno, counter, yytext, str[1], MUL);}
{div}	        {counter++; insert_in_list(yylineno, counter, yytext, str[1], DIV);}
{mod}			{counter++; insert_in_list(yylineno, counter, yytext, str[1], MOD);}
{equal}			{counter++; insert_in_list(yylineno, counter, yytext, str[1], EQUAL);}
{not_equal}     {counter++; insert_in_list(yylineno, counter, yytext, str[1], NOTEQUAL);}
{plus_plus}     {counter++; insert_in_list(yylineno, counter, yytext, str[1], PLUS_PLUS);}
{minus_minus}   {counter++; insert_in_list(yylineno, counter, yytext, str[1], MINUS_MINUS);}
{greater}       {counter++; insert_in_list(yylineno, counter, yytext, str[1], GREATER);}
{lesser}        {counter++; insert_in_list(yylineno, counter, yytext, str[1], LESSER);}
{lesser_equal}  {counter++; insert_in_list(yylineno, counter, yytext, str[1], LESSER_EQ);}
{greater_equal} {counter++; insert_in_list(yylineno, counter, yytext, str[1], GREATER_EQ);}


{leftbr}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], CURLY_BR_LEFT);}
{rightbr}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], CURLY_BR_RIGHT);}
{leftsqbr}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], SQUARE_BR_LEFT);}
{rightsqbr} 	{counter++; insert_in_list(yylineno, counter, yytext, str[5], SQUARE_BR_RIGHT);}
{leftpar}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], PARENTHESIS_LEFT);}
{rightpar}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], PARENTHESIS_RIGHT);}
{semicolon}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], SEMICOLON);}
{comma}			{counter++; insert_in_list(yylineno, counter, yytext, str[5], COMMA);}
{col}			{counter++; insert_in_list(yylineno, counter, yytext, str[5], COLON);}
{ccol}			{counter++; insert_in_list(yylineno, counter, yytext, str[5], DOUBLE_COLON);}
{dot}			{counter++; insert_in_list(yylineno, counter, yytext, str[5], DOT);}
{doubledot}		{counter++; insert_in_list(yylineno, counter, yytext, str[5], DOUBLE_DOT);}

{if}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], IF);}
{else}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], ELSE);}
{while}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], WHILE);}
{for}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], FOR);}
{function}		{counter++; insert_in_list(yylineno, counter, yytext, str[0], FUNCTION);}
{return}		{counter++; insert_in_list(yylineno, counter, yytext, str[0], RETURN);}
{break}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], BREAK);}
{continue}		{counter++; insert_in_list(yylineno, counter, yytext, str[0], CONTINUE);}
{and}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], AND);}
{not}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], NOT);}
{or}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], OR);}
{local}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], LOCAL);}
{true} 			{counter++; insert_in_list(yylineno, counter, yytext, str[0], TRUE);}
{false}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], FALSE);}
{nil}			{counter++; insert_in_list(yylineno, counter, yytext, str[0], NIL);}

{integer}       {counter++; insert_in_list(yylineno, counter, yytext, str[2], INTEGER);}

{float}         {counter++; insert_in_list(yylineno, counter, yytext, str[3], FLOAT);}

{comment1}		{counter++; insert_in_list(yylineno, counter, " ", str[7], COMMENT1);}

{identifier}    {counter++; insert_in_list(yylineno, counter, yytext, str[6], IDENTIFIER);}

.				{fprintf(mode, "Error: <line: %u> Cannot match character %s with any rule\n", yylineno, yytext); }

%%

int main (int argc, char ** argv){
	FILE * ofp; 
	if (argc > 1){
		if (!(yyin = fopen(argv[1], "r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}

	}
	else	yyin = stdin;
	
	if (argv[2] != NULL){
		
		ofp = fopen(argv[2], "w");
		mode = ofp;
		if (ofp == NULL) {
			fprintf(stderr, "Can't open output file %s!\n",argv[2]);
			return 2;
		}
	}else{
		mode = stderr;
	}
	
	alpha_yylex(yylval);
	
	printList();
	deleteList();

	return 0;
}
