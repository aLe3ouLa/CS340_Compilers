#include "ExpressionTable.h"

extern SymTable_t * mySymbolTable;
extern int yylineno;
extern int scope; 
extern int tempVarCounter;
extern looplist *breaklist;
extern looplist *continuelist;
extern FILE *fp;

unsigned int nextQuad = 0;
unsigned 	total = 0;
unsigned int    currQuad = 0;
quad*  quads = (quad*) 0;


char *opcode_str[]= { "assign", "add", "sub", "mul", "div", "mod", "uminus",
					  "and", "or", "not", "if_eq", "if_noteq", "if_lesserq", "if_greatereq","if_less",
					  "if_greater","call","param", "ret", "getretval","funcstart","funcend","tablecreate",
					  "jump","tablegetelem","tablesetelem"};
					  
char *type_str[] = { "var_e", "tableitem_e", "programfunc_e", "libraryfunc_e", "arithexpr_e", "boolexpr_e", 
					 "assignexpr_e", "newtable_e", "constnum_e", "constbool_e", "conststring_e", "nil_e"};					  


char* getOpCode(iopcode_t op){
	char* str_op= opcode_str[op]; 
	return str_op;
}


/* Creates a new Expression */
expr* createExpression(expr_t type, SymbolTableEntry_t* SymEntry, int numVal, char* strVal, unsigned char boolVal){
	
	/* Allocate a new node for an expression */
	expr* tempExpression = malloc(sizeof(expr));
	
	
	tempExpression->type = type;
	tempExpression->sym = SymEntry;
	tempExpression->index = NULL;
	tempExpression->next = NULL;
	tempExpression->isNumber = 0;
	tempExpression->isBoolean = 0;
	tempExpression->isString = 0;

	switch(type){
		case conststring_e : 
			tempExpression->strConst = malloc ((sizeof(strVal) + 1 ) * sizeof(char));
			strcpy(tempExpression->strConst, strVal);
			/* For error checking*/
			tempExpression->isString = 1;
    
			tempExpression->isConstant = 1;
			tempExpression->hasValue = 1;

			break;
		case constnum_e    : 
			tempExpression->numConst = numVal;
			tempExpression->isNumber = 1;

            		tempExpression->isConstant = 1;
			tempExpression->hasValue = 1;
			break;
		case constbool_e   : 
			tempExpression->boolConst = boolVal;
			tempExpression->isBoolean = 1;

          		tempExpression->isConstant = 1;
			tempExpression->hasValue = 1;
			break; 
		case arithexpr_e   : 
			tempExpression->numConst = numVal;    
			tempExpression->isNumber = 1;

          		tempExpression->hasValue = 1;
			break; 
		default            : 
			printf("default");
			//assert(0);
	}

	return tempExpression;
}


void expand (void){
	/* If we have reach the current size expand the array */
	quad* newQuads = (quad*) malloc(NEW_SIZE);
	
	if (newQuads == NULL){
		fprintf(stderr, "Cannot allocate the hash table.\n");
	    exit(EXIT_FAILURE);
	}
	
	if (quads){
		/* Copy the old array to new one*/
		memcpy(newQuads, quads, CURR_SIZE);
		/* Free the old array*/
		free(quads);
	}
	/* Make quads the array again*/
	quads = newQuads;
	/* Update the total */
	total += EXPAND_SIZE;
}


void emit(iopcode_t op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line){

	if(currQuad == total) { 
	/* if we reached total size of quad array expand it */
		expand();
	}
	/* Add a new quad in quadArray */
	quad* newQuad = quads + currQuad++;
	
	/* Fill the quads' details */
	newQuad->op = op;
	newQuad->arg1 = arg1;
	newQuad->arg2 = arg2;
	newQuad->result = result;
	newQuad->label = label;
	newQuad->line = line;

}


/*
 * Make an l-value expression from a symbol table entry.
 * The exprssion inherits the symbol type. 
 * LOCAL, GLOBAL & FORMAL are var_s , USERFUNC is programfunc and LIBFUNC is libraryfunc
 * */

expr* lvalue_expr (SymbolTableEntry_t* sym){
	
	assert(sym);
	
	expr* e = (expr*) malloc(sizeof(expr));
	memset (e, 0, sizeof(expr));
	
	e->next = (expr*) 0;
	e->sym = sym;
	
	switch(sym -> type){
		case LOCALVAR		   :    e->type = var_e; break;
		case GLOBALVAR		   :    e->type = var_e; break;
		case USERFUNC          :    e->type = programfunc_e; break;
		case LIBFUNC           :    e->type = libraryfunc_e; break;
		case FORMAL			   : 	e->type = var_e; break;
		default				   :    assert(0);
	}
	return e;
}

expr* newexpr (expr_t t){
	expr* e = malloc(sizeof(expr));
	memset (e, 0, sizeof(expr));
	e->type = t;
	return e;
}

expr* newexpr_conststring (char* s){
	expr* e = malloc(sizeof(expr));
	memset (e, 0, sizeof(expr));
	e->type = conststring_e;
	e->strConst = strdup(s);
	return e;

}
expr* newexpr_constnum(int num){
	expr* e = newexpr(constnum_e);
	e->numConst = num;
	return e;

}


expr* newexpr_constbool(unsigned char booleanVal){
	expr* e = newexpr(constbool_e);
	e->boolConst =  booleanVal;
	return e;

}


int IsInteger(double n){
  return n - (double)(int)n == 0.0;
}

void printQuadsArray(void){
	int i = 0;
	fprintf(fp,"Index:\t          Opcode\t    Result\t Arg1\tArg2\tLabel\tLine\n");
    for(i = 0; i<currQuad; i++){
        	fprintf(fp, "%2d:\t%16s\t", i, opcode_str[quads[i].op]);

         	if(quads[i].result != NULL){
			/* If the result is not null, if it has symbol print symbol name else print its value*/
			if ((quads[i].result)->sym != NULL){
				/* Print the symbol name*/
				fprintf(fp, "%10s\t",(quads[i].result)->sym->name);
			}else{
				/* Print the const value*/
				switch((quads[i].result)->type){
					case constnum_e:
						if (IsInteger((quads[i].result)->numConst)){
							fprintf(fp, "%d\t",(int)(quads[i].result)->numConst);
						}else{
							fprintf(fp, "%f\t",(quads[i].result)->numConst);
						}
						break;
					case conststring_e:
						fprintf(fp,"\"%s\"\t",(quads[i].result)->strConst);
						break;
					case constbool_e:
						if ((quads[i].result)->boolConst == 1){
							fprintf(fp, "TRUE\t");
						}else{
							fprintf(fp, "FALSE\t");
						}
						break;
					case nil_e:			
						fprintf(fp, "NIL\t");
						break;
					default:
						//assert(0);
						fprintf(fp, "\t");
						break;

				}
			}
			

		}else{
			fprintf(fp,"\t");
		}



		if(quads[i].arg1 != NULL){

			if ((quads[i].arg1)->sym != NULL){
				/* Print the symbol name*/
				fprintf(fp, "%s\t",(quads[i].arg1)->sym->name);
			}else{
				/* Print the const value*/
				switch((quads[i].arg1)->type){
					case constnum_e:
						if (IsInteger((quads[i].arg1)->numConst)){
							fprintf(fp, "%d\t",(int)(quads[i].arg1)->numConst);
						}else{
							fprintf(fp, "%f\t",(quads[i].arg1)->numConst);
						}
						break;
					case conststring_e:
						fprintf(fp,"\"%s\"\t",(quads[i].arg1)->strConst);
						break;
					case constbool_e:
						if ((quads[i].arg1)->boolConst == 1){
							fprintf(fp, "TRUE\t");
						}else{
							fprintf(fp, "FALSE\t");
						}
						break;
					case nil_e:			
						fprintf(fp, "NIL\t");
						break;
					default:
						//assert(0);
						fprintf(fp, "\t");
						break;

				}


			}


		}else{
			fprintf(fp,"\t");
		}



		if(quads[i].arg2 != NULL){

			if ((quads[i].arg2)->sym != NULL){
				/* Print the symbol name*/
				fprintf(fp, "%s\t",(quads[i].arg2)->sym->name);
			}else{
				/* Print the const value*/
				switch((quads[i].arg2)->type){
					case constnum_e:
						if (IsInteger((quads[i].arg2)->numConst)){
							fprintf(fp, "%d\t",(int)(quads[i].arg2)->numConst);
						}else{
							fprintf(fp, "%f\t",(quads[i].arg2)->numConst);
						}
						break;
					case conststring_e:
						fprintf(fp,"\"%s\"\t",(quads[i].arg2)->strConst);
						break;
					case constbool_e:
						if ((quads[i].arg2)->boolConst == 1){
							fprintf(fp, "TRUE\t");
						}else{
							fprintf(fp, "FALSE\t");
						}
						break;
					case nil_e:			
						fprintf(fp, "NIL\t");
						break;
					default:
						//assert(0);
						fprintf(fp, "\t");
						break;

				}


			}


		}else{
			fprintf(fp,"\t");
		}
        	if(quads[i].label != -1){
        		fprintf(fp, "%d\t",quads[i].label);
		}else{
			fprintf(fp, "-\t");
		}

		fprintf(fp, "%d\n",quads[i].line);
	}

    
     fclose(fp);
 
}


void backpatch(boolList* list, int label){
	int i;
	int *temp;
	
	if (list==NULL) {
		return;
	}
	
	while(list!=NULL){
		int listquad = list->quadlabel;
		printf("quad[%d] = %d\n", listquad, label);
		patchlabel(listquad, label);
		list = list->next;
	}
	
}


void patchlabel(unsigned quadNo, unsigned label){
	assert(quadNo < currQuad);
	
	quads[quadNo].label = label;
}

/* Reset the number of hidden Variables. At the end of a stmt, all hidden variables are available. */

int resetTemp(){
	tempVarCounter = 0;
}

/* Return the index of current quad. */

unsigned nextquadlabel(void){
	return currQuad;	
}


char *nameTempFunction(int counter){
/* Gives a fixed name in unnamed function*/
	char str[10];
    	sprintf(str, "_f%d",  counter);
    	return strdup(str);

}							 
	
						 
char *nameTempVariable(int counter){
/* Gives a fixed name in temp variable*/
     	char str[10];
    	sprintf(str, "_t%d",  counter);
    	return strdup(str);
}

expr* emit_iftableitem (expr* e){
	//printf("emit_iftableitem\n");
    if (e->type != tableitem_e){
		//printf("Into emit_iftableitem: type!= tableitem_e result->sym: %s\n", e->sym->name);
        return e;
    }else {
        expr* result = newexpr(var_e);
        result->sym = newtemp(mySymbolTable,nameTempVariable(tempVarCounter++),currscopespace(),scope, yylineno, currscopeoffset());
       // printf("Into emit_iftableitem: result->sym: %s\n", result->sym->name);
        emit(tablegetelem,e,e->index,result, 0, 0);
        return result;
    }
    return NULL;
 
}

expr* member_item(expr* lvalue, char* name){
	
	lvalue = emit_iftableitem(lvalue);
	
	expr * item = newexpr(tableitem_e);
	item->sym = lvalue->sym;
	item->index = newexpr_conststring(name);
	return item;

}



/* Check if minus has illegal operand. */
void checkminus (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in unary '-'\n", yylineno);
		

}

void checkAddition (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in '+'\n", yylineno);
		

}

void checkSubtraction (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in '-'\n", yylineno);
		

}

void checkMultiplication (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in '*'\n", yylineno);
		

}

void checkDivision (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in '/'\n", yylineno);
		

}

void checkModular (expr * expression){
	
	if (expression->type == constbool_e 			||
		expression->type == conststring_e			||
		expression->type == nil_e					||
		expression->type == newtable_e				||
		expression->type == programfunc_e			||
		expression->type == libraryfunc_e 			||
		expression->type == boolexpr_e)
		fprintf(stdout,"Warning <#%d>: Operand mismatch in 'MOD'\n", yylineno);
		

}


void Manage_break(int whileLoop){
	
	unsigned temp;
	
	resetTemp(); 
	temp = nextquadlabel();
	
	if (!whileLoop){
	/* Wrong placement of break */
			fprintf(stdout, "SYNTAX ERROR<#%d>:: Break statement not within loop.\n",yylineno);
			exit(EXIT_FAILURE);
	}
	emit (jump, NULL, NULL, NULL, -1, yylineno);
	makelooplist(&breaklist, temp);	
	return;
}

void Manage_continue(int whileLoop){
	
	unsigned temp;
	
	resetTemp(); 
	temp = nextquadlabel();
	
	if (!whileLoop){
	/* Wrong placement of continue */
			fprintf(stdout, "SYNTAX ERROR<#%d>:: Continue statement not within loop.\n",yylineno);
			exit(EXIT_FAILURE);
	}
	emit (jump, NULL, NULL, NULL, -1, yylineno);
	makelooplist(&continuelist, temp);	
	return;
}


void Manage_return(int intoFunction){
	
	if (intoFunction == 0){
		fprintf(stdout, "SYNTAX ERROR<#%d>: Return not in function.\n", yylineno);
		exit(EXIT_FAILURE);
	}
	
	emit(ret,NULL,NULL,NULL,-1,yylineno);
	fprintf(stdout, "Rule used: returnstmt -> RETURN;\n"); 
	return;
}

void Manage_return_expr_semicolon(int intoFunction, expr * expr){
	
	if (intoFunction == 0){
		fprintf(stdout, "SYNTAX ERROR<#%d>: Return not in function.\n",yylineno);
		exit(EXIT_FAILURE);
	}
	
	emit(ret,NULL,NULL,expr,-1,yylineno);
	fprintf(stdout, "Rule used: returnstmt -> RETURN expr; \n"); 
	return;
}

char * Manage_funcname(char * name){
		return name;
}

