%{

	#include "libfunc.h"
	#include "symtable.h"
	
	int yyerror( char * yaccProvidedMessage);
	int yylex(void);

	extern int yylineno;
	extern char * yytext;
	extern FILE * yyin;
	
	int scope = 0, functionScope = 0;
	SymTable_t * mySymbolTable;
	int numberOfUnnamed = 0;
	
	char *argumentName = NULL;
	struct argNames* list = NULL;
	
	int whileLoop = 0 ;
	bool intoFunction = false;
	
	int maxScope = 0;
	
	
%}

%start program

%union{
	int intValue;
	float floatValue;
	char* strValue;
	void *expression; 
}

%token <intValue> INTEGER
%token <floatValue> FLOAT
%token <strValue> IDENTIFIER STRING_DEF
%token COMMENT1 COMMENT2 COMMENT3 IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE 
%token AND NOT OR LOCAL TRUE FALSE NIL  GREATER_EQ LESSER_EQ DOUBLE_COLON DOUBLE_DOT PLUS_PLUS MINUS_MINUS

%right '='
%left OR
%left AND
%nonassoc EQUAL NOTEQUAL
%nonassoc '>' GREATER_EQ '<' LESSER_EQ 
%left '+' '-'
%left '*' '/' '%'
%right NOT PLUS_PLUS MINUS_MINUS UMINUS 
%left '.' DOUBLE_DOT
%left '[' ']'
%left '(' ')'
%left ELSE

%type <intValue> cmpExpr addsubExpr muldivmodExpr booleanExpr
%type <expression> const  returnstmt expr lvalue

%%

program
	: help_stmt 					{ fprintf(stdout, "Rule used: program -> help_stmt\n"); }
	|								{ fprintf(stdout, "Rule used: program -> e\n"); }
	;
	
help_stmt
	: stmt help_stmt 				{ fprintf(stdout, "Rule used: help_stmt -> stmt help_stmt\n"); } 
	| stmt							{ fprintf(stdout, "Rule used: help_stmt -> stmt\n"); }
	;
	
stmt
	: expr ';' 						{ fprintf(stdout, "Rule used: stmt -> expr;\n"); }
	| ifstmt						{ fprintf(stdout, "Rule used: stmt -> ifstmt\n"); }
	| whilestmt						{ fprintf(stdout, "Rule used: stmt -> whilestmt\n"); }
	| forstmt						{ fprintf(stdout, "Rule used: stmt -> forstmt\n");}
	| returnstmt					{ fprintf(stdout, "Rule used: stmt -> returnstmt\n"); }
	| BREAK ';'						{ 
		if (!whileLoop){
		/* Wrong placement of break */
				fprintf(stdout, "ERROR:: Break not in loop. Program will be terminated. \n");
				exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Rule used: stmt -> BREAK;\n"); 
	}
	| CONTINUE ';'					{ 
		if (!whileLoop){
		/* Wrong placement of continue */
					fprintf(stdout, "ERROR:: Continue not in loop. Program will be terminated. \n");
					exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Rule used: stmt -> CONTINUE;\n"); 
	}
	| block							{ fprintf(stdout, "Rule used: stmt -> block\n"); }
	| funcdef						{ fprintf(stdout, "Rule used: stmt -> funcdef\n"); }
	| comment						{ fprintf(stdout, "Rule used: stmt -> comment\n");}
	| ';'							{ fprintf(stdout, "Rule used: stmt -> ;\n");}
	;	
	
comment
	: COMMENT1						{ fprintf(stdout, "Rule used: comment -> Single Line Comment\n");}
	| COMMENT2						{ fprintf(stdout, "Rule used: comment -> Multi Line Comment\n");}
	| COMMENT3						{ fprintf(stdout, "Rule used: comment -> Nested Comment\n");}
	;	

expr
	: assignexpr					{ fprintf(stdout, "Rule used: expr -> assignexpr\n"); }
	| term							{ fprintf(stdout, "Rule used: expr -> term\n"); }
	| addsubExpr					{ fprintf(stdout, "Rule used: expr -> addsubExpr\n"); }
	| muldivmodExpr					{ fprintf(stdout, "Rule used: expr -> muldivmodExpr\n"); }
	| cmpExpr				 		{ fprintf(stdout, "Rule used: expr -> cmpExpr\n"); }
	| booleanExpr					{ fprintf(stdout, "Rule used: expr -> booleanExpr\n");}
	;	
	
addsubExpr
	: expr '+' expr 				{  fprintf(stdout, "Rule used: addsubExpr -> expr + expr\n"); }
	| expr '-' expr 				{  fprintf(stdout, "Rule used: addsubExpr -> expr - expr\n"); }
	;
	
muldivmodExpr
	: expr '*' expr 				{ fprintf(stdout, "Rule used: muldivmodExpr -> expr * expr\n"); }
	| expr '/' expr 				{ fprintf(stdout, "Rule used: muldivmodExpr -> expr / expr\n"); }
	| expr '%' expr 				{ fprintf(stdout, "Rule used: muldivmodExpr -> expr % expr\n"); }
	;
	
cmpExpr
	: expr '>' expr 				{ fprintf(stdout, "Rule used: cmpExpr -> expr > expr\n"); }
	| expr GREATER_EQ expr 			{ fprintf(stdout, "Rule used: cmpExpr -> expr >= expr\n"); }
	| expr '<' expr 				{ fprintf(stdout, "Rule used: cmpExpr -> expr < expr\n"); }	
	| expr LESSER_EQ expr 			{ fprintf(stdout, "Rule used: cmpExpr -> expr <= expr\n"); }
	| expr EQUAL expr 				{ fprintf(stdout, "Rule used: cmpExpr -> expr == expr\n"); }
	| expr NOTEQUAL expr 			{ fprintf(stdout, "Rule used: cmpExpr -> expr != expr\n"); }
	;
	
booleanExpr	
	: expr AND expr 				{ fprintf(stdout, "Rule used: booleanExpr -> expr AND expr\n"); }
	| expr OR expr 					{ fprintf(stdout, "Rule used: booleanExpr -> expr OR expr\n"); }
	;

term
	: '(' expr ')' 	 				{ fprintf(stdout, "Rule used: term -> (expr) \n");   } 
	| '-' expr %prec UMINUS			{ fprintf(stdout, "Rule used: term -> -expr \n"); 	 }
	| NOT expr						{ fprintf(stdout, "Rule used: term -> NOT expr \n"); }
	| plus lvalue 					{ 
		int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($2,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator ++ to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, $2);
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == $2){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator ++ to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
		
		fprintf(stdout, "Rule used: term -> ++lvalue \n"); 
	}
	| lvalue plus				{ 
		int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator ++ to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, $1);
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == $1){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator ++ to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
	fprintf(stdout, "Rule used: term -> lvalue++ \n"); }
	| minus lvalue			{ 
			int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($2,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator -- to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, $2);
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == $2){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator -- to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
			
			fprintf(stdout, "Rule used: term -> --lvalue \n"); }
	| lvalue minus			{ 
	
	int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator -- to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, $1);
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == $1){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator -- to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
	fprintf(stdout, "Rule used: term -> lvalue-- \n"); }		
	| primary						{ fprintf(stdout, "Rule used: term -> primary \n");  }	
	;

plus
	: PLUS_PLUS						{ fprintf(stdout, "plus -> ++ \n");  }	
	
minus
	: MINUS_MINUS 					{ fprintf(stdout, "minus -> -- \n");  }	
	
assignexpr 
	: lvalue '=' expr 			{ 
		/* We cannot assign in lib function and user function -- throw error*/
		int remainingLibFun = 0, isShadowed = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot assign to library function");
			
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, $1);
			if (varExists){
				int tempScope = scope;
				bool userFuncexist = false;
				while (tempScope >= 0){
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == $1){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot assign to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			}else{}
		}
		
		fprintf(stdout, "Rule used: assignexpr -> lvalue = expr; \n"); 
	}		
	;
	
primary 
	: lvalue						{ fprintf(stdout, "Rule used: primary -> lvalue \n"); }
	| call							{ fprintf(stdout, "Rule used: primary -> call \n"); }
	| objectdef						{ fprintf(stdout, "Rule used: primary -> objectdef \n"); }
	|'(' funcdef ')'				{ fprintf(stdout, "Rule used: primary -> (funcdef) \n"); }
	| const							{ fprintf(stdout, "Rule used: primary -> const \n"); }					
	;

lvalue 
	: IDENTIFIER					{ 
		$$ = $1;
		bool varExists;
		SymbolTableEntry_t* newEntry, * existVar;
		bool existsInSameScope = false;
		bool existsDiff = false;
		
		int i = 0;
		/* Look up in symbol table if this variable exists */
		varExists = lookup_in_all_scope(mySymbolTable, $1);
		int remainingLibFun = 0, isShadowed = 0;
		
		
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
	
		/*CASE that variable is shadowing lib function */	
		if (isShadowed && scope != 0){
			fprintf (stdout, "ERROR: Variable %s is shadowing library function.\n", $1);
		}else{
			
			if (varExists){
				
				/* if there is a id in same scope we refer to it */
				existVar = lookup_firstSymbolInScope(mySymbolTable, scope);
				while (existVar!=NULL){
					if (!strcmp(existVar->name, $1)){
						break;
					}
					existVar = existVar->nextinScope;
				}
					
				if (existVar != NULL){
					existsInSameScope = true;
					if (existVar->type != USERFUNC){
						fprintf(stdout, "ok:: Variable %s refers to an active variable in same scope.\n", $1);
					}else{
						fprintf(stdout, "ERROR : Cannot change the use of symbol %s from function to variable.\n", $1);
					}
				}
				
				/* if there is an id in different scope */
				if (existsInSameScope == false){
					/* Find in what scope the variable exists */
					for (i = 0; i < scope; i++){
						existsDiff = lookup_specific_scope (mySymbolTable, $1, i);
						
						if (existsDiff) break;
					}
					
					if (existsDiff){
						existVar = lookup_firstSymbolInScope(mySymbolTable, i);
						while (existVar!=NULL){
							if (!strcmp(existVar->name, $1)){
								break;
							}
							existVar = existVar->nextinScope;
						}
						
						if (existVar->scope < scope ){
							if (intoFunction == 1){
								fprintf(stdout, "ERROR : Cannot use symbol %s into this function.\n", $1);
							}else{
								fprintf(stdout, "ok:: Symbol %s has access to this variable.\n", $1);
							
							}
						}
						else{
						/* Different scope */
							if (scope == 0){
								newEntry = createSymbolTableEntry ($1, yylineno, scope, GLOBALVAR);
							}else{
								newEntry = createSymbolTableEntry ($1, yylineno, scope, LOCALVAR);
							}			
							insertSymbol (mySymbolTable, newEntry);
						
						}
					}
				}
				
			
			}else{
			/* if the variable doesn't exist insert it in symbol table. */
				if (scope == 0){
					newEntry = createSymbolTableEntry ($1, yylineno, scope, GLOBALVAR);
				}else{
					newEntry = createSymbolTableEntry ($1, yylineno, scope, LOCALVAR);
				}			
				insertSymbol (mySymbolTable, newEntry);
			
			}
		}
		
		fprintf(stdout, "Rule used: lvalue  -> IDENTIFIER\n"); 
	}
	| LOCAL IDENTIFIER				{ 
		$$ = $2;
		bool varExists = false;
		int remainingLibFun = 0, isShadowed = 0;
		SymbolTableEntry_t* newEntry;
		
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($2,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		/*CASE that variable is shadowing lib function */	
		if (isShadowed && scope != 0){
			fprintf (stdout, "ERROR: Variable %s is shadowing library function\n", $2);
		}else if(isShadowed && scope == 0){
			fprintf (stdout, "ok:: %s refers to library function\n", $2);
		}else{
			varExists = lookup_specific_scope (mySymbolTable, $2, scope);
			if (varExists){
				fprintf(stdout,"ok:: Variable %s refers to a local variable in same scope\n", $2);
			
			}else{
				if (scope == 0){
					newEntry = createSymbolTableEntry ($2, yylineno, scope, GLOBALVAR);
				}else{
					newEntry = createSymbolTableEntry ($2, yylineno, scope, LOCALVAR);
				}			
				insertSymbol (mySymbolTable, newEntry);
			}
			
		}
	
		fprintf(stdout, "Rule used: lvalue  -> LOCAL IDENTIFIER\n"); 
	
	}
	| DOUBLE_COLON IDENTIFIER		{ 
		bool varExists = false;
		$$ = $2;
		varExists = lookup_specific_scope (mySymbolTable, $2, 0);
		if (varExists){
			fprintf(stdout, "Variable %s refers to global variable\n",$2);
		}else{
			fprintf(stdout, "ERROR: Variable %s doesn't exist in global scope\n",$2);
		}
	
		fprintf(stdout, "Rule used: lvalue  -> DOUBLE_COLON IDENTIFIER\n"); 
	}
	| member						{ fprintf(stdout, "Rule used: lvalue  -> member\n"); }
	;
	
member 
	: lvalue '.' IDENTIFIER			{ 
		int remainingLibFun = 0, isShadowed = 0;
		
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use library function as lvalue.ID");
		}
		
		fprintf(stdout, "Rule used: member -> lvalue.IDENTIFIER\n"); 
	}
	| lvalue '[' expr ']'			{ 
		int remainingLibFun = 0, isShadowed = 0;
		
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp($1,libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use library function as lvalue[ID]");
		}
		fprintf(stdout, "Rule used: member -> lvalue [expr]\n"); }
	| call'.' IDENTIFIER			{ fprintf(stdout, "Rule used: member -> call.IDENTIFIER\n"); }
	| call '[' expr ']'				{ fprintf(stdout, "Rule used: member -> call [expr] \n"); }
	;	

call 
	: call normcall 				{ fprintf(stdout, "Rule used: call  -> call normcall  \n"); }
	| lvalue callsuffix 			{ fprintf(stdout, "Rule used: call  -> lvalue callsuffix \n"); }
	| '(' funcdef ')' normcall 		{ fprintf(stdout, "Rule used: call  -> '(' funcdef ')' normcall  \n"); }
	;
	
callsuffix 
	: normcall 						{ fprintf(stdout, "Rule used: callsuffix  -> normcall \n"); }
	| methodcall					{ fprintf(stdout, "Rule used: callsuffix  -> methodcall \n"); }
	;

normcall 
	: '(' elist ')'					{ fprintf(stdout, "Rule used: normcall  -> (elist) \n");}
	;	

methodcall 
	: DOUBLE_DOT IDENTIFIER '(' elist ')' { fprintf(stdout, "Rule used: methodcall  -> .. IDENTIFIER(elist) \n");}
	;	

elist 
	: expr listed_expr						{ fprintf(stdout, "Rule used: elist -> expr listed_eexpr\n"); }
	| expr									{ fprintf(stdout, "Rule used: elist -> expr\n"); }
	|
	;	
	
listed_expr
	: ',' expr listed_expr					{ fprintf(stdout, "Rule used: elist -> ',' expr listed_eexpr \n"); }
	| ',' expr								{ fprintf(stdout, "Rule used: elist -> ',' expr\n"); }
	;

eelist 
	: expr 							{ fprintf(stdout, "Rule used: elist -> expr\n"); }
	| expr listed_eexpr 			{ fprintf(stdout, "Rule used: elist -> listed_expr\n"); }
	;	
	
listed_eexpr
	: ',' expr 						{ fprintf(stdout, "Rule used: elist -> elist ',' expr\n"); }
	| ',' expr listed_eexpr
	;
	
	
objectdef 
	: '[' ']' 						{ fprintf(stdout, "Rule used: objectdef -> [] \n"); }
	| '[' eelist ']' 				{ fprintf(stdout, "Rule used: objectdef -> [elist] \n"); }
	| '[' indexed ']'				{ fprintf(stdout, "Rule used: objectdef -> [indexed] \n"); }
	;

indexed 
	: indexedelem indexed_expr 				{ fprintf(stdout, "Rule used: indexed -> indexedelem\n"); }
	| indexedelem							{ fprintf(stdout, "Rule used: indexed -> indexed indexed_expr \n"); }
	; 
	
indexed_expr
	: ',' indexedelem indexed_expr  			{ fprintf(stdout, "Rule used: indexed -> ,indexedelem  \n"); }
	| ',' indexedelem						{ fprintf(stdout, "Rule used: indexed -> ,indexedelem  \n"); }
	;
	
indexedelem 
	: '{' expr ':' expr '}'			{ fprintf(stdout, "Rule used: indexedelem -> '{' expr ':' expr '}'	 \n"); }
	;	
	
block 
	: '{' {++scope; if (maxScope < scope) {maxScope = scope;}} stmt morestmt '}' {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{' stmt '}' \n"); 
	}
	| '{' {++scope;} '}' {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{'  '}' \n"); 
	}
	;

morestmt
	: stmt morestmt					{ fprintf(stdout, "Rule used: morestmt -> stmt \n"); }
	|								{ fprintf(stdout, "Rule used: morestmt -> e \n"); }
	;

funcdef 
	: FUNCTION IDENTIFIER {
		if (scope == 0) {
			++functionScope;
		}else{
			functionScope = scope + 1;
		}		
		intoFunction = true; 
		printf("Func:%s %d", $2, functionScope);
		} '(' idlist ')' { 
			
			int remainingLibFun = 0, isShadowed = 0;
			/* Look up if the IDENTIFIER is shadowing library functions. */
			while(remainingLibFun < MAXLIBFUN){
				if (strcmp($2,libraryFunctions[remainingLibFun]) == 0){
					isShadowed = 1; 
					break;
				}
				remainingLibFun++;
			}
			/* If is shadowed print error and continue the syntax analysis. */
			if (isShadowed == 1){
				fprintf(stdout, "Error: User function %s tries to shadow library function.\n", $2); 
			}else{
				bool functionExists = lookup_specific_scope (mySymbolTable, $2, scope);
				if (functionExists){
					fprintf(stdout, "ERROR: User function name exists in scope.\n");
				} else {
					Argument * newArgument;	// If function has arguments insert in argument list
					SymbolTableEntry_t* newEntry = createSymbolTableEntry ($2, yylineno, scope, USERFUNC);
					
					while (list != NULL){
						bool formalExists = false;
						int remaining = 0, isShadowed = 0;
						/* Look up if the formal is shadowing library functions. */
						while(remaining < MAXLIBFUN){
							if (strcmp(list->name,libraryFunctions[remaining]) == 0){
								isShadowed = 1; 			
								break;
							}
							remaining++;
						}
						/* If formal shadows a libfunc print error*/
						if (isShadowed == 1){
							fprintf(stdout, "ERROR: lif func: %s is shadowed by variable %s:\n", libraryFunctions[remaining], list->name);
						}else{
								/*look if a formal exists in symbol table for this scope */
								formalExists = lookup_specific_scope (mySymbolTable, list->name, functionScope);
								
								if (formalExists == true){
									fprintf(stdout, "ERROR: Formal name exists in scope.\n");
								}else{
									
									newArgument=createArgument(list->name,FORMAL, functionScope, yylineno);
									insertArgument(mySymbolTable, newEntry, newArgument);
								}
						}
						list = list->next;
					}
					
					deleteList(&list);				
					insertSymbol(mySymbolTable, newEntry);
				}
				
	}

} block {functionScope--; intoFunction = false; fprintf(stdout, "Rule used: funcdef -> FUNCTION IDENTIFIER (idlist) block \n"); }
	| FUNCTION { 
		if (scope == 0) {
			++functionScope;
		}else{
			functionScope = scope + 1;
		}
		intoFunction = true;} '(' idlist ')'{
					char * buf;
					numberOfUnnamed++;
					
					buf = counterUnnamed(numberOfUnnamed);
				
					Argument * newArgument;	// If function has arguments insert in argument list
					SymbolTableEntry_t* newEntry = createSymbolTableEntry (buf, yylineno, scope, USERFUNC);
					
					while (list != NULL){
						bool formalExists = false;
						int remaining = 0, isShadowed = 0;
						/* Look up if the formal is shadowing library functions. */
						while(remaining < MAXLIBFUN){
							if (strcmp(list->name,libraryFunctions[remaining]) == 0){
								isShadowed = 1; 			
								break;
							}
							remaining++;
						}
						/* If formal shadows a libfunc print error*/
						if (isShadowed == 1){
							fprintf(stdout, "ERROR: lif func: %s is shadowed by variable %s:\n", libraryFunctions[remaining], list->name);
						}else{
								/*look if a formal exists in symbol table for this scope */
								formalExists = lookup_specific_scope (mySymbolTable, list->name, functionScope);
								
								if (formalExists == true){
									fprintf(stdout, "ERROR: Formal name exists in scope.\n");
								}else{
									newArgument=createArgument(list->name,FORMAL, functionScope, yylineno);
									insertArgument(mySymbolTable, newEntry, newArgument); /* Insert args in argument list AND in symbol table */
								}
						}
						list = list->next;
					}
					
					deleteList(&list);				
					insertSymbol(mySymbolTable, newEntry);
							
				
	} block	{ functionScope--; intoFunction = false; fprintf(stdout, "Rule used: funcdef -> FUNCTION (idlist) block \n"); }	
	;	
	
const 
	: INTEGER 						{ fprintf(stdout, "Rule used: const -> INTEGER \n"); }
	| FLOAT 						{ fprintf(stdout, "Rule used: const -> FLOAT \n"); }
	| STRING_DEF 					{ fprintf(stdout, "Rule used: const -> STRING_DEF \n"); }
	| NIL 							{ fprintf(stdout, "Rule used: const -> NIL\n"); }
	| TRUE 							{ fprintf(stdout, "Rule used: const -> TRUE\n"); }
	| FALSE							{ fprintf(stdout, "Rule used: const -> FALSE\n"); }
	;

idlist 
	: IDENTIFIER { 
	/* Single arg:: Save argument name, insert it into a list and print the */
		argumentName = malloc(strlen($1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(&list, argumentName);
		//print(list);
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER \n"); 
	}
	| IDENTIFIER  helpidlist  		{ 
		
		argumentName = malloc(strlen($1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(& list, argumentName);	
		//print(list);
	
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER  helpidlist \n"); 
	}
	| 								{ fprintf(stdout, "Rule used: idlist -> e\n"); }
	| comment						{ fprintf(stdout, "Rule used: idlist -> comment\n"); }
	| IDENTIFIER comment  { 
	/* Single arg:: Save argument name, insert it into a list and print the */
		argumentName = malloc(strlen($1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(&list, argumentName);
		//print(list);
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER comment \n"); 
	}
	| IDENTIFIER  helpidlist comment { 
		
		argumentName = malloc(strlen($1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(& list, argumentName);	
		//print(list);
	
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER  helpidlist comment \n"); 
	}
	;
	
helpidlist 
	: ',' IDENTIFIER helpidlist 	{ 
		argumentName = malloc(strlen($2) * sizeof(char));
		strcpy(argumentName, $2);
		insert(& list, argumentName);
		//print(list);		
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER helpidlist\n"); }
	| ',' IDENTIFIER				{ 
		argumentName = malloc(strlen($2) * sizeof(char));
		strcpy(argumentName, $2);
		insert(& list, argumentName);	
		//print(list);
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER	\n"); 
		}
	;	
	
ifstmt 
	: IF '(' expr ')' stmt 			{ fprintf(stdout, "Rule used: ifstmt -> IF \'(\' expr \')\' stmt\n"); }
	| IF '(' expr ')' stmt ELSE stmt { fprintf(stdout, "Rule used: ifstmt -> IF \'(\' expr \')\' stmt ELSE stmt\n"); }
	;

whilestmt 
	: WHILE {++whileLoop;} '(' expr ')'  stmt { whileLoop--; fprintf(stdout, "Rule used: whilestmt -> WHILE '(' expr ')' stmt\n"); }
	;	
	
forstmt
	: FOR {++whileLoop;} '(' elist ';' expr ';' elist ')'  stmt { whileLoop--; fprintf(stdout, "Rule used: forstmt -> FOR \n"); }
	;	
	
returnstmt
	: RETURN ';' 					{ 
	if (intoFunction == false){
		fprintf(stdout, "ERROR: Return not in function. \n");
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "Rule used: returnstmt -> RETURN;\n"); 
	}
	| RETURN expr ';'				{ 
		if (functionScope == 0){
			fprintf(stdout, "ERROR: Return not in function. Program will be terminated. \n");
			exit(EXIT_FAILURE);
		}else {
			/*bool varExists = false;
			SymbolTableEntry_t* entry = lookup_firstSymbolInScope(mySymbolTable, functionScope);
			varExists = lookup_specific_scope(mySymbolTable,  $2, functionScope);
			if (varExists){
				while(entry != NULL){
					if (!strcmp(entry->name,  $2)){
						break;
					}
					entry = entry->nextinScope;
				}
			}
			if (!varExists || (varExists && entry->scope != 0)){
				fprintf(stdout, "ERROR: Function returns invalid expression. \n");
			}
		*/
		}
		fprintf(stdout, "Rule used: returnstmt -> RETURN expr ;\n"); 
	}
	;
	
	
%%

/* Function for recognizing errors*/
int yyerror(char * yaccProvidedMessage){
	fprintf(stderr, "%s: at line %d, before token: %s\nINPUT NOT VALID\n", yaccProvidedMessage, yylineno, yytext);
}

int main (int argc, char ** argv){
	
	int remainingLibFun = 0;
	
	mySymbolTable = createSymbolTable();
	/* Insert in symbol table all library functions with scope 0 and type LIBFUNC*/
	while(remainingLibFun < MAXLIBFUN){
	
		SymbolTableEntry_t* newEntry = createSymbolTableEntry (libraryFunctions[remainingLibFun], 0, 0, LIBFUNC);
		insertSymbol(mySymbolTable, newEntry);
		remainingLibFun++;
		
	}
	
	if (argc > 1){
		if (!(yyin = fopen(argv[1], "r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}

	}
	else{
		yyin = stdin;
	}
	
	yyparse();
	
	printSymbolTable(mySymbolTable);
	//printSymbolTableperscope(mySymbolTable, maxScope);
	freeSymbolTable(mySymbolTable);
	
	return 0;
}