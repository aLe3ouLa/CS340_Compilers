#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SYMBOL_TABLE_SIZE 509 

/*scope spaces*/
typedef enum {
	programvar,
	functionlocal,
	formalarg
}scopespace_t;


/* Symbol Types*/
typedef enum{
    GLOBALVAR,  LOCALVAR, FORMAL,
    USERFUNC, LIBFUNC
}SymbolTableType_t;

typedef enum { false, true } bool;

/* Structure of Arguments */
typedef struct Argument {
    char *name;
	SymbolTableType_t type;
	int scope;
	int isActive;
	int lineno;
	scopespace_t space;
    	unsigned offset;     
	struct Argument* next;
} Argument;


/* Structure for symbol table entries */
typedef struct SymbolTableEntry {
    char * name; 
    int scope; 
    int lineno;
    bool isActive;
    SymbolTableType_t type;

    /* Scopespace stuff */
    scopespace_t space;
    unsigned offset;    
    struct expr* value;	    
    int totallocals;
    unsigned iaddress;

    Argument * argList;
	/* Points to the next Symbol of the list */
    struct SymbolTableEntry * next;
	
	/* Connections for the list of scopes -- Collision Lists*/
    struct SymbolTableEntry * nextinScope;
    struct SymbolTableEntry * prvScope;
	
} SymbolTableEntry_t;

/* Symbol Table structure */
typedef struct SymbolTable {
	unsigned int size;
        SymbolTableEntry_t** hash;
}SymTable_t;

/* Function Prototypes */

/*-------------------------------------------------------------------------------------------------------*/

/* Hash Function -- hashing according the name */
int hash(SymTable_t* symbolTable, const char *key );

/* Creates the symbol table */
SymTable_t* createSymbolTable (); 

/* Creates a new entry for the symbol table */
SymbolTableEntry_t* createSymbolTableEntry (char * name, int lineno, int scope, SymbolTableType_t type, scopespace_t currscopespace, unsigned offset);

/* Inserts symbol in the hash table*/
void insertSymbol (SymTable_t* symbolTable, SymbolTableEntry_t* newEntry);  

/* Creates a new argument list for the symbol entry */
Argument* createArgument (char* name,SymbolTableType_t type, int scope, int lineno, scopespace_t space, unsigned offset );

/* Insert argument list in symbol table entry */
void insertArgument (SymTable_t* symbolTable, SymbolTableEntry_t* newSymbol,Argument* newArg);

/* Find the first symbol inserted in that scope */
SymbolTableEntry_t* lookup_firstSymbolInScope(SymTable_t *symbolTable, int scope);

/* Find the last symbol inserted in that scope */
SymbolTableEntry_t* lookup_LastSymbolInScope(SymTable_t *symbolTable, int scope);

/* Find symbol with name, in all scopes -- slow approach */
bool lookup_all_scopes (SymTable_t* mySymTab, char* name);

/* Find symbol with name, in all scopes -- fast approach */
bool lookup_in_all_scope(SymTable_t* symbolTable, char* name);

/* Find symbol with name in scope using collision lists  */
bool lookup_specific_scope (SymTable_t* mySymTab, char* name, int scope); 

/* Find a symbol with a particular name and scope AND return it*/
SymbolTableEntry_t* lookupSpecificScope (SymTable_t* mySymTab, char* name, int scope);

/* Hide Symbols in scope */
void Hide(SymTable_t* mySymTab, int scope);

/* Print symbol table and argument lists  */
void printSymbolTable (SymTable_t* mySymTab); 

/* Print symbol table per scope*/
void printSymbolTableperscope(SymTable_t* symbolTable, int maxScope);

/* Free symbol table */
void freeSymbolTable(SymTable_t* symbolTable);

/* Current Scope Space number */
scopespace_t currscopespace(void);

SymbolTableEntry_t *newtemp(SymTable_t* symbolTable, char *buf, int space, int scope, int line, int offset);

#endif
