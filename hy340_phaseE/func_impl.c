#include <assert.h>
#include <stdio.h>      
#include <math.h>    
#include <stdlib.h>
#include <ctype.h> 

#include "fun.h"
#include "alpha_vm.h"

#define AVM_NUMACTUALS_OFFSET   + 4
#define AVM_SAVEDPC_OFFSET      + 3
#define AVM_SAVEDTOP_OFFSET     + 2
#define AVM_SAVEDTOPSP_OFFSET   + 1


extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern unsigned top;
extern unsigned topsp;
extern avm_memcell stack[AVM_STACKSIZE];
extern unsigned char executionFinished;
extern unsigned pc;

extern double * doubleArray;
extern char ** strArray;
extern char ** libArray;
extern userfunc ** userArray;
extern unsigned int codeCounter;

extern unsigned int totalNumUF;
extern unsigned int totalNumLF;

extern instruction *instructions;

unsigned totalActuals = 0;

avm_table_bucket* printed[100];
int indexprinted = 0;

char * vt_str[]= { "number", 
				   "string", 
				   "bool", 
				   "table", 
				   "userfunction", 
				   "libraryfunction", 
				   "nil",
				   "undef"};

typedef void (*library_func_t)(void);				   

char* libs[] = { "print",
				  "input",
				  "objectmemberkeys",
				  "objecttotalmembers",
				  "objectcopy",
				  "totalarguments",
				  "argument",
				  "typeof",
				  "strtonum",
				  "sqrt",
				  "cos",
				  "sin"
	};				   

library_func_t libFuncs[] = {
				libfunc_print,
				libfunc_input,
				libfunc_objectmemberkeys,
				libfunc_objecttotalmembers,
				libfunc_objectcopy,
				libfunc_totalarguments,
				libfunc_argument,
				libfunc_typeof,
				libfunc_strtonum,
				libfunc_sqrt,
				libfunc_cos,
				libfunc_sin
};


typedef char *(*tostring_func_t) (avm_memcell*);


tostring_func_t tostringFuncs[] = {
	number_tostring,
	string_tostring,
	bool_tostring,
	table_tostring,
	userfunc_tostring,
	libfunc_tostring,
	nil_tostring,
	undef_tostring
};

char* number_tostring (avm_memcell* m){
		double  num = m->data.numVal;
		char output[100];
		snprintf(output,100,"%.3lf",num);
		return strdup(output);
}
char* string_tostring (avm_memcell* m){
		return strdup(m->data.strVal);
}
char* bool_tostring (avm_memcell* m){
	unsigned char b = m->data.boolVal;
	if(b == 0 || b == 'T')
		return strdup("false");
	else
		return strdup("true");
}
char * retName(unsigned addr){
		int i = 0;
		for (; i< totalNumUF; i++){
			if (userArray[i]->address == addr){
				return userArray[i]->id;
			}
		}
}
void printBucket(struct avm_table_bucket** p, char * output){
	int i = 0;
	for (;  i < AVM_TABLE_HASHSIZE; i++){
		
		if (p[i] != NULL){
			avm_table_bucket * temp = p[i];
			
			while(temp!= NULL){
				//printf("i: %d\n", i);
				
				printed[indexprinted] = temp;
				indexprinted++;
				temp = temp->next;
			}
			
			
		}
	}	
}
char* table_tostring (avm_memcell* m){
	/* Print table some how */
		char output[3024];
		int i = 0;
		int len = 0;
		avm_table* temp = m->data.tableVal;
		printBucket(temp->numIndexed, output);
		printBucket(temp->strIndexed, output);
		printBucket(temp->boolIndexed, output);
		printBucket(temp->libFuncIndexed, output);
		printBucket(temp->userFuncIndexed, output);
		printBucket(temp->tableIndexed, output);
		
		len += sprintf(output + len, "{");
		for (i = 0; i < indexprinted; i++){
			if (printed[i]->key.type == number_m){
					if (printed[i]->value.type == number_m){
						len += sprintf(output+ len,"[%lf: %lf]", printed[i]->key.data.numVal, printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[%lf: %s]", printed[i]->key.data.numVal, printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[%lf: false]", printed[i]->key.data.numVal );
						}else{
							len += sprintf(output + len,"[%lf: true]", printed[i]->key.data.numVal );
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[%lf: %p]", printed[i]->key.data.numVal, printed[i]->value.data.tableVal );
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[%lf: %s]", printed[i]->key.data.numVal, userArray[printed[i]->value.data.funcVal]->id );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[%lf: %s]", printed[i]->key.data.numVal, printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[%lf: nil]", printed[i]->key.data.numVal );
					}else{
						len += sprintf(output + len,"[%lf: undef]", printed[i]->key.data.numVal );
					}
			}else if (printed[i]->key.type == string_m){
				//printf("str as key\n");
					if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[%s: %lf]", printed[i]->key.data.strVal, printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[%s: %s]", printed[i]->key.data.strVal, printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[%s: false]", printed[i]->key.data.strVal );
						}else{
							len += sprintf(output + len,"[%s: true]",printed[i]->key.data.strVal );
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[%s: %p]", printed[i]->key.data.strVal, printed[i]->value.data.tableVal );
						//table_tostring (&printed[i]->value);
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[%s: %s]", printed[i]->key.data.strVal, retName(printed[i]->value.data.funcVal) );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[%s: %s]", printed[i]->key.data.strVal, printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[%s: nil]",printed[i]->key.data.strVal );
					}else{
						len += sprintf(output + len,"[%s: undef]",printed[i]->key.data.strVal );
					}
			}else if (printed[i]->key.type == bool_m){
				//printf("str as key\n");
				if (printed[i]->key.data.boolVal == 1){
					if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[true: %lf]", printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[true: %s]", printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[true: false]" );
						}else{
							len += sprintf(output + len,"[true: true]" );
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[true: %p]", printed[i]->value.data.tableVal );
						//table_tostring (&printed[i]->value);
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[true: %s]",  userArray[printed[i]->value.data.funcVal]->id );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[true: %s]", printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[true: nil]");
					}else{
						len += sprintf(output + len,"[true: undef]");
					}
				}else{
					
					if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[false: %lf]", printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[false: %s]", printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[false: false]" );
						}else{
							len += sprintf(output + len,"[false: true]" );
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[false: %p]", printed[i]->value.data.tableVal );
						//table_tostring (&printed[i]->value);
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[false: %s]",  userArray[printed[i]->value.data.funcVal]->id );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[false: %s]", printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[false: nil]");
					}else{
						len += sprintf(output + len,"[false: undef]");
					}
					
				}
			}else if (printed[i]->key.type == libfunc_m){
				
				if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[%s: %lf]", printed[i]->key.data.libfuncVal, printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[%s: %s]", printed[i]->key.data.libfuncVal, printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[%s: false]", printed[i]->key.data.libfuncVal );
						}else{
							len += sprintf(output + len,"[%s: true]",printed[i]->key.data.libfuncVal);
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[%s: %p]", printed[i]->key.data.libfuncVal, printed[i]->value.data.tableVal );
						//table_tostring (&printed[i]->value);
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[%s: %s]", printed[i]->key.data.libfuncVal, retName(printed[i]->value.data.funcVal));
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[%s: %s]", printed[i]->key.data.libfuncVal, printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[%s: nil]",printed[i]->key.data.libfuncVal );
					}else{
						len += sprintf(output + len,"[%s: undef]",printed[i]->key.data.libfuncVal );
					}
			}else if (printed[i]->key.type == userfunc_m){
				
				if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[%s: %lf]", retName(printed[i]->key.data.funcVal), printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[%s: %s]", retName(printed[i]->key.data.funcVal), printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[%s: false]", retName(printed[i]->key.data.funcVal) );
						}else{
							len += sprintf(output + len,"[%s: true]",retName(printed[i]->key.data.funcVal));
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[%s: %p]", retName(printed[i]->key.data.funcVal), printed[i]->value.data.tableVal );
						//table_tostring (&printed[i]->value);
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[%s: %s]", retName(printed[i]->key.data.funcVal), retName(printed[i]->value.data.funcVal) );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[%s: %s]", retName(printed[i]->key.data.funcVal), printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[%s: nil]",retName(printed[i]->key.data.funcVal));
					}else{
						len += sprintf(output + len,"[%s: undef]",retName(printed[i]->key.data.funcVal));
					}
			}else if (printed[i]->key.type == table_m){
				
				if (printed[i]->value.type == number_m){
						len += sprintf(output + len,"[%p: %lf]", printed[i]->key.data.tableVal, printed[i]->value.data.numVal );
					}else if (printed[i]->value.type == string_m){
						len += sprintf(output+ len,"[%p: %s]", printed[i]->key.data.tableVal, printed[i]->value.data.strVal );
					}else if (printed[i]->value.type == bool_m){
						if (printed[i]->value.data.boolVal == 0){
							len += sprintf(output + len,"[%p: false]", printed[i]->key.data.tableVal );
						}else{
							len += sprintf(output + len,"[%p: true]",printed[i]->key.data.tableVal);
						}
					}else if (printed[i]->value.type == table_m){
						len += sprintf(output + len,"[%p: %p]", printed[i]->key.data.tableVal, printed[i]->value.data.tableVal );
					}else if (printed[i]->value.type == userfunc_m){
						len += sprintf(output + len,"[%p: %s]", printed[i]->key.data.tableVal , retName(printed[i]->value.data.funcVal) );
					}else if (printed[i]->value.type == libfunc_m){
						len += sprintf(output + len,"[%p: %s]", printed[i]->key.data.tableVal, printed[i]->value.data.libfuncVal );
					}else if (printed[i]->value.type == nil_m){
						len += sprintf(output + len,"[%p: nil]",printed[i]->key.data.tableVal);
					}else{
						len += sprintf(output + len,"[%p: undef]",printed[i]->key.data.tableVal);
					}
			}else{}
			
		}
		len += sprintf(output + len,"}");
		
		
		return strdup(output);
}
char* userfunc_tostring (avm_memcell* m){
	 char output[100];
	 snprintf(output,100,"user function: <%s, %d> ",retName(m->data.funcVal), m->data.funcVal);
	 return strdup(output);
}
char* libfunc_tostring (avm_memcell* m){
	char output[100];
	 snprintf(output,100,"library func: <%s> ",m->data.libfuncVal);
	return strdup(output);
}
char* nil_tostring (avm_memcell* m){
	return strdup("nil");
}
char* undef_tostring (avm_memcell* m){
		return strdup("undef");
}



library_func_t avm_getlibraryfunc (char * id){
		int i = 0;
		while (i < 12){
			if (!strcmp(libs[i], id)){
				//printf("i: %d, lib[i]: %s\n", i, libs[i]);
				return (libFuncs[i]);
			}
			i++;
		}
		return NULL;
		//avm_error ("Not supported library");
	
}

void avm_calllibfunc(char* id){
	
		library_func_t f = avm_getlibraryfunc(id);
		if (!f){
			avm_error("unsupported lib func called!");
			executionFinished == 1;
		}else{
			/* NOtice that enter function and exit function are called manually!*/
			topsp = top; /* Enter function sequence. No stack locals.*/
			totalActuals = 0;
			(*f)(); /* Call library function */
			if (!executionFinished) /* AN error may naturally occur inside*/
				execute_FUNCEXIT((instruction *)0); /* Return sequence */
			
		}
}

char * avm_tostring (avm_memcell * m){
	assert(m->type >= 0 && m->type <= undef_m);
	return (*tostringFuncs[m->type])(m);
}

void avm_dec_top(void){
		if (!top){
			avm_error("stack overflow");
			executionFinished = 1;
		}else{
			--top;
		}
}

void avm_push_envvalue(unsigned val){
		stack[top].type = number_m;
		stack[top].data.numVal = val;
		avm_dec_top();
}

void avm_callsaveenvironment (void) {
	avm_push_envvalue(totalActuals);
	avm_push_envvalue(pc+1);
	avm_push_envvalue(top + totalActuals +2);
	avm_push_envvalue(topsp);
}

void execute_CALL(instruction* instr){
	avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);
	
	/*if (func->type == table_m) {
        avm_assign(&stack[top],func);
        ++totalActuals;
        avm_dec_top();
    }*/
    
	avm_callsaveenvironment();
	
	switch (func->type){
		case userfunc_m: {
			//printf("func->val: %d", func->data.funcVal);
			pc = func->data.funcVal;
			assert(pc < AVM_ENDING_PC);
			assert(instructions[pc].opcode == funcenter_v);
			break;
		}
		
		case string_m: {
			avm_calllibfunc(func->data.strVal);
			break;
		}
		
		case libfunc_m: {
			avm_calllibfunc(func->data.libfuncVal);
			break;
		}
		
		default: {
			//char * s  = avm_tostring(func);
			//avm_error("call: cannot bind %s  to function ", s);
			avm_error("call: cannot bind  to function ");
			//free(s);
			executionFinished = 1;
		}
		
		
	}
	
}

userfunc * avm_getfuncinfo(unsigned address){
	int i = 0;
	for (; i < totalNumUF; i++){
		if (address == userArray[i]->address){
					return userArray[i];
		}
	}
	return NULL;
}

void execute_FUNCENTER (instruction* instr){
	avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);
	assert(pc == func->data.funcVal);
	
	/* Calles actions here. */
	totalActuals = 0;
	userfunc* funcInfo = avm_getfuncinfo(pc);
	topsp = top;
	top = top - funcInfo->localSize;
	
}

unsigned avm_get_envvalue(unsigned i){	
	assert(stack[i].type == number_m);
	unsigned val = (unsigned) stack[i].data.numVal;
	assert( stack[i].data.numVal == (double)val);
	return val;
}

void execute_FUNCEXIT (instruction* instr){	
	unsigned oldTop = top;
	top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
	topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	
	while(++oldTop <= top){
		avm_memcellclear(&stack[oldTop]);
	}
}

unsigned avm_totalactuals (void){
		return avm_get_envvalue(topsp +  AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual (unsigned i){
		assert(i < avm_totalactuals());
		return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

/* Implementation of the library function "print". It displays every argument at the console. */

void libfunc_print(void){
	int i = 0;
		unsigned n = avm_totalactuals();
		for (i = 0; i < n; ++i){
			//printf("avm_getactual(i): %d\n", avm_getactual(i)->type);
				char* s = avm_tostring(avm_getactual(i));
				int j= 0;
				for (; j < strlen(s); j++){
					if (s[j] == '\"')
						continue;
					else
						fputc(s[j], stdout);	
					
				}
				//fputs(s, stdout);
				//puts(s);
				free(s);
		}
}

void libfunc_input(void){
	
	char * input = malloc (sizeof(char) * 1024);
	int count = 0;
	char f = fgetc(stdin);
	
	while (f != EOF && f != '\n'){
			input[count] = f;
			count++;
			f = fgetc(stdin);
	}
	input[count] = '\0';
	
	
	avm_memcellclear(&retval);
	
	/* Check if it is number */
	double db = strtod(input, NULL);
	retval.type = number_m;
	retval.data.numVal = db;
	printf("%lf", db);
	if (db == 0.0){
				/* Not number or just zero */
				int length = strlen(input);
				int i = 0;
				int num = 0;
				int dot = 0;
				
				for (; i < length; i++){
					if (isdigit(input[i])){
						num++;
					}else{
						if (input[i] == '.'){
								dot++;
								if (dot > 1){
									break;
								}
								continue;
						}
						break;
					}
				}
				
				if (dot == 1 && num == length){
						retval.type = number_m;
						retval.data.numVal = db;
				}else if (!strcmp(input, "true")){
					retval.data.boolVal = 1;
					retval.type = bool_m;
					return;
				}else if (!strcmp(input, "false")){
					retval.data.boolVal = 0;
					retval.type = bool_m;
					return;
				}else{
					retval.data.strVal = strdup(input);
					retval.type = string_m;
				}
				
				
			}
	

	
	
	
}

void libfunc_typeof (void) {

	unsigned n = avm_totalactuals();
	
	if (n !=1)
		avm_error("one argument expected in 'typeof'!");
	else {
		/*Thats how a library function returns a result.
		  It has to only set the 'retval' register!*/
		avm_memcellclear(&retval); /* Don't forget to clean-it up!*/
		retval.type = string_m;
		retval.data.strVal = strdup(vt_str[(avm_getactual(0))->type]);
	}	
}

void libfunc_objectmemberkeys(void){
	unsigned n = avm_totalactuals();
	
	if (n !=1)
		avm_error("one argument expected in 'objectmemberkeys'!");
	if (avm_getactual(0)->type == table_m){
		
		
		avm_memcellclear(&retval);
		retval.type = table_m;
		retval.data.tableVal =  avm_getactual(0)->data.tableVal;
	}else{
		avm_error("'objectmemberkeys' expects table element");
	}	
}

void libfunc_objecttotalmembers (void){
	unsigned n = avm_totalactuals();
	
	if (n !=1)
		avm_error("one argument expected in 'objecttotalmembers'!");
		
	if (avm_getactual(0)->type == table_m){
		avm_memcellclear(&retval);
		retval.type=number_m;
		retval.data.numVal = avm_getactual(0)->data.tableVal->total;
	}else{
		avm_error("'objecttotalmembers' expects table element");
	}	
			
}

void libfunc_objectcopy(void){
	unsigned n = avm_totalactuals();
	
	if (n !=1)
		avm_error("one argument expected in 'objectcopy'!");
		
	if (avm_getactual(0)->type == table_m){
		avm_memcellclear(&retval);
		retval.type = table_m;
		retval.data.tableVal = avm_getactual(0)->data.tableVal;
	}else{
		avm_error("'objecttotalmembers' expects table element");
	}
}

void libfunc_totalarguments (void){
	/*Get topsp of previous activation record*/
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);
	
	if (!p_topsp){ /*If 0, no previous activation record.*/
		avm_error("'totalarguments' called outside a function!");
		retval.type = nil_m;
	}else{
		/* Extract the number of actual arguments for the 
		previous activation record.*/
		retval.type = number_m;
		retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
	}
	
}

void libfunc_argument (void){
	/*Get topsp of previous activation record*/
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);
	
	if (!p_topsp){ /*If 0, no previous activation record.*/
		avm_error("'arguments' called outside a function!");
		retval.type = nil_m;
	}else{
		
		unsigned n = avm_totalactuals();
	
		if (n !=1)
			avm_error("one argument expected in 'arguments'!");
		else{	
			/* Extract the number of actual arguments for the 
			previous activation record.*/
			retval.type = number_m;
			retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET+ n);
		}
	}
	
}

void libfunc_strtonum (void){
	unsigned n = avm_totalactuals();
	
	if (n!= 1)
		avm_error("one argument expected in 'strtonum'!");
	else{
		if (avm_getactual(0)->type == string_m) {
			avm_memcellclear(&retval); /* Don't forget to clean-it up!*/
			char * str = strdup(avm_getactual(0)->data.strVal);
			/* Check if the str is number ->> all are digits. */
			double d = strtod(str, NULL);
			int error = 0;
			
			retval.data.numVal = d;			
			retval.type = number_m;
			if (d == 0.0){
				/* CHeck if its actually wrong or the 0.0*/
				int length = strlen(str);
				int i = 0;
				int num = 0;
				int dot = 0;
				
				for (; i < length; i++){
					if (isdigit(str[i])){
						num++;
					}else{
						if (str[i] == '.'){
								dot++;
								if (dot > 1){
									break;
									error = 1;
									retval.type = nil_m;
								}
								continue;
						}
						avm_error("String is not number");
						error = 1;
						retval.type = nil_m;
						break;
					}
				}
			}
			
		
		}else{
			avm_error("strtonum is used only with strings!");
		}
	}
		
}

void libfunc_sqrt (void){
	unsigned n = avm_totalactuals();
	
	if (n!= 1)
		avm_error("one argument expected in 'cos'!");
	else{
		if (avm_getactual(0)->type == number_m) {
			avm_memcellclear(&retval); /* Don't forget to clean-it up!*/
			retval.type = number_m;
			retval.data.numVal = sqrt(avm_getactual(0)->data.numVal);
		}else{
			avm_error("Sqrt is used only with numbers!");
		}
	}
}

void libfunc_cos (void){
	unsigned n = avm_totalactuals();
	
	if (n!= 1)
		avm_error("one argument expected in 'cos'!");
	else{
		if (avm_getactual(0)->type == number_m) {
			avm_memcellclear(&retval); /* Don't forget to clean-it up!*/
			retval.type = number_m;
			retval.data.numVal = cos(avm_getactual(0)->data.numVal);
		}else{
			avm_error("Cos is used only with numbers!");
		}
	}	
}

void libfunc_sin (void){
	unsigned n = avm_totalactuals();
	
	if (n!= 1)
		avm_error("one argument expected in 'sin'!");
	else{
		if (avm_getactual(0)->type == number_m) {
			avm_memcellclear(&retval); /* Don't forget to clean-it up!*/
			retval.type = number_m;
			retval.data.numVal = sin(avm_getactual(0)->data.numVal);
		}else{
			avm_error("Sin is used only with numbers!");
		}
	}	
}


/* With the following every library function is manually added in the VM library function resolution map*/

void avm_registerlibfunc(char* id, library_func_t addr){
	
}

void execute_PUSHARG (instruction* instr){
	avm_memcell *arg = avm_translate_operand(instr->result, &ax);
	assert(arg);
	
	avm_assign(&stack[top], arg);
	++totalActuals;
	avm_dec_top();
}
