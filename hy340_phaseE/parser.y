%{

	#include "libfunc.h"
	#include "symtable.h"
	
	#include "generate_final_code.h"
	#include "ExpressionTable.h"
	#include "stack.h"
	#include "vstack.h"
	#include "vlist.h"
	
	int yyerror( char * yaccProvidedMessage);
	int yylex(void);

	extern int yylineno;
	extern char * yytext;
	extern FILE * yyin;
	
	int scope = 0, functionScope = 0;
	SymTable_t * mySymbolTable;
	int numberOfUnnamed          = 0;
	int tempVarCounter           = 0; 

	char *argumentName           = NULL;
	struct argNames* list        = NULL;
	
	int whileLoop                = 0 ;
	
	int isTableItem 	     = false;
	
	int maxScope                 = 0;
	int formalLine 	             = 0;
	
	FILE *fp, *fp_instr;
	FILE * f;
	
	stack* localArgStack;

	
	vstack_t numStack;
	vstack_t strStack;
	vstack_t libFuncStack;
	vstack_t userFuncStack;
	
	looplist *breaklist;
	looplist *continuelist;

	int intoTable 				= 0;
	int intoFunction            = 0;
	bool error = false;
		
	
%}

%start program

%union{

	int intValue;
	float floatValue;
	char* strValue;
	void* expression;
	struct SymbolTableEntry* sym;
	struct forStruct* forTest;

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
//%left ELSE

%expect 1

%type <expression> const
%type <expression>  primary term expr 
%type <expression> lvalue help_stmt stmt call objectdef
%type <expression> addsubExpr cmpExpr muldivmodExpr booleanExpr
%type <expression> assignexpr member
%type <strValue> funcname
%type <sym> funcprefix funcdef
%type <intValue> M ifprefix elseprefix
%type <intValue> whilestart whilecond for_N for_M
%type <forTest> forprefix
%type <expression> loopstmt
%type <expression> elist listed_expr eelist listed_eexpr  indexed 
%type <expression> indexed_expr indexedelem



%%

program:					{ 
	
								fp = fopen("quads.txt","w+"); 
								fp_instr = fopen("instructions.txt", "w+"); 
								f = fopen ("instr.bin", "wb+");
								printf("files opened\n");  }
	help_stmt 				{ fprintf(stdout, "Rule used: program -> help_stmt\n"); }
	|						{ fprintf(stdout, "Rule used: program -> e\n"); }
	;
	
help_stmt
	: stmt help_stmt 		{ fprintf(stdout, "Rule used: help_stmt -> stmt help_stmt\n"); } 
	| stmt					{ 
								expr* temp = $1;
								$$ = temp;
								fprintf(stdout, "Rule used: help_stmt -> stmt\n");
							}
	;


	
/* At the end of a statement we reset the counter. All the variables are considered available. */
stmt
	: expr ';' 					{resetTemp(); fprintf(stdout, "Rule used: stmt -> expr;\n"); }
	| ifstmt					{resetTemp(); fprintf(stdout, "Rule used: stmt -> ifstmt\n"); }
	| whilestmt					{resetTemp(); fprintf(stdout, "Rule used: stmt -> whilestmt\n"); }
	| forstmt					{resetTemp(); fprintf(stdout, "Rule used: stmt -> forstmt\n");}
	| returnstmt				{resetTemp(); fprintf(stdout, "Rule used: stmt -> returnstmt\n"); }
	| BREAK ';'					{Manage_break(whileLoop); fprintf(stdout, "Rule used: stmt -> BREAK;\n"); }
	| CONTINUE ';'				{Manage_continue(whileLoop); fprintf(stdout, "Rule used: stmt -> CONTINUE;\n");}
	| block						{resetTemp(); fprintf(stdout, "Rule used: stmt -> block\n"); }
	| funcdef					{resetTemp(); fprintf(stdout, "Rule used: stmt -> funcdef\n"); }
	| comment					{resetTemp(); fprintf(stdout, "Rule used: stmt -> comment\n");}
	| ';'						{resetTemp(); fprintf(stdout, "Rule used: stmt -> ;\n");}
	;	
	
comment
	: COMMENT1					{ fprintf(stdout, "Rule used: comment -> Single Line Comment\n");}
	| COMMENT2					{ fprintf(stdout, "Rule used: comment -> Multi Line Comment\n");}
	| COMMENT3					{ fprintf(stdout, "Rule used: comment -> Nested Comment\n");}
	;	

expr
	: assignexpr			{  expr* temp = $1; $$ = temp;  fprintf(stdout, "Rule used: expr -> assignexpr\n"); }
	| term					{ 
								expr* temp = $1;
								$$ = temp;
								fprintf(stdout, "Rule used: expr -> term\n"); 
							}
									
	| addsubExpr			{ expr* temp = $1; $$ = temp; fprintf(stdout, "Rule used: expr -> addsubExpr\n"); }
	| muldivmodExpr			{ expr* temp = $1; $$ = temp; fprintf(stdout, "Rule used: expr -> muldivmodExpr\n"); }
	| cmpExpr				{ expr* temp = $1; $$ = temp; fprintf(stdout, "Rule used: expr -> cmpExpr\n"); }
	| booleanExpr			{ expr* temp = $1; $$ = temp; fprintf(stdout, "Rule used: expr -> booleanExpr\n"); }
	;	
	
addsubExpr
	: expr '+' expr     {  
			
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
						
				/*Read two operands*/
				arg1 = $1;
				arg2 = $3;
				
				/* The expression will be an arithmetic expression and we will create a new hidden var for it. */				
				result = newexpr(arithexpr_e); 						
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset()); 
				
				
				checkAddition(arg1);
				checkAddition(arg2);															
					
				if (arg1->type == constnum_e && arg2->type == constnum_e){
					result->numConst = arg1->numConst + arg2->numConst;
				}
						
					
				emit(add, arg1, arg2, result,-1, yylineno);		
								
				$$ = result;													
					
				fprintf(stdout, "Rule used: addsubExpr -> expr + expr\n"); 
			}
	| expr '-' expr 	{  
				
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(arithexpr_e);
				result->sym  = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
								

				checkSubtraction(arg1);
				checkSubtraction(arg2);
				
				if (arg1->type == constnum_e && arg2->type == constnum_e){
					result->numConst = arg1->numConst - arg2->numConst;
				}

				emit(sub, arg1, arg2, result,-1, yylineno);
							
				$$ = result;
				fprintf(stdout, "Rule used: addsubExpr -> expr - expr\n"); 
			}
	;

	
muldivmodExpr
	: expr '*' expr 	{ 
				
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
						
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(arithexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				checkMultiplication(arg1);
				checkMultiplication(arg2);
				
				if (arg1->type == constnum_e && arg2->type == constnum_e){
					result->numConst = arg1->numConst * arg2->numConst;
				}

				emit(mul, arg1, arg2, result, -1, yylineno);
				
				//result->falselist =  mergelist(arg1->falselist, arg2->falselist);
				//result->truelist  =  mergelist(arg1->truelist , arg2->truelist);	
										
				$$ = result;

						
				fprintf(stdout, "Rule used: muldivmodExpr -> expr * expr\n"); 
			}
	| expr '/' expr     { 
				/* Read 2 arguments */
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
								
				arg1 = $1;
				arg2 = $3;
					
				result = newexpr(arithexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				checkDivision(arg1);
				checkDivision(arg2);
			
				if (arg1->type == constnum_e && arg2->type == constnum_e){
					if(arg2->numConst == 0){
						fprintf(stdout, "Warning <#%d>: Devision by zero\n", yylineno);
					}
					result->numConst = arg1->numConst / arg2->numConst;
				}
				
				
				emit(divv, arg1, arg2, result, -1, yylineno);									
									
				$$ = result;
				fprintf(stdout, "Rule used: muldivmodExpr -> expr / expr\n"); 
			}
	| expr '%' expr 	{ 

				/* Read 2 arguments */
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				
				arg1 = $1;
				arg2 = $3;
				
				result = newexpr(arithexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				checkModular(arg1);
				checkModular(arg2);

				if (arg1->type == constnum_e && arg2->type == constnum_e){
					if(arg2->numConst == 0){
						fprintf(stdout, "Warning <#%d>: Mod by zero\n", yylineno);
					}
					//result->numConst = (int)arg1->numConst % (int)arg2->numConst;
				}
				
				emit(mod, arg1, arg2, result,-1, yylineno);
								
				$$ = result;
				
				fprintf(stdout, "Rule used: muldivmodExpr -> expr mod expr\n"); 
		}
;


cmpExpr
	: expr '>' expr 		    { 

				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				int bptemp1 = 0, bptemp2 = 0;
		
				arg1 = $1;
				arg2 = $3;
					
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				//makeList(&(result->truelist), nextquadlabel());
				//makeList(&(result->falselist), nextquadlabel()+1);

				emit(if_greater, arg1, arg2, NULL, nextquadlabel() + 3 , yylineno);
				
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;	

				
				emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL, NULL,nextquadlabel() + 2, yylineno); 
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);

						
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist, bptemp1);
				
				$$ = result;
				fprintf(stdout, "Rule used: cmpExpr -> expr > expr\n"); 

				}
	| expr GREATER_EQ expr 		{ 

				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				
				int bptemp1 = 0, bptemp2 = 0;
							
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				//makeList(&(result->truelist), nextquadlabel());
				//makeList(&(result->falselist), nextquadlabel()+1);

				emit(if_greatereq, arg1, arg2, NULL, nextquadlabel() + 3, yylineno);
			
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;
				
				emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL, NULL, nextquadlabel() + 2, yylineno); 
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);

				
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);		
									
				$$ = result;

				fprintf(stdout, "Rule used: cmpExpr -> expr >= expr\n"); 
				}
	| expr '<' expr 			{ 

				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				int bptemp1 = 0, bptemp2 = 0;
				
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				

				//makeList(&(result->truelist) , nextquadlabel());
				//makeList(&(result->falselist), nextquadlabel()+1);

				emit(if_less, arg1, arg2, NULL, nextquadlabel() + 3, yylineno);
				
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;

				emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL, NULL, nextquadlabel()+2, yylineno);
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);

				
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);			

				$$ = result;
						
				fprintf(stdout, "Rule used: cmpExpr -> expr < expr\n"); 
				}	
	| expr LESSER_EQ expr 		{ 
	
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				
				int bptemp1 = 0, bptemp2 = 0;
												
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());

				//makeList(&(result->truelist), nextquadlabel());
				//makeList(&(result->falselist),nextquadlabel()+1);

				emit(if_lessereq, arg1, arg2, NULL, nextquadlabel() + 3, yylineno);
				
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;
	
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL, NULL, nextquadlabel()+2, yylineno);
				emit(assign, newexpr_constbool(0), NULL, result,-1, yylineno);

				
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);			
							
				$$ = result;

				fprintf(stdout, "Rule used: cmpExpr -> expr <= expr\n"); 
				}
	| expr EQUAL expr 			{ 
									
									
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				int bptemp1 = 0, bptemp2 = 0;
					
				
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());

				//makeList(&(result->truelist), nextquadlabel());
				//makeList(&(result->falselist), nextquadlabel()+1);

				emit(if_eq, arg1, arg2, NULL, nextquadlabel() + 3, yylineno);
				
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;
				
	
				emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL,NULL, nextquadlabel()+2, yylineno);
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);	

				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);				
						
				$$ = result;

				fprintf(stdout, "Rule used: cmpExpr -> expr == expr\n"); 
				}
	| expr NOTEQUAL expr 		{ 
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				int bptemp1 = 0, bptemp2 = 0;
					
				arg1 = $1;
				arg2 = $3;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());

				//makeList(&(result->truelist), nextquadlabel());
				//makeList(&(result->falselist), nextquadlabel()+1);

				emit(if_noteq, arg1, arg2, NULL, nextquadlabel() + 3, yylineno);
				
				//bptemp1 = nextquadlabel(); 
				//bptemp2 = nextquadlabel()+2;

				emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				emit(jump, NULL, NULL, NULL, nextquadlabel()+2, yylineno);
				emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);		
				
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);	

				$$ = result;
				fprintf(stdout, "Rule used: cmpExpr -> expr != expr\n"); 
				}
	;

	
booleanExpr	
	: expr AND M expr 			{ 

				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;		
				int bptemp1 = 0, bptemp2 = 0;
										
				arg1 = $1;
				arg2 = $4;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());

				 
	  			//backpatch (arg1->truelist, $3);
				//result->falselist = mergelist (arg1->falselist, arg2->falselist);
				//result->truelist = arg2->truelist;

				//bptemp1 = nextquadlabel();
				//bptemp2 = nextquadlabel()+2;
				
				emit(and, arg1, arg2, result, -1, yylineno);
				
				//emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);	
				//emit(jump, NULL, NULL, NULL, nextquadlabel() + 2, yylineno);
				//emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
					
					
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);		
									
				$$ = result;
				fprintf(stdout, "Rule used: booleanExpr -> expr AND expr\n"); 

			}
	| expr OR M expr 			{ 
				expr *arg1 = NULL, *arg2 = NULL, *result = NULL;
				int bptemp1 = 0, bptemp2 = 0;
				
				
				arg1 = $1;
				arg2 = $4;
						
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				//switch(arg1->type){

					//case conststring_e : arg1->type = constbool_e;break;
					//case constnum_e    : arg1->type = constbool_e;break;
					//case programfunc_e : arg1->type = constbool_e;break;
					//case libraryfunc_e : arg1->type = constbool_e;break;
					//case tableitem_e   : arg1->type = constbool_e;break;
					//case newtable_e    : arg1->type = constbool_e;break;
					//case nil_e         : arg1->type = constbool_e;break;

				//}
			 
  				//backpatch (arg1->falselist, $3);

				//result->truelist = mergelist (arg1->truelist, arg2->truelist);
				//result->falselist = arg2->falselist;

				emit(or, arg1, arg2, result, -1, yylineno);

				//bptemp1 = nextquadlabel();
				//bptemp2 = nextquadlabel()+2;

				//emit(assign, newexpr_constbool(0), NULL, result, -1, yylineno);	
				//emit(jump, NULL, NULL, NULL, nextquadlabel() + 2, yylineno);
				//emit(assign, newexpr_constbool(1), NULL, result, -1, yylineno);	
				
				
				//backpatch(result->truelist,bptemp2);
				//backpatch(result->falselist,bptemp1);
						
				$$ = result;
				fprintf(stdout, "Rule used: booleanExpr -> expr OR expr\n"); 
			}
	;


M
	:	{ 
			
			$$ = nextquadlabel();
			fprintf(stdout, "Rule used: M -> empty\n"); 
		}
;



term
	: '(' expr ')'			{ expr * temp = $2; $$ = temp;  fprintf(stdout, "Rule used: term -> (expr) \n");  } 
	| '-' expr %prec UMINUS	{ 
				expr *result = NULL, *arg1 = $2;
				
				checkminus(arg1); 						
				
				result = newexpr(arithexpr_e);  		
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				emit(uminus, arg1, NULL, result, -1, yylineno);	
							
				$$ = result;
				fprintf(stdout, "Rule used: term -> -expr \n"); 	 
	
			}
	| NOT expr	{ 
				expr* result = NULL, * arg1 = $2;
				
				result = newexpr(boolexpr_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			
				//result->truelist  = arg1->falselist;
				//result->falselist = arg1->truelist;

				emit(not, arg1, NULL, result, nextquadlabel(), yylineno);

				//emit(if_eq, arg1, newexpr_constbool(1), NULL, nextquadlabel()+3, yylineno);
				
	  			//emit(assign, newexpr_constbool(1), NULL, result,-1,yylineno);
	 			//emit(jump,NULL,NULL,NULL, nextquadlabel()+2, yylineno);
	  			//emit(assign, newexpr_constbool(0), NULL, result, -1,yylineno);	
							
				$$ = result;
				fprintf(stdout, "Rule used: term -> NOT expr \n");

 		}
	| PLUS_PLUS lvalue 			{ 
				int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
				bool varExists = false;
				expr *result, * arg1 = $2;
				
				/* Look up if the IDENTIFIER is shadowing library functions. */
				isShadowed =  isShadowingLibFunc(arg1->sym->name);

				if (isShadowed){
					fprintf(stdout,"SYNTAX ERROR<#%d>:: Cannot use operator ++ to library function\n", yylineno);	
					error = true;
				}else{
					varExists = lookup_in_all_scope(mySymbolTable, arg1->sym->name);
					if (varExists){
						tempScope = scope;
						while (tempScope >= 0){
							bool userFuncexist = false;
							SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
							while(firstSymbol != NULL){
								if(firstSymbol->name == arg1->sym->name){
									if (firstSymbol->type == USERFUNC){
										userFuncexist = true;
										fprintf(stdout, "SYNTAX ERROR<#%d>:: Cannot use operator ++ to user function\n", yylineno);
										error = true;
										break;
									}
								}
								firstSymbol = firstSymbol->nextinScope;
							}
							tempScope--;
						}
					
					}
				}
				
				
				if (arg1->type == tableitem_e){
					result = (expr*) emit_iftableitem (arg1);

					emit(add,result, newexpr_constnum(1), result, -1, yylineno);
					emit(tablesetelem, arg1, arg1->index, result,-1, yylineno);
				
				}else{
					
					emit(add,arg1,  newexpr_constnum(1), arg1, -1, yylineno);
					result = newexpr(arithexpr_e);	
					
					result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
					emit(assign, arg1, NULL, result, -1, yylineno);
				}
				$$ = result;
				fprintf(stdout, "Rule used: term -> ++lvalue \n"); 

	}
	| lvalue PLUS_PLUS				{ 
				int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
				bool varExists = false;
				expr *result, * value, * arg1 = $1;
				
				/* Look up if the IDENTIFIER is shadowing library functions. */
				/* id -> lvalue has been upgrade to an expression. So we need to check if the symbol in expression is shadowing some function. */
				isShadowed =  isShadowingLibFunc(arg1->sym->name);
				
				if (isShadowed){
					fprintf(stdout,"SYNTAX ERROR<#%d>:: Cannot use operator ++ to library function\n",yylineno);	
					error = true;
				}else{
					varExists = lookup_in_all_scope(mySymbolTable, arg1->sym->name);
					if (varExists){
						tempScope = scope;
						while (tempScope >= 0){
							bool userFuncexist = false;
							SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
							while(firstSymbol != NULL){
								if(firstSymbol->name == arg1->sym->name){
									if (firstSymbol->type == USERFUNC){
										userFuncexist = true;
										fprintf(stdout, "SYNTAX ERROR<#%d>:: Cannot use operator ++ to user function\n", yylineno);
										error = true;
										break;
									}
								}
								firstSymbol = firstSymbol->nextinScope;
							}
							tempScope--;
						}
					
					}
				}
			
		result = newexpr(var_e);
		result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
		
		if (arg1->type == tableitem_e){
			//printf("arg1->type == tableitem_e\n");
			value =  (expr*)emit_iftableitem (arg1);
			emit(assign, value, NULL, result, -1, yylineno);
			emit(add,value, newexpr_constnum(1), value, -1, yylineno);
			emit(tablesetelem, arg1, arg1->index, value,-1, yylineno);
		
		}else{
			//printf("arg1->type != tableitem_e\n");
			emit(assign, arg1, NULL, result, -1, yylineno);
			emit(add,arg1, newexpr_constnum(1), arg1, -1, yylineno);
		}
		
		$$ = result;
		
		fprintf(stdout, "Rule used: term -> lvalue++ \n"); 
		
	}
	| MINUS_MINUS lvalue			{ 

				int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
				bool varExists = false;
				expr *result, * arg1 = $2;
				
				/* Look up if the IDENTIFIER is shadowing library functions. */
				isShadowed =  isShadowingLibFunc(arg1->sym->name);

				if (isShadowed){
					fprintf(stdout,"SYNTAX ERROR<#%d>:: Cannot use operator ++ to library function\n", yylineno);	
					error = true;
				}else{
					varExists = lookup_in_all_scope(mySymbolTable, arg1->sym->name);
					if (varExists){
						tempScope = scope;
						while (tempScope >= 0){
							bool userFuncexist = false;
							SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
							while(firstSymbol != NULL){
								if(firstSymbol->name == arg1->sym->name){
									if (firstSymbol->type == USERFUNC){
										userFuncexist = true;
										fprintf(stdout, "SYNTAX ERROR<#%d>:: Cannot use operator ++ to user function\n", yylineno);
										error = true;
										break;
									}
								}
								firstSymbol = firstSymbol->nextinScope;
							}
							tempScope--;
						}
					
					}
				}
				
				
				
				if (arg1->type == tableitem_e){
					
					result = (expr *)emit_iftableitem (arg1);
					emit(sub,result, newexpr_constnum(1), result, -1, yylineno);
					emit(tablesetelem, arg1, arg1->index, result,-1, yylineno);
				
				}else{
					emit(sub,arg1, newexpr_constnum(1), arg1, -1, yylineno);
					result = newexpr(arithexpr_e);	
					
					result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
					emit(assign, arg1, NULL, result, -1, yylineno);
				}
				
				$$ = result;
					fprintf(stdout, "Rule used: term -> --lvalue \n"); 
		}
	| lvalue MINUS_MINUS			{ 
	
				int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
				bool varExists = false;
				expr *result, * value, * arg1 = $1;
				
				/* Look up if the IDENTIFIER is shadowing library functions. */
			
				
				isShadowed =  isShadowingLibFunc(arg1->sym->name);
				
				if (isShadowed){
					fprintf(stdout,"ERROR<#%d>:: Cannot use operator ++ to library function\n", yylineno);
					error = true;
						
				}else{
					varExists = lookup_in_all_scope(mySymbolTable, arg1->sym->name);
					if (varExists){
						tempScope = scope;
						while (tempScope >= 0){
							bool userFuncexist = false;
							SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
							while(firstSymbol != NULL){
								if(firstSymbol->name == arg1->sym->name){
									if (firstSymbol->type == USERFUNC){
										userFuncexist = true;
										fprintf(stdout, "ERROR<#%d>:: Cannot use operator ++ to user function\n", yylineno);
										error = true;
										break;
									}
								}
								firstSymbol = firstSymbol->nextinScope;
							}
							tempScope--;
						}
					
					}
				}
				
				result = newexpr(var_e);
				result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
				
				
				if (arg1->type == tableitem_e){
					
					value =  (expr*)emit_iftableitem (arg1);

					emit(assign, value, NULL, result, -1, yylineno);
					emit(sub,value, newexpr_constnum(1), value, -1, yylineno);
					emit(tablesetelem, arg1, arg1->index, value, -1 , yylineno);
				
				}else{
					emit(assign, arg1, NULL, result,-1, yylineno);
					emit(sub,arg1, newexpr_constnum(1), arg1, -1, yylineno);
				}
				
				$$ = result;
				fprintf(stdout, "Rule used: term -> lvalue-- \n"); 
		}		
	| primary	{ expr* temp = $1; $$ = temp; fprintf(stdout, "Rule used: term -> primary \n"); }	
	;


assignexpr 
	: lvalue '=' expr 			{ 
		/* We cannot assign in lib function and user function -- throw error*/
		int  isShadowed = 0;
		bool varExists = false;
		expr* lvalueExpr = $1;
		expr * eExpr =  $3;
		expr * result;
		
		/* Look up if the IDENTIFIER is shadowing library functions. */
		isShadowed =  isShadowingLibFunc(lvalueExpr->sym->name);
		
		if (isShadowed){
			fprintf(stdout,"SYNTAX ERROR<#%d>:: Cannot assign to library function", yylineno);
			error = true;
			
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, lvalueExpr->sym->name);
			if (varExists){
				int tempScope = scope;
				bool userFuncexist = false;
				while (tempScope >= 0){
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == lvalueExpr->sym->name){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "SYNTAX ERROR<#%d>:: Cannot assign to user function\n", yylineno);
								error = true;
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			}else{}
		}
		
		if (lvalueExpr->type == tableitem_e){
			//emit(tablesetelem,lvalueExpr->index,lvalueExpr,eExpr,-1, yylineno); 
			emit(tablesetelem,lvalueExpr->index,eExpr,lvalueExpr,-1, yylineno); 
			result = (expr *)emit_iftableitem(lvalueExpr);
			result->type = assignexpr_e;
		}else{
			emit(assign,eExpr,NULL, lvalueExpr,-1, yylineno);
			
			result = newexpr(assignexpr_e);
			result->sym  = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			emit(assign, lvalueExpr, NULL, result, -1, yylineno);		
		}


		$$ = result;
		fprintf(stdout, "Rule used: assignexpr -> lvalue = expr; \n"); 
	}		
	;
	
primary 
	: lvalue	{ $$ = (expr *)emit_iftableitem($1); fprintf(stdout, "Rule used: primary -> lvalue \n"); }
	| call		{ expr * temp = $1; $$ = temp; fprintf(stdout, "Rule used: primary -> call \n"); }
	| objectdef	{ expr * temp = $1; temp->type = newtable_e; $$ = temp; fprintf(stdout, "Rule used: primary -> objectdef \n"); }
	|'(' funcdef ')'	{ 
						expr* result = newexpr(programfunc_e);
						result->sym = $2;
						$$ = result;
						fprintf(stdout, "Rule used: primary -> (funcdef) \n"); 
						}
	| const		{ expr * temp = $1; $$ = temp; fprintf(stdout, "Rule used: primary -> const \n"); }					
	;

lvalue 
	: IDENTIFIER	{ 
		
		expr * idExpr;
		bool varExists;
		SymbolTableEntry_t* newEntry, * existVar;
		bool existsInSameScope = false;
		bool existsDiff = false;
		SymbolTableEntry_t *var;
		int remainingLibFun = 0, isShadowed = 0, i = 0;
		SymbolTableEntry_t* symbolExists = NULL;
		int tempScope = 0;
		
		/* Koitame apo to trexon scope mehri to global na doume an uparxei ston symbol table symbolo */
		for (tempScope = scope; tempScope >= 0; tempScope--){
				symbolExists = lookupSpecificScope (mySymbolTable, $1, tempScope);
				if (symbolExists != NULL) break;  // ama broume kati koitame an exoume prosbash se ayto
		}
			
		/* An den brethei tipota se kanena scope, tote prosthetoume nea metablhth sto trexon scope*/
		if (symbolExists == NULL){
			fprintf(stdout, "New entry: <%d>Not in any scope %s\n",yylineno, $1);
			if (scope == 0){
				newEntry = createSymbolTableEntry ($1, yylineno, scope, GLOBALVAR, currscopespace(),currscopeoffset());
			}else{
				newEntry = createSymbolTableEntry ($1, yylineno, scope, LOCALVAR,currscopespace(), currscopeoffset());
			}			
			var = newEntry;
			insertSymbol (mySymbolTable, newEntry);
			incurrscopeoffset();
		}else {
			/* An brethei se kapoio scope kati anaferomaste ekei an exoume prosvash */
			
			if (tempScope <= scope){
				
					/* Einai prosbasimes oles oi orates sunarthseis , toso sto trexwn oso kai se e3wterikotera scope */	
					if (symbolExists->type == USERFUNC && symbolExists->isActive == 1){
						fprintf (stdout, "ok, refers to active function in scope: %d\n", tempScope);
						var = symbolExists;
					}else if (symbolExists->scope == 0){
						/* Einai prosvasimes oles oi katholikes metablhtes (se scope 0) kai oi synarthseis bibliothikis */
						if (symbolExists->type == GLOBALVAR && symbolExists->isActive == 1){
							fprintf (stdout, "ok, refers to global variable\n");
						}else if (symbolExists->type == LIBFUNC && symbolExists->isActive == 1 ){
							fprintf (stdout, "ok, refers to library function\n");
						}
						var = symbolExists;
					}else{
			
							if (functionScope > symbolExists->scope && intoFunction > 0){
								printf("SYNTAX ERROR <#%d>:: functionScope > symbolExists for %s\n", yylineno, $1);
								exit(0);
							}else{
								if (symbolExists->type == FORMAL && symbolExists->isActive == 1){	
									fprintf (stdout, "ok, refers to formal arg in scope: %d\n", tempScope);
									var = symbolExists;
								}	
								
								if (symbolExists->type == LOCALVAR && symbolExists->isActive == 1){	
									fprintf (stdout, "ok, refers to local var in scope: %d\n", tempScope);
									var = symbolExists;
								}
							}	
					}
					

				
			}else{
					fprintf (stdout, "SYNTAX ERROR <#%d> Cannot access the symbol \n", yylineno);
					exit(0);
			}
			

		}
		if (var != NULL){
			printf("ID: %s , symbol table entry: %s", $1, var->name);
		}else{
			fprintf(stdout, "VAR IS NULL!"); 
		}
		isTableItem = false;	

		$$ = lvalue_expr(var);
		fprintf(stdout, "Rule used: lvalue  -> IDENTIFIER\n"); 
	}

	| LOCAL IDENTIFIER { 
		
		bool varExists = false;
		int remainingLibFun = 0, isShadowed = 0;
		SymbolTableEntry_t* newEntry, *existsInScope;
		expr * localExpr;
		
		isShadowed =  isShadowingLibFunc($2);
		
		/*CASE that variable is shadowing lib function */	
		if (isShadowed && scope != 0){
			fprintf (stdout, "ERROR<#%d>:: Variable %s is shadowing library function.\nExiting.\n", yylineno,$2);
			exit (0);
		}else if(isShadowed && scope == 0){
			fprintf (stdout, "ok:: %s refers to library function\n", $2);
		}else{
			existsInScope = lookupSpecificScope (mySymbolTable, $2, scope);
			if (existsInScope != NULL){
				fprintf(stdout,"ok:: Variable %s refers to a local variable in same scope\n", $2);
				localExpr = lvalue_expr(existsInScope);
			
			}else{
				if (scope == 0){
					newEntry = createSymbolTableEntry ($2, yylineno, scope, GLOBALVAR,currscopespace(),currscopeoffset());
					newEntry->space = currscopespace();
				}else{
					newEntry = createSymbolTableEntry ($2, yylineno, scope, LOCALVAR,currscopespace(), currscopeoffset());
					newEntry->space = currscopespace();
				}
				
				incurrscopeoffset();
				insertSymbol (mySymbolTable, newEntry);
				localExpr = lvalue_expr(newEntry);
			}
			
		}
		isTableItem = false;
		$$ = localExpr;	
		fprintf(stdout, "Rule used: lvalue  -> LOCAL IDENTIFIER\n"); 
	
	}
	| DOUBLE_COLON IDENTIFIER		{ 

		expr * globalExpr;
		bool varExists = false;
		varExists = lookup_specific_scope (mySymbolTable, $2, 0);
		SymbolTableEntry_t* symbol = lookupSpecificScope(mySymbolTable, $2, 0);
		if (varExists){
			fprintf(stdout, "Variable %s refers to global variable\n",$2);
		}else{
			fprintf(stdout, "ERROR<#%d>:: Variable %s doesn't exist in global scope.\nCompilation failed.\n",yylineno,$2);
			exit(0);
		}
	
		globalExpr = lvalue_expr(symbol);
		isTableItem = false;
		$$=globalExpr;
		fprintf(stdout, "Rule used: lvalue  -> DOUBLE_COLON IDENTIFIER\n"); 

	}
	| member						{ 

		expr* temp = $1;
		$$ = temp;
		isTableItem = true;
		fprintf(stdout, "Rule used: lvalue  -> member\n"); 
	}
	;
	
member 
	: lvalue '.' IDENTIFIER			{ 
	
		expr *lvalue = $1;
		char *name = $3;
		int isShadowed = 0;
		
		isShadowed =  isShadowingLibFunc(name);
		if (isShadowed){
			fprintf(stdout,"ERROR<#%d>:: Cannot use library function as lvalue.ID", yylineno);
			error = true;
		}
			
	
		$$ = member_item(lvalue,name);
		
		fprintf(stdout, "Rule used: member -> lvalue.IDENTIFIER\n"); 


	}
	| lvalue '[' expr ']'			{ 

		expr * lvalue = $1;
		expr * expression = $3;
		expr * tableitem;
		int remainingLibFun = 0, isShadowed = 0;
		
		lvalue = (expr *)emit_iftableitem(lvalue);
		
		tableitem = newexpr(tableitem_e);
		tableitem->sym = lvalue->sym;
		tableitem->index = expression;
	
		$$ = tableitem;
		
		fprintf(stdout, "Rule used: member -> lvalue [expr]\n"); 
	}
	| call'.' IDENTIFIER			{ 
		
			expr *call_ = $1;
			char *name = $3;
			int isShadowed = 0;
			
			isShadowed =  isShadowingLibFunc(name);
			if (isShadowed){
				fprintf(stdout,"ERROR<#%d>:: Cannot use library function as lvalue.ID", yylineno);
				error = true;
			}
			
	
			$$ = member_item(call_,name);
			
			fprintf(stdout, "Rule used: member -> call.IDENTIFIER\n"); 
			}
	| call '[' expr ']'				{ 
		expr * call_ = $1;
		expr * expression = $3;
		expr * tableitem;
		int remainingLibFun = 0, isShadowed = 0;
		
		call_ = (expr *)emit_iftableitem(call_);
		
		tableitem = newexpr(tableitem_e);
		tableitem->sym = call_->sym;
		tableitem->index = expression;
	
		$$ = tableitem;
		
		fprintf(stdout, "Rule used: member -> call [expr] \n"); }
	;	

call 
	: call '(' elist ')'									{ 
		
			expr * func, * result, *arg1 = $1 , *temp = $3;
			expr * pars[100];
			int i;
			
			
			func = (expr *)emit_iftableitem(arg1);
				
			
			for (i = 0; temp != NULL; i++ ){
				pars[i] = temp;
				temp = temp->next;
			}
			/* We have to emit param from last to first */
			for (i--; i>=0; i--){
				emit(param,NULL, NULL, pars[i],-1,yylineno);
			}
			
			emit(call, NULL, NULL, func, -1, yylineno);
			
			result = newexpr(var_e);
			result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			emit(getretval, NULL, NULL, result, -1, yylineno);
			
			$$ = result;
			fprintf(stdout, "Rule used: call  -> call (elist)  \n"); 
			}
	| lvalue DOUBLE_DOT IDENTIFIER '(' elist ')' 			{ 
		
			
			expr * pars[100];
			expr * self = $1;
			expr * lvalue;
			expr * temp = $5;
			expr * func;
			int i = 0;
			
			lvalue = emit_iftableitem(member_item(self, $3));
			
			/* add the lvalue in param list */
			self->next = temp;
			temp =  self;
			
			func = emit_iftableitem(lvalue);
			
			for (i = 0; temp != NULL; i++){
				pars[i] = temp;
				temp=temp->next;
			}
	
			for (i--; i>=0; i--){
				emit(param,NULL,NULL, pars[i],-1,yylineno);
			}
		
			emit(call, NULL, NULL,func, -1, yylineno);
			
			expr *result = newexpr(var_e);
			result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			emit(getretval, NULL, NULL, result, -1, yylineno);

			$$ = result;
			fprintf(stdout, "Rule used: call  -> lvalue ..id(elist) \n"); }
	| lvalue '(' elist ')'									{ 
			expr * func, * result;
			expr * temp = malloc (sizeof(expr));
			temp = $3;
			
			expr * pars[100];
			expr * arg1 = $1;
			int i = 0;
			
				
			func = (expr *)emit_iftableitem(arg1);	
			
			for (i = 0; temp != NULL; i++){	
				pars[i] = temp;
				temp = temp->next;				
			}
			
			for (i--; i>=0; i--){
				emit(param,NULL,NULL, pars[i],-1,yylineno);
			}
		
			
			emit(call, NULL, NULL, func, -1, yylineno);
			
			result = newexpr(var_e);
			result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			emit(getretval, NULL, NULL, result, -1, yylineno);
			$$ = result;
			
			
			fprintf(stdout, "Rule used: call  -> lvalue (elist) \n"); 
			}
	| '(' funcdef ')' '(' elist ')'							{ 
			expr *func,* result, * f;
			expr * temp = $5;
			expr * pars[100];

			int i = 0;
			
			func = newexpr(programfunc_e);
			func->sym = $2;
			
			f = (expr *) emit_iftableitem(func);
					
			
			for (i = 0; temp != NULL; i++ ){
				pars[i] = temp;
				temp = temp->next;
			}
			
			for (i--; i>=0; i--){
				emit(param,NULL, NULL, pars[i],-1,yylineno);
			}
			
			emit(call, NULL, NULL, f, -1, yylineno);
			
			result = newexpr(var_e);
			result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
			emit(getretval, NULL, NULL, result, -1, yylineno);
			$$ = result;
		
			fprintf(stdout, "Rule used: call  -> '(' funcdef ')' (elist)  \n"); }
	;


elist 
	: expr listed_expr	{ 

					expr * temp = $1;
					temp->next = $2;
					$$ = temp;
					
					fprintf(stdout, "Rule used: elist -> expr listed_expr\n"); 
				}
	| expr			{ 

					expr * temp = $1;
					temp->next = NULL;
					$$ = temp;

					fprintf(stdout, "Rule used: elist -> expr\n"); 
				}
	|			{	$$ = NULL;}
	;	
	
listed_expr
	: ',' expr listed_expr	{ 
					
					expr * temp = $2;
					temp->next = $3;
					 $$ = temp;
					fprintf(stdout, "Rule used: elist -> ',' expr listed_eexpr \n"); 

					}
	| ',' expr			{ 
	
					expr * temp = $2;
					temp->next = NULL;
					$$ = temp;
					
					fprintf(stdout, "Rule used: elist -> ',' expr\n"); 
					}
	;

eelist 
: expr 			{ 
								
					expr * temp = $1;
					temp->next = NULL;
					$$ = temp;
					
					fprintf(stdout, "Rule used: elist -> expr\n"); 
			}
	| expr listed_eexpr 		{ 
					
					expr * temp = $1;
					$$ = temp;
					temp->next = $2;
					
					fprintf(stdout, "Rule used: elist -> listed_expr\n"); 
					}
	;	
	
listed_eexpr
	: ',' expr 			{ 
					
					
					expr * temp = $2;
					temp->next = NULL;
					$$ = temp;
					
					fprintf(stdout, "Rule used: elist -> ,expr\n"); }
	| ',' expr listed_eexpr		{

						
						expr * temp = $2;
						$$ = temp;
						temp->next = $3;
					
						
						fprintf(stdout, "Rule used: elist -> ,expr listed_eexpr\n");
					}						
	;

	
objectdef 
	: '[' ']' 			{ 
							expr * table = newexpr(newtable_e);
							table->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
							emit(tablecreate, NULL, NULL, table, -1, yylineno);
							$$ = table;
							fprintf(stdout, "Rule used: objectdef -> [] \n"); 
					}
	| '[' eelist ']' 				{ 
							expr * table = NULL, *tmp = NULL;
							int i = 0;
							
							table = newexpr(newtable_e);
							table->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
						   
							emit(tablecreate, NULL, NULL, table, -1, yylineno);
							tmp = $2;
							
							for (i = 0; tmp != NULL; i++){
								emit(tablesetelem,newexpr_constnum(i) , tmp,table , -1, yylineno);
								tmp = tmp->next;
							}
							
							$$ = table;
							fprintf(stdout, "Rule used: objectdef -> [elist] \n"); 
		}
	| '[' indexed ']'				{ 
							expr * table = NULL,  *tmp = NULL;
							int i = 0;
							
							table = newexpr(newtable_e);
							table->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
						   
							emit(tablecreate, NULL, NULL, table, -1, yylineno);
							tmp = $2;
							for (i = 0; tmp != NULL; i++){	
									emit(tablesetelem,tmp, tmp->index,table,-1,yylineno);
									tmp = tmp->next;
							}
							
							$$ = table;

							fprintf(stdout, "Rule used: objectdef -> [indexed] \n"); 
		}
	;
	
	

indexed 
	: indexedelem indexed_expr 		{ 

							
							expr * temp = $1;
							$$ = temp;
							temp->next = $2;
							
							fprintf(stdout, "Rule used: indexed -> indexedelem\n"); 
						}
	| indexedelem				{ 

							expr * temp = $1;
							$$ = temp;
							temp->next = NULL;
							fprintf(stdout, "Rule used: indexed -> indexed indexed_expr \n"); 
						}
	; 
	
indexed_expr
	: ',' indexedelem indexed_expr  	{ 
														
							expr * temp = $2;
							$$ = temp;
							temp->next = $3;
					
					
							fprintf(stdout, "Rule used: indexed -> ,indexedelem indexed_expr  \n"); 
						}
	| ',' indexedelem			{ 
							
							expr * temp = $2;
							$$ = temp;
							temp->next = NULL;
							fprintf(stdout, "Rule used: indexed -> ,indexedelem  \n"); 
						}
	;
	
indexedelem 
	: '{' expr ':' expr '}'			{ 

							expr * temp = $2;
							temp->index = $4;
							$$ = temp;
							fprintf(stdout, "Rule used: indexedelem -> '{' expr ':' expr '}' \n");
						}
	;	
	


block 
	: '{' {++scope; if (maxScope < scope) {maxScope = scope;}} help_stmt '}' {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{' stmt '}' \n"); 
	}
	| '{' {++scope; if (maxScope < scope) {maxScope = scope;}} '}' {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{'  '}' \n"); 
	}
	;


funcname
	: IDENTIFIER	{ $$ = Manage_funcname($1); fprintf(stdout, "Rule used: funcname -> IDENTIFIER \n"); }				
	|				{ $$ = Manage_funcname(nameTempFunction(numberOfUnnamed++));	fprintf(stdout, "Rule used: funcname -> empty \n"); }
	;


funcprefix
	: FUNCTION funcname		{
						int isShadowed = 0;
						SymbolTableEntry_t* newEntry = NULL;
						int prevSpace, currSpace; 
						
						if (scope == 0) {
							++functionScope;
						}else{
							functionScope = scope + 1;
						}		

						/* If is shadowed print error and continue the syntax analysis. */
						isShadowed =  isShadowingLibFunc($2);

						/* If is shadowed print error and continue the syntax analysis. */
						if (isShadowed == 1){
							fprintf(stdout, "ERROR<#%d>:: User function %s tries to shadow library function.\n", yylineno, $2);
							exit (0);
						}else{
							bool functionExists = lookup_specific_scope (mySymbolTable, $2, scope);
							if (functionExists){
								fprintf(stdout, "ERROR<#%d>:: User function name exists in scope.\n", yylineno);
								exit (0);
							} else {
					
								newEntry = createSymbolTableEntry ($2, yylineno, scope, USERFUNC, currscopespace(),currscopeoffset());
								newEntry->iaddress = nextquadlabel();
								emit(funcstart, NULL, NULL, lvalue_expr(newEntry) , -1, yylineno);
								insertSymbol(mySymbolTable, newEntry);
							}
						}

						localArgStack = push(localArgStack, currscopeoffset());
						enterscopespace();
						resetformalargsoffset(); // Start formals from zero
						
						$$ = newEntry;
						fprintf(stdout, "Rule used: funcprefix -> FUNCTION funcname \n");
					}
	;

funcargs
	: '(' idlist ')'		{
						
						
						struct argNames* temp = list;
						bool formalExists = false;
						int  isShadowed = 0;
						SymbolTableEntry_t * argSymbol;
						
						
	
						while (temp != NULL){
							/* Look up if the formal is shadowing library functions. */
							isShadowed =  isShadowingLibFunc(temp->name);
							if (isShadowed == 1){
								fprintf(stdout, "ERROR: lif func is shadowed by variable %s:\n", list->name);
								exit (0);
							}else{
									/*look if a formal exists in symbol table for this scope */
									formalExists = lookup_specific_scope (mySymbolTable, temp->name, temp->scope);
							
									if (formalExists == true){
										fprintf(stdout, "ERROR: Formal name exists in scope.\n");
										exit (0);
									}else{
										//printf("ARG: %s, SPACE: %d, OFFSET: %d\n", temp->name, currscopespace() ,currscopeoffset() );
										argSymbol  = createSymbolTableEntry (temp->name, temp->yyline, temp->scope, FORMAL,  currscopespace() ,currscopeoffset());
										insertSymbol (mySymbolTable, argSymbol);
										incurrscopeoffset();
									}
							}
							temp = temp->next;
						}
						deleteList(&list);
						enterscopespace();
						resetfunctionlocalsoffset(); //Need to start locals from zero 
						intoFunction++;
						fprintf(stdout, "Rule used: funcargs-> (idlist) \n");
					}
	;


funcbody
	: block 				{	
						exitscopespace();
						fprintf(stdout, "Rule used: funcbody-> block \n");
					}
	;


funcdef
	: funcprefix funcargs funcbody {

	SymbolTableEntry_t * entry = $1;
	int curr;
		
	
	functionScope--; 
	intoFunction--; 

	exitscopespace();
	entry->totallocals = currscopeoffset();
	curr = currscopeoffset();
	localArgStack = pop(localArgStack,&(curr));
	
	emit(funcend, NULL, NULL, lvalue_expr(entry), -1, yylineno); 
	
	$$ = entry;
}
;
	
	
const 
	: INTEGER { 
			int temp = $1;								/* The integer lex recognized*/
			expr* intExpr = newexpr(constnum_e); 		/* Create new expression with type constnum_e*/
			intExpr->numConst = temp;					/* Store the integer in the expr*/
			$$ = intExpr; 								/* Return expr as const */
			fprintf(stdout, "Rule used: const -> INTEGER \n");
	}

	| FLOAT   { 
			double temp = $1;							/* The float lex recognized*/	
			expr* floatExpr = newexpr(constnum_e); 		/* Create new expression with type constnum_e*/
			floatExpr->numConst = temp;					/* Store the float in the expr*/
			$$ = floatExpr;								/* Return expr as const */
			fprintf(stdout, "Rule used: const -> FLOAT \n");
	}
	| STRING_DEF { 
			expr* stringExpr = newexpr(conststring_e);	/* Create new expression with type conststring_e*/
			stringExpr->strConst = strdup($1);			/* Store the string in the expr*/
			$$ = stringExpr;							/* Return expr as const */
			fprintf(stdout, "Rule used: const -> STRING_DEF \n");
	}

	| NIL   { 
			expr* nilExpr = newexpr(nil_e);				/* Create new expression with type nil_e*/	
			$$ = nilExpr;								/* Return expr as const */
			fprintf(stdout, "Rule used: const -> NIL\n");
}
	| TRUE { 
			expr* trueExpr = newexpr(constbool_e);		/* Create new expression with type constbool_e*/
			trueExpr->boolConst = 1;					/* TRUE == 1 */
			
			/* Create truelist and emit a jump in quads */
			makeList(&(trueExpr->truelist), nextquadlabel());
			
			//emit(jump, NULL, NULL, NULL, -1, yylineno);
			$$ = trueExpr;
			fprintf(stdout, "Rule used: const -> TRUE\n");
	}
	| FALSE	{ 

			expr* falseExpr = newexpr(constbool_e);		/* Create new expression with type constbool_e*/
			falseExpr->boolConst = 0;					/* FALSE == 0 */
			

			/* Create falselist and emit a jump in quads */
			makeList(&(falseExpr->falselist), nextquadlabel());
			
			//emit(jump, NULL, NULL, NULL, -1, yylineno);
			$$ = falseExpr;
			fprintf(stdout, "Rule used: const -> FALSE\n");			
													
	}	
	;


idlist 
	: IDENTIFIER { 
	/* Single arg:: Save argument name, insert it into a list and print the */
		argumentName = malloc((strlen($1) + 1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(&list, argumentName, yylineno, scope+1);
		
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER \n"); 
	}
	| IDENTIFIER  helpidlist  		{ 
		
		argumentName = malloc((strlen($1) + 1) * sizeof(char));
		strcpy(argumentName, $1);
		insert(&list, argumentName, yylineno, scope+1);
		
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER  helpidlist \n"); 
	}
	| 	{ fprintf(stdout, "Rule used: idlist -> e\n"); }
	;
	
helpidlist 
	: ',' IDENTIFIER helpidlist 	{ 
		argumentName = malloc(strlen($2) * sizeof(char));
		strcpy(argumentName, $2);
		insert(&list, argumentName, yylineno, scope+1);
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER helpidlist\n"); }
	| ',' IDENTIFIER				{ 
		argumentName = malloc(strlen($2) * sizeof(char));
		strcpy(argumentName, $2);
		insert(&list, argumentName, yylineno, scope+1);
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER	\n"); 
		}
	;	


ifprefix
	: IF '(' expr ')'	{
					expr* expression = $3;
					int temp;

					emit(if_eq, expression,newexpr_constbool(1),NULL, nextquadlabel()+2 ,yylineno);
					temp = nextquadlabel();
					emit(jump, NULL, NULL, NULL, -1, yylineno);

					$$ = temp;

					fprintf(stdout, "Rule used: ifprefix -> IF \'(\' expr \')\' \n");
				}
	;

elseprefix
	: ELSE			{
					int temp = nextquadlabel();
					emit(jump, NULL, NULL, NULL, -1, yylineno);
					$$ = temp;
					fprintf(stdout, "Rule used: elseprefix -> ELSE \n");	
				}
	;


ifstmt 
	:  ifprefix stmt       {
					int patched = $1;
					patchlabel(patched,nextquadlabel());
					fprintf(stdout, "Rule used: ifstmt -> ifprefix stmt \n");
				}		
	|  ifprefix stmt elseprefix stmt {
					int ifpr = $1;
					int elsepr = $3;
					patchlabel(ifpr,elsepr + 1);
					patchlabel(elsepr,nextquadlabel());
					fprintf(stdout, "Rule used: ifstmt -> ifprefix stmt elseprefix stmt \n");
				}
	;
	
loopstart
	:		{ 
				whileLoop++;
				fprintf(stdout, "Rule used: loopstart -> e\n");
			}
	;
	

loopend
	:		{ 
				whileLoop--;
				fprintf(stdout, "Rule used: loopend -> e\n");
 			}
	;

loopstmt
	:loopstart stmt loopend  
			{
				expr *result;
				result = $2;
				$$ = result;
				fprintf(stdout, "Rule used: loopstmt -> loopstart stmt loopend\n");
			}
	;

whilestart
	: WHILE  {
			$$ = nextquadlabel();
			fprintf(stdout, "Rule used: whilestart -> WHILE\n");				
			
		}
	;



whilecond
	: '(' expr ')' {	
				int temp;
				expr * expression = $2;
				emit(if_eq, expression,newexpr_constbool(1),NULL,nextquadlabel()+2,yylineno);
				temp = nextquadlabel();
				emit(jump, NULL, NULL, NULL, -1, yylineno);
				$$ = temp;
				fprintf(stdout, "Rule used: whilestart -> WHILE\n");			
			}

	;


whilestmt
	: whilestart whilecond loopstmt{
		looplist *tempContList = continuelist;
		looplist *tempBreakList = breaklist;
		int tempPatch;
		int whilestart = $1;
		int whilecond = $2;
		
		
		emit(jump, NULL, NULL, NULL, whilestart, yylineno);
		patchlabel(whilecond,nextquadlabel());
		
		while(tempBreakList != NULL){
			if (tempBreakList->quadLabel >=0){
				tempPatch = tempBreakList->quadLabel;
				patchlabel(tempPatch,nextquadlabel());
				
			}
			tempBreakList = tempBreakList->next;
		}
		deletelooplist(&breaklist);

		while(tempContList != NULL){

			if (tempContList->quadLabel >=0){
				tempPatch = tempContList->quadLabel;
				patchlabel(tempPatch,whilestart);
			}
			tempContList = tempContList->next;
		}
		deletelooplist(&continuelist);


		continuelist = NULL;
		breaklist = NULL;

									
		
		fprintf(stdout, "Rule used: whilestmt -> whilestart whilecond stmt\n");
	}
	;




for_N
	:			{
				
				$$ = nextquadlabel();
				//printf("FOR_N: %d\n", nextquadlabel());
				emit(jump, NULL, NULL, NULL, -1, yylineno);
				fprintf(stdout, "Rule used: for_N -> e\n");
				}
	;
for_M
	:			{	
				int temp = nextquadlabel();
				$$ = temp;
				//printf("FOR_M: %d\n", nextquadlabel());
				fprintf(stdout, "Rule used: for_M -> e\n");
				}
	;


forprefix
	: FOR '(' elist ';' for_M expr ';'
				{
					struct forStruct * test = malloc(sizeof(struct forStruct));
					int enter_ = nextquadlabel();
					int test_ = $5;
					
					emit(if_eq, $6, newexpr_constbool(1),NULL, -1, yylineno);
					
					test->enter = enter_;
					test->test = test_;
					//printf("enter: %d test: %d\n", enter, test_);
					$$ = test;
					fprintf(stdout, "Rule used: forprefix -> FOR (elist; for_M expr;  \n");
				}
	;
	


forstmt
	: forprefix for_N elist ')' for_N loopstmt for_N { 
					int N1, N2, N3;
					int tempPatch;
					enter_test * t = $1;
					int enter = t->enter;
					int p_M = t->test;
					looplist *tempContList = continuelist;
					looplist *tempBreakList = breaklist;
	
					N1 = $2;
					N2 = $5;
					N3 = $7;
					printf("p_M: %d, N1: %d, N2: %d, N3: %d\n", p_M, N1,N2, N3);
					printf("##1 patch: %d with: %d \n", enter, N2+1);
					patchlabel(enter, N2 + 1);//wrong $1
					printf("##2 patch: %d with: %d \n", N1, nextquadlabel());
					patchlabel(N1, nextquadlabel());
					printf("##3 patch: %d with: %d \n", N2, p_M);
					patchlabel(N2, p_M);//wrong $1
					printf("##4 patch: %d with: %d \n", N3, N1 + 1);
					patchlabel(N3, N1 + 1);
					 
					while(tempBreakList != NULL){
						if (tempBreakList->quadLabel >=0){
							tempPatch = tempBreakList->quadLabel;
							patchlabel(tempPatch,nextquadlabel());
							printf("##breaklist patch: %d with: %d \n", tempPatch, nextquadlabel());
						}
						tempBreakList = tempBreakList->next;
					}
					deletelooplist(&breaklist);

					while(tempContList != NULL){

						if (tempContList->quadLabel >=0){
							tempPatch = tempContList->quadLabel;
							patchlabel(tempPatch,N1 + 1);
							printf("##contlist patch: %d with: %d \n", tempPatch,N1 + 1);
						}
						tempContList = tempContList->next;
					}
					deletelooplist(&continuelist);


					continuelist = NULL;
					breaklist = NULL;
					fprintf(stdout, "Rule used: forstmt -> FOR \n"); 


	}
	;	
	


returnstmt
	: RETURN ';' 		{ Manage_return(intoFunction); }
	| RETURN expr ';'	{ Manage_return_expr_semicolon(intoFunction, $2); }
	;
	
	
%%

/* Function for recognizing errors*/
int yyerror(char * yaccProvidedMessage){
	fprintf(stderr, "%s: at line %d, before token: %s\nINPUT NOT VALID\n", yaccProvidedMessage, yylineno, yytext);
}

void free_string(void *data)
{
  free(*(char **)data);
}

int main (int argc, char ** argv){
	
	int remainingLibFun = 0;
	
	mySymbolTable = createSymbolTable();
	initStack(localArgStack);
	stack_new(&numStack, sizeof(double), NULL);
	stack_new(&strStack, sizeof(char *), free_string);
	stack_new(&libFuncStack, sizeof(char *), free_string);
	stack_new(&userFuncStack, sizeof(SymbolTableEntry_t *), NULL);
	/* Insert in symbol table all library functions with scope 0 and type LIBFUNC*/
	while(remainingLibFun < MAXLIBFUN){
	
		SymbolTableEntry_t* newEntry = createSymbolTableEntry (libraryFunctions[remainingLibFun], 0, 0, LIBFUNC, currscopespace(),currscopeoffset());
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
	
	if (error == true){
		
			printf("Compilation failed..\n");
	
	}else{
			printf("Compilation was successful! Printing quads in quads.txt..\n");
			printQuadsArray();
			generate_instructions();
			printInstructions();
			printINBytes();
	}	
	
	

	freeSymbolTable(mySymbolTable);
	
	return 0;
}
