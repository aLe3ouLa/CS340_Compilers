#include <assert.h>

#include "fun.h"
#include "alpha_vm.h"

extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern unsigned top;
extern unsigned topsp;
extern avm_memcell stack[AVM_STACKSIZE];
extern unsigned char executionFinished;
extern unsigned pc;


typedef bool (*cmp_func) (double x , double y);
typedef unsigned char (*tobool_func_t) (avm_memcell*);

char* typeStrings[] = {
	"number",
	"string",
	"bool",
	"table",
	"userfunc",
	"libfunc",
	"nil",
	"undef"
};

cmp_func comparisonFuncs[] ={
	jle_impl,
	jge_impl,
	jlt_impl,
	jgt_impl
};

tobool_func_t toboolFuncs[] = {
	number_tobool,
	string_tobool,
	bool_tobool,
	table_tobool,
	userfunc_tobool,
	libfunc_tobool,
	nil_tobool,
	undef_tobool
};


bool jle_impl(double x, double y){
	return x <= y;	
}
bool jge_impl(double x, double y){
	return x >= y;	
}
bool jlt_impl(double x, double y){
	return x < y;	
}
bool jgt_impl(double x, double y){
	return x > y;	
}

void execute_comparison (instruction* instr){
	
	//avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*) 0);
	unsigned char lv;
	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);
	
	//assert(lv && (&stack[0] <= lv && &stack[top] > lv || lv == &retval));
	assert(rv1 && rv2);
	
	if (rv1->type != number_m || rv2->type != number_m) {
		avm_error("not a number in comparison!");
		executionFinished = 1;
	}else{
		
		int opc = 0;
		if (instr->opcode == jle_v){
			opc = 0;
		}else if (instr->opcode == jge_v){
			opc = 1;
		}else if (instr->opcode == jlt_v){
			opc = 2;
		}else {
			opc = 3;
		}
		cmp_func op = comparisonFuncs[opc];
		lv = (*op)(rv1->data.numVal, rv2->data.numVal);
		
		if (lv){
			pc = instr->result->val;
		}
	}
	
}

unsigned char number_tobool (avm_memcell* m) { 
	printf("num: %lf", m->data.numVal);
	printf("bool: %d", m->data.numVal != 0);
	return m->data.numVal != 0; 
}
unsigned char string_tobool (avm_memcell* m) { 
	return m->data.strVal[0] != 0;
}
unsigned char bool_tobool (avm_memcell* m){ 
	return m->data.boolVal; 
}
unsigned char table_tobool (avm_memcell* m) { 
	return 1;
}
unsigned char userfunc_tobool (avm_memcell* m) { 
	return 1;
}
unsigned char libfunc_tobool (avm_memcell* m) { 
	return 1;
}
unsigned char nil_tobool (avm_memcell* m) { 
	return 0;
}
unsigned char undef_tobool (avm_memcell* m) { 
	assert(0); 
	return 0;
}


unsigned char avm_tobool (avm_memcell* m) {
	assert(m->type >= 0 && m->type < undef_m);
	return (*toboolFuncs[m->type]) (m);
}

void execute_JEQ (instruction* instr) {

	assert(instr->result->type == label_a);

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

	unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m)
		avm_error("'undef' involved in equality!");
	else if 	(rv1->type == nil_m || rv2->type == nil_m)
		result = rv1->type == nil_m && rv2->type == nil_m;
	else if (rv1->type == bool_m && rv2->type == bool_m)
		result = rv1->data.boolVal == rv2->data.boolVal;
	else if (rv1->type != rv2->type)
		avm_error("'Equality' is illegal between these operands");
	else if (rv1->type == number_m && rv2->type == number_m){	
			result = rv1->data.numVal == rv2->data.numVal;
	}else if(rv1->type == number_m && rv2->type != number_m){
			result = rv1->data.numVal == avm_tobool(rv2);
	}else if(rv1->type != number_m && rv2->type == number_m){
			result = avm_tobool(rv1) == rv2->data.numVal;
	}else{
		/*Equality check with dispatching.*/
		result=(avm_tobool(rv1)==avm_tobool(rv2));
		
	}

	if (!executionFinished && result){
		pc = instr->result->val;
	}

}


void execute_JNE (instruction* instr) {
	assert(instr->result->type == label_a);

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

	unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m)
		avm_error("'undef' involved in equality!");
	else if 	(rv1->type == nil_m || rv2->type == nil_m)
		result = rv1->type == nil_m && rv2->type == nil_m;
	else if (rv1->type == bool_m || rv2->type == bool_m)
		result = (avm_tobool(rv1) != avm_tobool(rv2));
	else if (rv1->type != rv2->type)
		avm_error("'Inequality' is illegal between these operands");
	else if (rv1->type == number_m && rv2->type == number_m){	
			result = rv1->data.numVal != rv2->data.numVal;
	}else if(rv1->type == number_m && rv2->type != number_m){
			result = rv1->data.numVal != avm_tobool(rv2);
	}else if(rv1->type != number_m && rv2->type == number_m){
			result = avm_tobool(rv1) != rv2->data.numVal;
	}else{
		/*Equality check with dispatching.*/
		result=(avm_tobool(rv1)!=avm_tobool(rv2));
	}

	if (!executionFinished && result)
		pc = instr->result->val;
	}

void execute_JUMP (instruction* instr){
		pc = instr->result->val;
}

