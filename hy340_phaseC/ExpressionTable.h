#ifndef EXPRTABLE_H
#define EXPRTABLE_H


#define EXPAND_SIZE     1024
#define CURR_SIZE       (total*sizeof(struct quad))
#define NEW_SIZE        (EXPAND_SIZE*sizeof(struct quad)+CURR_SIZE)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"
#include "list.h"
#include "vlist.h"


extern FILE *fp;




/* Available opcode set */
typedef enum iopcode {
        assign,                 add,                    sub,
        mul,                    divv,                   mod,
        uminus,                 and,                    or,
        not,                    if_eq,                  if_noteq,
        if_lessereq,             if_greatereq,           if_less,
        if_greater,             call,                   param,
        ret,                    getretval,              funcstart,
        funcend,                tablecreate,		    jump,
        tablegetelem,   	    tablesetelem
}iopcode_t;

/* Types of expressions */
typedef enum e{
        	var_e, 
		tableitem_e,
		
		programfunc_e, 
		libraryfunc_e,
		
		arithexpr_e,
		boolexpr_e,
        	assignexpr_e,
		newtable_e,
		
		constnum_e,
		constbool_e, 
		conststring_e,
		
		nil_e
}expr_t;

/* Expression structure */
typedef struct expressions {
	
	expr_t type;
	SymbolTableEntry_t* sym;
	struct expressions*    index;
	
	double numConst;
	char*  strConst;
	unsigned char boolConst;
	
	struct expressions* next;
	
	/*Logic for number checking*/
	unsigned int isNumber;
	unsigned int isBoolean;
	unsigned int isString;

	/* Maybe ? */
	unsigned int isConstant;
	unsigned int hasValue;
	char *name;
	/* truelist falselist*/
	boolList *truelist;
	boolList *falselist;
	
	//looplist *continuelist;
	//looplist *breaklist;
	
}expr;


typedef struct forStruct{
	int enter;
	int test;
}enter_test;


typedef struct idxelement{
   expr *arg1;
   expr *arg2;
}indexedElement;


/* Quad structure */
typedef struct quad {
        iopcode_t    	op;
        expr*    result;
        expr*    arg1;
        expr*    arg2;
        unsigned        label;
        unsigned        line;
}quad;


/* Quad Number structure */
typedef struct nodes{
	int quad_num;
	struct nodes *next;
} quad_node ;

/* Returns the str representation of opcode*/
char* getOpCode(iopcode_t op);

/* creates an expression*/
expr* createExpression(expr_t type, SymbolTableEntry_t* SymEntry, int numVal, char* strVal, unsigned char boolVal);

/* Expands the size of array og quads*/
void expand (void);
/* Make new entry in quads array */
void emit(iopcode_t op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line);
/* Creates an expr pointing at sym */
expr* lvalue_expr (SymbolTableEntry_t* sym);
/* Creates new expression with type t*/
expr* newexpr (expr_t t);
/* Creates new expression with strConst*/
expr* newexpr_conststring (char* s);
/* Creates new expression with numConst*/
expr* newexpr_constnum(int num);
/* Creates new expression with boolConst*/
expr* newexpr_constbool(unsigned char booleanVal);

/* Check if a number is integer */
int IsInteger(double n);

/* Backpatch truelist and falselist */
void backpatch(boolList* list, int label);
/* Patch a quad */
void patchlabel(unsigned quadNo, unsigned label);
/* Print quads array */
void printQuadsArray(void);





/* Resets Counter for hidden variable creation */
int resetTemp();

/* Get current quad */
unsigned nextquadlabel(void);

/* Type check for unary minus */
void checkminus (expr * expression);

/* Type check for addition */
void checkAddition (expr * expression);

/* Type check for substraction */
void checkSubtraction (expr * expression);

/* Type check for multiplication */
void checkMultiplication (expr * expression);


void checkDivision(expr * expression);
void checkModular(expr * expression);

char *nameTempFunction(int counter);
char *nameTempVariable(int counter);

expr* emit_iftableitem (expr* e); 

expr* member_item(expr* lvalue, char* name);


/********************* Managers ***********************/

void Manage_break(int whileLoop);
void Manage_continue(int whileLoop);
void Manage_return(int intoFunction);
void Manage_return_expr_semicolon(int intoFunction, expr * expr);
char * Manage_funcname(char * name);
#endif
