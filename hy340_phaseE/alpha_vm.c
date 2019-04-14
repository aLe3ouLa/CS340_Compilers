#include <stdio.h>
#include <stdlib.h>

#include "alpha_vm.h"
#include "fun.h"

instruction *instructions =(instruction *) 0;

double * doubleArray;
char ** strArray;
char ** libArray;
userfunc ** userArray;

unsigned int magicNumber = 0;
unsigned int totalNumStr = 0;
unsigned int totalNumDoub = 0;
unsigned int totalNumUF = 0;
unsigned int totalNumLF = 0;
unsigned int codeCounter = 0;
unsigned int totalGlobalVars = 0;

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;

avm_memcell stack[AVM_STACKSIZE];

avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned int top, topsp;


char *vmopcode_str[]= { "assign", "add", "sub", "mul", "div", "mod", "uminus",
					  "and", "or", "not", "jeq", "jne", "jle", "jge","jlt",
					  "jgt","call","pusharg", "ret", "getretval","funcenter","funcexit","newtable",
					  "jump","tablegetelem","tablesetelem", "nop"};
					  
char * vmarg_t_str[]= { "label_a", "global_a", "formal_a", "local_a", "number_a", "string_a", "bool_a", "nil_a", "userfc_a", "libfc_a", "retval_a"};

typedef void (*memclear_func_t)(avm_memcell *);
typedef void (*execute_func_t)(instruction *);

memclear_func_t memclearFuncs[] = {
	0, /*number*/
	memclear_string,
	0,/*bool*/
	memclear_table,
	0,/*userfunc*/
	0,/*livfunc*/
	0,/*nil*/
	0/*undef*/
};

execute_func_t executeFuncs[] = {
	execute_ASSIGN,
	execute_ADD,
	execute_SUB,
	execute_MUL,
	execute_DIV,
	execute_MOD,
	execute_UMINUS,
	execute_AND,
	execute_OR,
	execute_NOT,
	execute_JEQ,
	execute_JNE,
	execute_JLE,
	execute_JGE,
	execute_JLT,
	execute_JGT,
	execute_CALL,
	execute_PUSHARG,
	execute_RET,
	execute_GETRETVAL,
	execute_FUNCENTER,
	execute_FUNCEXIT,
	execute_NEWTABLE,
	execute_JUMP,
	execute_TABLEGETELEM,
	execute_TABLESETELEM,
	execute_NOP
};

void execute_cycle (void) {
	
	if (executionFinished){
		return;
	}else if (pc == AVM_ENDING_PC){
		executionFinished = 1;
		return;
	}else{
		assert(pc < AVM_ENDING_PC);
		instruction* instr = instructions + pc;
		assert(
			instr->opcode >= 0 &&
			instr->opcode <= AVM_MAX_INSTRUCTIONS
		);
		
		if (instr->srcLine){
			currLine = instr->srcLine;
		}
		unsigned oldPC = pc;
		//printf("op: %s\n",vmopcode_str[instr->opcode]);
		(*executeFuncs[instr->opcode]) (instr);
		if (pc == oldPC) {
			pc++;
		}

	}
	
}

avm_memcell* avm_translate_operand (vmarg* arg, avm_memcell* reg) {

	//printf("%s\n", vmarg_t_str[arg->type]);
	switch (arg->type){

		/*Variables*/
		case global_a: {
			//printf("stack[AVM_STACKSIZE-1-arg->val: %d\n", AVM_STACKSIZE-1-arg->val);
			return &stack[AVM_STACKSIZE-1-arg->val];
		}
		case local_a: {
			//printf("topsp-arg->val: %d\n", topsp-arg->val);
			return &stack[topsp-arg->val];
		}
		case formal_a: {
			return &stack[topsp+AVM_STACKENV_SIZE+1+arg->val];
		}
		case retval_a: {
			//printf("RETVAL: %lf\n", retval.data.numVal);
			return &retval;
		}
		/*Constants*/
		case number_a: {
			reg->type = number_m;
			reg->data.numVal = consts_getnumber(arg->val);
			//printf("reg->data.numVal: %lf\n", reg->data.numVal);
			return reg;
		}
		
		case string_a: {
			//printf("string: %s\n", consts_getstring(arg->val));
			reg->type = string_m;
			reg->data.strVal = consts_getstring(arg->val);
			return reg;
		}
		
		case bool_a: {
			reg->type = bool_m;
			reg->data.boolVal = arg->val;
			return reg;
		}
		
		case nil_a: {
			reg->type = nil_m; 
			return reg;
		}
		
		/*Functions*/
		
		case userfunc_a: {
			reg->type = userfunc_m;
			reg->data.funcVal = arg->val;
			return reg;
		}
		
		case libfunc_a: {
			reg->type = libfunc_m;
			reg->data.libfuncVal = libfuncs_getused(arg->val);
			return reg;
		}
		
		default : assert(0);
	}

}

void printStack(){
	int i = 0;
	printf("In here");
	for (; i < AVM_STACKSIZE; i++){
			if (stack[i].type != undef_m){
				printf("type: %d" ,stack[i].type);
			}
	}
	
	}

int getfunc(unsigned address){
	int i = 0;
	for (; i < totalNumUF; i++){
		if (address == userArray[i]->address){
					return i;
		}
	}
}

FILE * openFile(){
	FILE * fp = fopen("instr.bin", "rb+");
	
	if(!fp) {
        fprintf(stderr, "File opening failed");
        exit(EXIT_FAILURE);
    }
    
    return fp;
}

void readBinaryFile(FILE * fp){
	int i;
	
	
	fread(&magicNumber, sizeof(unsigned int), 1, fp);
	//printf("MagicNumber is: %d\n", magicNumber);
	if (magicNumber != 340200501){
		fprintf(stderr, "MagicNumber is not correct!\n");
		exit(EXIT_FAILURE);
	}
	
	fread(&totalGlobalVars, sizeof(unsigned int), 1, fp);
	//printf("Total Global vars: %d\n", totalGlobalVars);
	
	fread(&totalNumStr, sizeof(unsigned int), 1, fp);
	//printf("Total String Number is: %d\n", totalNumStr);
	
	if (totalNumStr != 0){
		strArray = malloc (totalNumStr * sizeof(char *));
		for (i = 0; i < totalNumStr; i++){
			unsigned int length = 0;
			fread(&length, sizeof(unsigned int), 1, fp);
			//printf("length: %d", length);
			
			strArray[i] = malloc (sizeof(char) * length);
			fread(strArray[i], sizeof(char) * length, 1, fp);
			//printf(" str: %s\n", strArray[i]);
		}
		
	}
	
	fread(&totalNumDoub, sizeof(unsigned int), 1, fp);
	//printf("Total Double Number is: %d\n", totalNumDoub);
	
	if (totalNumDoub != 0){
		doubleArray = malloc(sizeof(double)*totalNumDoub);
		for (i = 0; i < totalNumDoub; i++ ){
			fread(&(doubleArray[i]), sizeof(double), 1, fp);
		}
		
		//for (i = 0; i < totalNumDoub; i++ ){
			//printf("%lf\n", doubleArray[i]);
		//}
	}
	
	fread(&totalNumUF, sizeof(unsigned int), 1, fp);
	//printf("Total User Function Number is: %d\n", totalNumUF);
	
	if (totalNumUF != 0){
		userArray = malloc(totalNumUF * sizeof(userfunc *));
		
		for (i = 0; i < totalNumUF; i++){
			unsigned int address, localsize, length;
			char * str;
			fread(&address,sizeof(unsigned int),1,fp);
			fread(&localsize,sizeof(unsigned int),1,fp);
			fread(&length, sizeof(unsigned int),1,fp);
			str = malloc (length * sizeof(char));
			fread(str, length * sizeof(char), 1, fp);
			
			userArray[i] = malloc (sizeof(userfunc));
			
			userArray[i]->address = address;
			userArray[i]->localSize = localsize;
			userArray[i]->id = str;
			
			//printf("%d %d %s\n", userArray[i]->address, userArray[i]->localSize, userArray[i]->id  );
			
		}
	}
	
	fread(&totalNumLF, sizeof(unsigned int), 1, fp);
	//printf("Total Library Function Number is: %d\n", totalNumLF);
	if (totalNumLF != 0){
		libArray = malloc (totalNumLF * sizeof(char *));
		for (i = 0; i < totalNumLF; i++){
			unsigned int length = 0;
			fread(&length, sizeof(unsigned int), 1, fp);
			//printf("length: %d", length);
			
			libArray[i] = malloc (sizeof(char) * length);
			fread(libArray[i], sizeof(char) * length, 1, fp);
			//printf(" str: %s\n", libArray[i]);
		}
		
	}


	fread(&codeCounter,sizeof(unsigned int),1,fp);
	//printf("# of instructions: %d\n", codeCounter );
	
	if (codeCounter != 0){
		instructions = malloc (codeCounter * sizeof(instruction));
		for (i = 0; i < codeCounter; i++){
			int opcode = 0;
			int type = 0;
			unsigned int val = 0;
			unsigned int src = 0;
			vmarg * result = malloc(sizeof(vmarg));
			vmarg * arg1 = malloc(sizeof(vmarg));
			vmarg * arg2 = malloc(sizeof(vmarg));
			
			fread(&opcode,sizeof(int),1,fp);
			//printf("%s ", vmopcode_str[opcode]);
			/* add, sub, mul, div, mod -> 3 operands*/
			if (opcode == 0 || opcode == 18 || opcode == 19  ){
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				result->type = type;
				result->val = val;
				
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				arg1->type = type;
				arg1->val = val;
				
				arg2 = NULL;
				
				//printf("%s %d %s %d\n", vmarg_t_str[result->type], result->val, vmarg_t_str[arg1->type], arg1->val);
			} else if (opcode == 24 || opcode == 25 || (opcode >= 1 && opcode <= 15)){
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				result->type = type;
				result->val = val;
				
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				arg1->type = type;
				arg1->val = val;
				
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				arg2->type = type;
				arg2->val = val;
				//printf("%s %d %s %d %s %d\n", vmarg_t_str[result->type], result->val, vmarg_t_str[arg1->type], arg1->val, vmarg_t_str[arg2->type], arg2->val);
			}else if (opcode == 16 || opcode == 17 || opcode == 20 || opcode == 21 || opcode == 22 || opcode == 23 ){
				fread (&type, sizeof(int), 1, fp);
				fread (&val, sizeof(unsigned int), 1, fp);
				result->type = type;
				result->val = val;
				
				arg1 = NULL;
				arg2 = NULL;
				//printf("%s %d\n", vmarg_t_str[result->type], result->val);
			}
			
			fread(&src,sizeof(unsigned int),1,fp);
			
			instruction *instr  = instructions + i;
			instr->opcode = opcode;
			instr->result = result;
			instr->arg1 = arg1;
			instr->arg2 = arg2;
			instr->srcLine = src;
			
		}
	}
	
	//for (i = 0; i < codeCounter; i++){
		//printf("%s %d\n",  vmopcode_str[instructions[i].opcode] , instructions[i].srcLine);
	//}
	
	fclose(fp);
}

static void avm_initstack(void){
	int i = 0;
	for (; i < AVM_STACKSIZE; ++i){
		AVM_WIPEOUT(stack[i]);
		stack[i].type = undef_m;
	}
}

avm_table* avm_tablenew(void){
		avm_table* t = (avm_table*)malloc (sizeof(avm_table));
		AVM_WIPEOUT(*t);
		
		t->refCounter = t->total = 0;
		avm_tablebucketsinit(t->numIndexed);
		avm_tablebucketsinit(t->strIndexed);
		avm_tablebucketsinit(t->boolIndexed);
		avm_tablebucketsinit(t->userFuncIndexed);
		avm_tablebucketsinit(t->libFuncIndexed);
		avm_tablebucketsinit(t->tableIndexed);
		return t;
}
void avm_tabledestroy(avm_table* t){
		avm_tablebucketsdestroy(t->strIndexed);
		avm_tablebucketsdestroy(t->numIndexed);
		avm_tablebucketsdestroy(t->boolIndexed);
		avm_tablebucketsdestroy(t->userFuncIndexed);
		avm_tablebucketsdestroy(t->libFuncIndexed);
		avm_tablebucketsdestroy(t->tableIndexed);
		free(t);
}

void avm_tableincrefcounter(avm_table* t){
	++t->refCounter;	
}
void avm_tabledecrefcounter(avm_table* t){
	assert(t->refCounter >= 0);
	if (!--t->refCounter){
		avm_tabledestroy(t);
	}
}
void avm_tablebucketsinit(avm_table_bucket** p){
		int i = 0;
		for (i = 0; i < AVM_TABLE_HASHSIZE; ++i)
			p[i] = (avm_table_bucket*)0;
}
void avm_tablebucketsdestroy(avm_table_bucket** p){
		int i = 0;
		avm_table_bucket* b;
		for (i = 0; i < AVM_TABLE_HASHSIZE; ++i, ++p){
			for (b = *p; b;){
				avm_table_bucket* del = b;
				b = b->next;
				avm_memcellclear(&del->key);
				avm_memcellclear(&del->value);
				free(del);
			}
			p[i] = (avm_table_bucket*)0;
		}
}
void avm_memcellclear(avm_memcell* m){
		if (m->type != undef_m){
			memclear_func_t f = memclearFuncs[m->type];
			if (f)
				(*f)(m);
		
			m->type = undef_m;
		}
}

void memclear_string(avm_memcell* m){
		assert(m->data.strVal);
		free(m->data.strVal);
}

void memclear_table(avm_memcell* m){
		assert(m->data.tableVal);
		avm_tabledecrefcounter(m->data.tableVal);
}

void execute_NOP (){
		printf("Nop.\n");
}
void execute_UMINUS (instruction* instr){}
void execute_AND (instruction* instr){}
void execute_OR (instruction* instr){}
void execute_NOT (instruction* instr){}
void execute_RET (instruction* instr){}
void execute_GETRETVAL (instruction* instr){}

void avm_error(char* msg){
	executionFinished = 1;
	printf("\nRuntime Error: %s at line\n", msg, currLine);
}
void avm_warning(char* msg){
	printf("\nRuntime Warning: %s at line %d\n ",msg, currLine);
}


double consts_getnumber (unsigned int index){
		return doubleArray[index];
}
char* consts_getstring (unsigned index){
	 return strdup(strArray[index]);
}
char* libfuncs_getused (unsigned index){
		return strdup(libArray[index]);
}


void avm_initialize (void){
	avm_initstack();
	
	avm_registerlibfunc("print", libfunc_print);
	avm_registerlibfunc("input", libfunc_input);
	avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
	avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
	avm_registerlibfunc("objectcopy", libfunc_objectcopy);
	avm_registerlibfunc("totalarguments", libfunc_totalarguments);
	avm_registerlibfunc("argument", libfunc_argument);
	avm_registerlibfunc("typeof", libfunc_typeof);
	avm_registerlibfunc("strtonum", libfunc_strtonum);
	avm_registerlibfunc("sqrt", libfunc_sqrt);
	avm_registerlibfunc("cos", libfunc_cos);
	avm_registerlibfunc("sin", libfunc_sin);
	
}
int main (void){
	
	FILE *fp = openFile();
	readBinaryFile(fp);
	avm_initstack();
	
	printf("------------------------\n");
	top = topsp = (AVM_STACKSIZE- 1 )- totalGlobalVars;
	while(executionFinished == 0){
		//printf("\n\nexecutionFinished: %d", executionFinished);
		execute_cycle();
	}
}
