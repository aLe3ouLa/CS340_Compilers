#include <assert.h>

#include "fun.h"
#include "alpha_vm.h"

extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern unsigned top;
extern unsigned topsp;
extern avm_memcell stack[AVM_STACKSIZE];
extern unsigned char executionFinished;


void avm_assign (avm_memcell* lv, avm_memcell* rv){
		
		/* Same cells? Destructive to assign!*/
		if (lv == rv)
			return;
			
		/* Same tables? No need to assign */	
		if (lv->type == table_m &&
			rv->type == table_m &&
			lv->data.tableVal == rv->data.tableVal)
			return;	
		
			
		/* From undefined r-value? warn! */	
		if (rv->type == undef_m)
				avm_warning("assigning from \'undef\' content!\n");
				
		/* Clear old cell contents*/		
		avm_memcellclear(lv);
		/* Copy rv to lv*/
		memcpy(lv, rv, sizeof(avm_memcell));	
	
		/* Now take care of copied values of ref counter */
		if (lv->type == string_m){
			lv->data.strVal = strdup(rv->data.strVal);
		} else if (lv->type == table_m){
			avm_tableincrefcounter(lv->data.tableVal);
		}else if (lv->type == number_m){
			//printf("lv->data.numVal: %lf\n",lv->data.numVal);
			lv->data.numVal = rv->data.numVal;
			//printf("lv->data.numVal: %lf\n",lv->data.numVal);
		}else if (lv->type == bool_m){
			lv->data.boolVal = rv->data.boolVal;
		}else if (lv->type == userfunc_m){
			//printf("assign userfunc\n");
			lv->type = userfunc_m;
			rv->type = userfunc_m;
			//printf("lv->vall: %d\n", lv->data.funcVal);
			lv->data.funcVal = rv->data.funcVal;
		}else if (lv->type == libfunc_m){
			lv->data.libfuncVal = rv->data.libfuncVal;
		}else{
			printf("nil");
		}			
	
}

void execute_ASSIGN(instruction* instr){
		avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*)0);
		avm_memcell* rv = avm_translate_operand(instr->arg1, &ax);
		
		
		//assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv >= &stack[top] || lv == &retval));
		assert(rv);
		
		avm_assign(lv, rv);
}
