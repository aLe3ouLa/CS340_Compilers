#include "generate_final_code.h"


unsigned int    nextInstr = 0;
unsigned 	    totalInstr = 0;
unsigned int    currInstr = 0;
unsigned int 	procQuad = 0;
instruction* instractions = (instruction*) 0;

incomplete_jump* ij_head = (incomplete_jump*)0;
unsigned ij_total = 0;

extern vstack_t strStack;
unsigned  totalStringConsts =0;
extern vstack_t numStack;
unsigned  totalNumConsts =0;
extern vstack_t userFuncStack;
unsigned  totalUserFuncs =0;
extern vstack_t libFuncStack;
unsigned  totalNamedLibFuncs = 0;
extern SymTable_t * mySymbolTable;
userfunc *funcStack = NULL;

char *vmopcode_str[]= { "assign", "add", "sub", "mul", "div", "mod", "uminus",
					  "and", "or", "not", "jeq", "jne", "jle", "jge","jlt",
					  "jgt","call","pusharg", "ret", "getretval","funcenter","funcexit","newtable",
					  "jump","tablegetelem","tablesetelem", "nop"};
					  
char * vmarg_t_str[]= { "label_a", "global_a", "formal_a", "local_a", "number_a", "string_a", "bool_a", "nil_a", "userfc_a", "libfc_a", "retval_a"};


void pushfunc(SymbolTableEntry_t * sym){
		userfunc *newNode = malloc(sizeof(userfunc));
		newNode->id = sym->name;
		newNode->address = sym->iaddress;
		newNode->localSize = sym->totallocals;
		newNode->symbol = sym;
		if (funcStack == NULL){
			newNode->next = NULL;
		}else{
			newNode->next = funcStack;
		}
		funcStack = newNode;
}	

userfunc * topfunc(void){
		return funcStack;
}

userfunc *popfunc(void){
		userfunc * temp = funcStack;
		funcStack = temp->next;
		return temp;
}
	
typedef void (*generator_func_t) (quad*);       

generator_func_t generators[] = {
	generate_ASSIGN,
	generate_ADD,
	generate_SUB,
	generate_MUL,
	generate_DIV,
	generate_MOD,
	generate_UMINUS,
	generate_AND,
	generate_OR,
	generate_NOT,
	generate_IF_EQ,
	generate_IF_NOTEQ,
	generate_IF_LESSEREQ,
	generate_IF_GREATEREQ,
	generate_IF_LESS,
	generate_IF_GREATER,
	generate_CALL,
	generate_PARAM,
	generate_RET,
	generate_GETRETVAL,
	generate_FUNCSTART,
	generate_FUNCEND,
	generate_TABLECREATE,
	generate_JUMP,
	generate_TABLEGETELEM, 
	generate_TABLESETELEM,
	generate_NOP   
 };
 
/************************/
  
void expand_instr (void){
	
	/* If we have reach the current size expand the array */
	instruction* newInstruction = (instruction*)malloc(NEW_SIZE_INSTR);
		
	if (newInstruction == NULL){
		fprintf(stderr, "Cannot allocate space for instructions.\n");
	    exit(EXIT_FAILURE);
	}
	
	if (instractions){
		/* Copy the old array to new one*/
		memcpy(newInstruction, instractions, CURR_SIZE_INSTR);
		/* Free the old array*/
		free(instractions);
	}
	/* Make quads the array again*/
	instractions = newInstruction;
	/* Update the total */
	totalInstr += EXPAND_SIZE_INSTR;
	
	}
void emit_instr(instruction * instr){
	
	if(currInstr == totalInstr) { 
		expand_instr();
	}
	
	instruction* newInstruction = instractions + currInstr++;
	
	newInstruction->opcode  = instr->opcode;
	newInstruction->arg1    = instr->arg1;
	newInstruction->arg2    = instr->arg2;
	newInstruction->result  = instr->result;
	newInstruction->srcLine = instr->srcLine;
	
	
	}

/************************/
 
unsigned nextInstructionLabel(void){
	return currInstr;	
}
unsigned currProcessedQuad(void){
	return procQuad;
}
 
/************************/ 
void make_operand (expr* e, vmarg* arg){
	 if( e == NULL) return;
	 
	// printf("makeop: %d\n", e->type);
	 switch(e->type){
		 /* ALl those below use a variable for storage */
			case var_e       :
			case assignexpr_e:
			case tableitem_e :
			case arithexpr_e :
			case boolexpr_e  :
			case newtable_e  :  {
				assert(e->sym);
				arg->val = e->sym->offset;
				
				switch(e->sym->space){
						case programvar    : arg->type = global_a; break;
						case functionlocal : arg->type = local_a; break;
						case formalarg	   : arg->type = formal_a; break;
						default            : assert(0);
				}
				break; /* From case newtable_e */
			}
			/* Constants */
			case constbool_e: {
				arg->val = e->boolConst;
				arg->type = bool_a;
				break;
			}
			
			case conststring_e: {
				arg->val = consts_newstring(e->strConst);
				arg->type = string_a;
				break;
			}
			
			case constnum_e: {
				arg->val = consts_newnumber(e->numConst);
				arg->type = number_a;
				break;
			}
			
			case nil_e :{
				arg->type = nil_a;
				break;
			}
			
			case programfunc_e : {
				arg->val = userfuncs_newfunc(e->sym);
				// arg->val = e->sym->iaddress;
				arg->type = userfunc_a;
				break;
			}
			
			case libraryfunc_e : {
					arg->val = libfuncs_newused(e->sym->name);
					arg->type = libfunc_a;
					break;
			}
			
			default: assert(0);
	 
	}
}
void make_numberoperand (vmarg* arg, double val){
	 arg->val = consts_newnumber(val);
	 arg->type = number_a;
}
void make_booloperand(vmarg* arg, unsigned val){
	arg->val = val;
	arg->type = bool_a;
}
void make_retvaloperand(vmarg* arg){
	arg->val = 0;
	arg->type = retval_a;	
}

/************************/

void add_incomplete_jump(unsigned instrNo, unsigned iaddress){
	incomplete_jump *incJump = malloc(sizeof(incomplete_jump));
	ij_total++;
	incJump->instrNo = instrNo;
	incJump->iaddress = iaddress;
	if (ij_head == NULL){
		incJump->next = NULL;
	}else{
		incJump->next = ij_head;
	}
	
	ij_head = incJump;
}

void patch_incomplete_jumps(void){
	incomplete_jump * temp = ij_head;
	
	while (temp!=NULL){
		if (temp->iaddress == nextquadlabel()){
			(instractions[temp->instrNo].result)->val = nextInstructionLabel();
		}else{
			(instractions[temp->instrNo].result)->val = quads[temp->iaddress].taddress;
		}
		temp = temp->next;
	}

}

/************************/
  
void generate(vmopcode_t opcode ,quad* quad_){
	 
	 instruction *tempInstr = malloc(sizeof(instruction));
	 tempInstr->result = malloc(sizeof(vmarg));
	 tempInstr->arg1 = malloc(sizeof(vmarg));
	 tempInstr->arg2 = malloc(sizeof(vmarg));
	 
	 
	 
	 tempInstr->opcode = opcode;
	 tempInstr->srcLine = quad_->line;
	 	
	// printf("INSTR: opcode: %d, srcline: %d\n", tempInstr->opcode ,tempInstr->srcLine );
	 
	 if (quad_->result!= NULL){
		make_operand(quad_->result, tempInstr->result);	
	 }else{
		tempInstr->result = NULL;
		}
	 if (quad_->arg1!= NULL){
		make_operand(quad_->arg1, tempInstr->arg1);
	 }else{
		tempInstr->arg1 = NULL;
		}
	 if (quad_->arg2 != NULL){
		make_operand(quad_->arg2, tempInstr->arg2);
	 }else{
		tempInstr->arg2 = NULL;
		}
	 quad_->taddress = nextInstructionLabel();
	 emit_instr(tempInstr);
} 
void generate_relational(vmopcode_t opcode ,quad* quad_){
	 
	 instruction *tempInstr = malloc(sizeof(instruction));
	 tempInstr->result = malloc(sizeof(vmarg));
	 tempInstr->arg1 = malloc(sizeof(vmarg));
	 tempInstr->arg2 = malloc(sizeof(vmarg));
	 
	 tempInstr->opcode = opcode;
	 tempInstr->srcLine = quad_->line;
	 
	 if (quad_->arg1 != NULL){		
		make_operand(quad_->arg1, tempInstr->arg1);
	 }else{
		 tempInstr->arg1 = NULL;
	 }
	 
	 if (quad_->arg2 != NULL){
		make_operand(quad_->arg2, tempInstr->arg2);
	 }else{
		 tempInstr->arg2 = NULL;
	 }
	 
	 tempInstr->result->type = label_a;
	 
	 //printf("quad_->label: %d, currProcessedQuad(): %d\n",quad_->label, currProcessedQuad() );
	 if (quad_->label < currProcessedQuad()){
		// printf("quads[quad_->label].taddress: %d \n", quads[quad_->label].taddress);
		tempInstr->result->val = quads[quad_->label].taddress;
	 }else{
		// printf(" quad_->label): %d nextinst: %d, currProcessedQuad(): %d 	\n",quad_->label, nextInstructionLabel(), currProcessedQuad() );
		 add_incomplete_jump(nextInstructionLabel(), quad_->label);	
	 }
	 
	 quad_->taddress = nextInstructionLabel();
	 emit_instr(tempInstr);
} 

/************************/

void generate_ADD (quad* quad_){
	generate(add_v, quad_);
}
void generate_SUB (quad* quad_){
	generate(sub_v, quad_);
}
void generate_MUL (quad* quad_){
	generate(mul_v, quad_);
}
void generate_DIV (quad* quad_){
	generate(div_v, quad_);
}
void generate_MOD (quad* quad_){
	generate(mod_v, quad_);
}
void generate_UMINUS(quad* currentQuad){
	
	/* multiply  num * (-1)*/
	currentQuad->taddress = nextInstructionLabel();
	instruction* tempInstr;
	tempInstr = malloc(sizeof(instruction));
	
	tempInstr->opcode = mul_v; /* change uminus with mul */
	tempInstr->srcLine = currentQuad->line;
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));

	if (currentQuad->result!= NULL){
		make_operand(currentQuad->result, tempInstr->result);	
	}else{
		tempInstr->result = NULL;
	}
	
	if (currentQuad->arg1!= NULL){
		make_operand(currentQuad->arg1, tempInstr->arg1);
	}else{
		tempInstr->arg1 = NULL;
	}
	 
	make_numberoperand(tempInstr->arg2, -1);
	 
	
	emit_instr(tempInstr);	
}

void generate_TABLECREATE (quad* quad_){
	generate(newtable_v, quad_);
}
void generate_TABLEGETELEM (quad* quad_){
	generate(tablegetelem_v, quad_);
}
void generate_TABLESETELEM (quad* quad_){
	generate(tablesetelem_v, quad_);
}

void generate_ASSIGN (quad* quad_){
	generate(assign_v, quad_);
}

void generate_NOP (){
	instruction* tempInstr;
	tempInstr->opcode = nop_v;
	emit_instr(tempInstr);
}

void generate_JUMP (quad* quad_){
	generate_relational(jump_v, quad_);
}
void generate_IF_EQ (quad* quad_){
	generate_relational(jeq_v, quad_);
}
void generate_IF_NOTEQ (quad* quad_){
	generate_relational(jne_v, quad_);
}
void generate_IF_LESSEREQ (quad* quad_){
	generate_relational(jle_v, quad_);
}
void generate_IF_GREATEREQ (quad* quad_){
	generate_relational(jge_v, quad_);
}
void generate_IF_LESS (quad* quad_){
	generate_relational(jlt_v, quad_);
}
void generate_IF_GREATER (quad* quad_){
	generate_relational(jgt_v, quad_);
}

void generate_NOT (quad* quad_){
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->srcLine = quad_->line;
	
	tempInstr->opcode = jeq_v;
	if(quad_->arg1 != NULL){
		make_operand(quad_->arg1, tempInstr->arg1);
	}else{
		tempInstr->arg1 = NULL;
	}
	make_booloperand(tempInstr->arg2, 0);
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 3;
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	
	make_booloperand(tempInstr->arg1, 0);
	tempInstr->arg2 = NULL;
	
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jump_v;
	tempInstr->arg1 = NULL;
	tempInstr->arg2 = NULL;
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 2;
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	make_booloperand(tempInstr->arg1,  1);
	tempInstr->arg2 = NULL;
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	emit_instr(tempInstr);
	
}
void generate_OR (quad* quad_){
	
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jeq_v;
	tempInstr->srcLine = quad_->line;
	
	if(quad_->arg1 != NULL){
		make_operand(quad_->arg1, tempInstr->arg1);
	}else{
		tempInstr->arg1 = NULL;
	}
	make_booloperand(tempInstr->arg2, 1);
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 4;
	emit_instr(tempInstr);
	
	tempInstr->arg1 = malloc(sizeof(vmarg));
	
	if(quad_->arg2 != NULL){
		make_operand(quad_->arg2, tempInstr->arg1);
	}else{
		tempInstr->arg2 = NULL;
	}
	
	tempInstr->result->val = nextInstructionLabel() + 3;
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	make_booloperand(tempInstr->arg1,  0);
	tempInstr->arg2 = NULL;
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jump_v;
	tempInstr->arg1 =NULL;
	tempInstr->arg2 = NULL;
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 2;
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	make_booloperand(tempInstr->arg1,  1);
	tempInstr->arg2 = NULL;
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	emit_instr(tempInstr);
}
void generate_AND (quad* quad_){

	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jeq_v;
	tempInstr->srcLine = quad_->line;
	
	if(quad_->arg1 != NULL){
		make_operand(quad_->arg1, tempInstr->arg1);
	}else{
		tempInstr->arg1 = NULL;
	}
	make_booloperand(tempInstr->arg2, 0);
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 4;
	emit_instr(tempInstr);
	
	
	tempInstr->arg1 = malloc(sizeof(vmarg));
	
	if(quad_->arg2 != NULL){
		make_operand(quad_->arg2, tempInstr->arg1);
	}else{
		tempInstr->arg2 = NULL;
	}
	
	tempInstr->result->val = nextInstructionLabel() + 3;
	emit_instr(tempInstr);
	
	
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	make_booloperand(tempInstr->arg1,  1);
	tempInstr->arg2 = NULL;
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jump_v;
	tempInstr->arg1 =NULL;
	tempInstr->arg2 = NULL;
	tempInstr->result->type = label_a;
	tempInstr->result->val = nextInstructionLabel() + 2;
	emit_instr(tempInstr);
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = assign_v;
	make_booloperand(tempInstr->arg1,  0);
	tempInstr->arg2 = NULL;
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	emit_instr(tempInstr);
	
	
}

void generate_PARAM (quad* quad_){
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = NULL;
	tempInstr->arg2 = NULL;
	
	tempInstr->opcode = pusharg_v;
	tempInstr->srcLine = quad_->line;
	
	make_operand(quad_->result, tempInstr->result);
	
	emit_instr(tempInstr);	
}
void generate_CALL(quad* quad_){
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = NULL;
	tempInstr->arg2 = NULL;
	
	tempInstr->opcode = call_v;
	tempInstr->srcLine = quad_->line;
	
	make_operand(quad_->result , tempInstr->result);	
	emit_instr(tempInstr);
}
void generate_GETRETVAL(quad* quad_){
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = NULL;
	
	tempInstr->opcode = assign_v;
	tempInstr->srcLine = quad_->line;
	make_operand(quad_->result, tempInstr->result);
	make_retvaloperand(tempInstr->arg1);
	emit_instr(tempInstr);
}
void generate_FUNCSTART (quad* quad_){
	
		quad_->taddress = nextInstructionLabel();
		SymbolTableEntry_t * f = quad_->result->sym;
		pushfunc(f);
		
		//f->taddress = nextinstructionlabel();
		//userfunctions.add(f->id, f->iaddress, f->totallocals);
		
		instruction* tempInstr = malloc(sizeof(instruction));
		tempInstr->result = malloc(sizeof(vmarg));
		tempInstr->arg1 = NULL;
		tempInstr->arg2 = NULL;
		
		tempInstr->opcode = funcenter_v;
		tempInstr->srcLine = quad_->line;
		
		make_operand(quad_->result, tempInstr->result);
		emit_instr(tempInstr);
}
void generate_RET(quad* quad_){
	quad_->taddress = nextInstructionLabel();
	instruction* tempInstr = malloc(sizeof(instruction));
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->srcLine = quad_->line;
	tempInstr->opcode = assign_v;
	
	if(quad_->result != NULL){
		make_operand(quad_->result, tempInstr->result);
	}else{
		tempInstr->result = NULL;
	}
	make_retvaloperand(tempInstr->arg1);
	tempInstr->arg2 = NULL;
	
	emit_instr(tempInstr);
	
	userfunc *f = topfunc();
	append(&(f->rl), nextInstructionLabel());
	
	tempInstr->result = malloc(sizeof(vmarg));
	tempInstr->arg1 = malloc(sizeof(vmarg));
	tempInstr->arg2 = malloc(sizeof(vmarg));
	
	tempInstr->opcode = jump;
	tempInstr->arg1 = NULL;
	tempInstr->arg2 = NULL;
	tempInstr->result->type = label_a;
	emit_instr(tempInstr);	
}
void generate_FUNCEND(quad* quad_){
		userfunc * f = popfunc();
		backpatch_rl(f->rl, nextInstructionLabel());
		
		quad_->taddress = nextInstructionLabel();
		instruction* tempInstr = malloc(sizeof(instruction));
		tempInstr->result = malloc(sizeof(vmarg));
		tempInstr->arg1 = NULL;
		tempInstr->arg2 = NULL;
		
		tempInstr->opcode = funcexit_v;
		tempInstr->srcLine = quad_->line;
		
		if(quad_->result != NULL){
			make_operand(quad_->result, tempInstr->result);
		}else{
			tempInstr->result = NULL;
		}
		emit_instr(tempInstr);
}

void backpatch_rl(returnlist *rl, int label){
   returnlist *temp = rl;
   while(temp != NULL){
		(instractions[temp->returnValue]).result->val = label;
      temp = temp->next;
   }
}
void append( returnlist **rl, int label ){
	
	returnlist *tmp = malloc(sizeof(returnlist));
	tmp->returnValue = label;

	if( *rl == NULL ){
		tmp->next = NULL;
	}
	else{
		tmp->next = *rl;
	}
	*rl = tmp;
}

unsigned consts_newstring(char * s){
	stack_append(&strStack, &s, totalStringConsts);
	return totalStringConsts++;
}
unsigned consts_newnumber (double n){
	stack_append(&numStack, &n, totalNumConsts);
	return totalNumConsts++;
}
unsigned libfuncs_newused(char* s){
	stack_append(&libFuncStack, &s, totalNamedLibFuncs);
	return totalNamedLibFuncs++;
}
unsigned userfuncs_newfunc(SymbolTableEntry_t * sym){
	stack_append(&userFuncStack, &sym, totalUserFuncs);
	totalUserFuncs++;
	return sym->iaddress;
}
 

void generate_instructions (void) {
	 int i;
	 printf("##generate instructions\n");
	 printf("##currQuad == %d\n\n", currQuad);
	 
	 for (i = 0; i < currQuad; i++){
		 procQuad = i;
		// printf("quads[i].op: %d\n", quads[i].op);
		 (*generators[quads[i].op])(quads + i);
	 }
	
	patch_incomplete_jumps();
}

/* Iterators for the stacks/lists */
bool_ iterate_double(void *data, int i){
    // printf("Found value: %lf\n", *(double *)data);
     fprintf(fp_instr,"%d: %lf\n",i, *(double *)data);
  return T;
     
 }
bool_ iterate_double_binary(void *data, int i){
	//fwrite(&i,sizeof(int),1,fp_binary);
	double * db = malloc(sizeof(double));
	db = (double*) data;
    fwrite(db, sizeof(double), 1, f);
	return T;
 }
bool_ iterate_string(void *data, int i){
  printf("Found string value: %s\n", *(char **)data);
  fprintf(fp_instr,"%d: %s\n", i, *(char **)data);
  return T;
}
bool_ iterate_string_binary(void *data, int i){
	
	/*string -> size char* */
	char * str = *(char **)data;
	unsigned int length = strlen(str);
	//printf("BYTE: %d %s\n",length, str);
	fwrite(&length,sizeof(unsigned int),1,f);
    fwrite(str, length * sizeof(char), 1, f);
	return T;
}
bool_ iterate_symbol(void *data, int i){
  //printf("Found symbol value: %s\n", ((SymbolTableEntry_t *)data)->name);
  fprintf(fp_instr,"%d: %s\n", i, (*(SymbolTableEntry_t **)data)->name);
  return T;
}
bool_ iterate_symbol_binary(void *data, int i){
  
  SymbolTableEntry_t * temp =  *(SymbolTableEntry_t **)data;
  unsigned int address = temp->iaddress;
  unsigned int localsize = temp->totallocals;
  char * name = temp->name;
  unsigned int length = strlen(name);
	printf("%d %d %s\n", address, localsize, name);
	fwrite(&address,sizeof(unsigned int),1,f);
	fwrite(&localsize,sizeof(unsigned int),1,f);
    fwrite(name, length * sizeof(char), 1, f);
  return T;
}
 

char * lookup_all_scopes_addr (SymTable_t* symbolTable, vmarg * arg){
    int i = 0;
    bool flag = false;
    SymbolTableEntry_t* temp;
    int off = arg->val;
    int type = arg->type;
    
    printf("type: %d off: %d\n", type, off);
    for (i = 0; i < SYMBOL_TABLE_SIZE; i++){
        temp = symbolTable->hash[i];
            while (temp != NULL){
				
				if (type == 8 && temp->type == USERFUNC){
					printf("name: %s, off: %d space: %d type: %d iaddr: %d\n", temp->name, temp->offset, temp->space, temp->type, temp->iaddress);
					if (off == temp->iaddress)
						return temp->name;
				}
						
				if (type == 9 && temp->type == LIBFUNC){
					if (off == temp->iaddress)
						return temp->name;
				}
              
                if (temp->offset == off){
					
						if (type == 1 && temp->type == GLOBALVAR){
							/* global VAR*/
							return temp->name;
						}
						if (type == 2 && (temp->type == FORMAL || temp->space == formalarg)){
							/* formal VAR*/
							return temp->name;
						}
						if (type == 3 && temp->type == LOCALVAR){
							return temp->name;
						}
		
				}
                temp = temp->next;
            }
    }
    
    return NULL;
    
}


void printInstructions(void){
	
	int i = 0;
	int magicNumber = 340200501;
	int prVarsCounter = programVarOffset;
	int strCounter = totalStringConsts;
	int numCounter = totalNumConsts;
	int libCounter = totalNamedLibFuncs;
	int userCounter = totalUserFuncs;
	char temp[] = "(not used)";
	char res[] = "Result";
	char op[] = "VMopcode";
	char a1[] = "Argument 1";
	char a2[] = "Argument 2";
	fprintf(fp_instr,"MagicNumber: %d\n", magicNumber);
	fprintf(fp_instr,"---------------\n");
	fprintf(fp_instr,"Total Number Consts: %d\n", numCounter);
	fprintf(fp_instr,"-----------------------\n\n");
	stack_for_each(&numStack, iterate_double);
    
	fprintf(fp_instr,"Total String Consts: %d\n", strCounter);
	fprintf(fp_instr,"-----------------------\n\n");
	stack_for_each(&strStack, iterate_string);
   
    fprintf(fp_instr,"Total Library Functions: %d\n", libCounter);
    fprintf(fp_instr,"---------------------------\n\n");
    stack_for_each(&libFuncStack, iterate_string);
    
    fprintf(fp_instr,"Total User Functions: %d\n", userCounter);
    fprintf(fp_instr,"------------------------\n\n");
    stack_for_each(&userFuncStack, iterate_symbol);
    
    
	fprintf(fp_instr," ##  %14s   %30s%30s%30s    Line\n\n", op,res, a1, a2);
	for (i = 0; i< currInstr; i++){
			fprintf(fp_instr, "%3d: %14s", i, vmopcode_str[instractions[i].opcode]);
		
			if(instractions[i].result != NULL){
				
				if ((instractions[i].result)->type == 1 || 
					(instractions[i].result)->type == 2 || 
					(instractions[i].result)->type == 3 || 
					(instractions[i].result)->type == 8 ||
					(instractions[i].result)->type == 9  ){
				char * name = lookup_all_scopes_addr(mySymbolTable, instractions[i].result);
				//fprintf(fp_instr,"%s\t",  name);
				/* Symbol Table elements */		
					fprintf(fp_instr,"%20s[%8s,%2d]", name, vmarg_t_str[(instractions[i].result)->type], (instractions[i].result)->val);	
				}else{
					/* Something from const arrays */
					int k = 0;
					for (;k<20; k++)
						fprintf(fp_instr, " ");
					fprintf(fp_instr,"[%8s,%2d]",  vmarg_t_str[(instractions[i].result)->type], (instractions[i].result)->val);
				}
				
			}else{
				fprintf(fp_instr,"%32s", temp);
			}
			
			
			if(instractions[i].arg1 != NULL){
				if ((instractions[i].arg1)->type == 1 || 
					(instractions[i].arg1)->type == 2 || 
					(instractions[i].arg1)->type == 3 || 
					(instractions[i].arg1)->type == 8 ||
					(instractions[i].arg1)->type == 9  ){
				char * name = lookup_all_scopes_addr(mySymbolTable, instractions[i].arg1);
				//fprintf(fp_instr,"%s\t",  name);
				/* Symbol Table elements */		
					fprintf(fp_instr,"%20s[%8s,%2d]", name, vmarg_t_str[(instractions[i].arg1)->type], (instractions[i].arg1)->val);	
				}else{
					/* Something from const arrays */
					int j = 0;
					for (;j<20; j++)
						fprintf(fp_instr, " ");
						
					fprintf(fp_instr,"[%8s,%2d]",  vmarg_t_str[(instractions[i].arg1)->type], (instractions[i].arg1)->val);
				}
			}else{
				fprintf(fp_instr,"%32s", temp);
			}
			
			if(instractions[i].arg2 != NULL){
				if ((instractions[i].arg2)->type == 1 || 
					(instractions[i].arg2)->type == 2 || 
					(instractions[i].arg2)->type == 3 || 
					(instractions[i].arg2)->type == 8 ||
					(instractions[i].arg2)->type == 9  ){
				char * name = lookup_all_scopes_addr(mySymbolTable, instractions[i].arg2);
				//fprintf(fp_instr,"%s\t",  name);
				/* Symbol Table elements */		
					fprintf(fp_instr,"%20s[%8s,%2d]", name, vmarg_t_str[(instractions[i].arg2)->type], (instractions[i].arg2)->val);	
				}else{
					/* Something from const arrays */
					int j = 0;
					for (;j<20; j++)
						fprintf(fp_instr, " ");
						
					fprintf(fp_instr,"[%8s,%2d]",  vmarg_t_str[(instractions[i].arg2)->type], (instractions[i].arg2)->val);
				}
			}else{
				fprintf(fp_instr,"%32s", temp);
			}
			
		fprintf(fp_instr, "\t%10d\n", instractions[i].srcLine);
		
		
	}
	
	fclose(fp_instr);
	
}
void printINBytes(void){
	
	unsigned int magicNumber = 340200501;
	int prVarsCounter = programVarOffset;
	unsigned int strCounter = totalStringConsts;
	unsigned int numCounter = totalNumConsts;
	unsigned int libCounter = totalNamedLibFuncs;
	unsigned int userCounter = totalUserFuncs;
	
	printf("Typing in binary file..");
	
	/*avmbinaryfile -> magicnumber arrays code*/
	
	/* magicnumber -> 340200501 (unsigned int)*/
	unsigned int * mn = malloc (sizeof (unsigned int));
	mn = &magicNumber;
	
	fwrite(mn,sizeof(unsigned int),1,f);
	
	/*arrays -> strings numbers userfunctions libfunctions */
	
	/* strings -> total (string)* */
	/* total -> unsigned int */
	unsigned int * sc = malloc (sizeof (unsigned int));
	sc = &strCounter;
	fwrite(sc,sizeof(unsigned int),1,f);
	
	/* string -> size (char*)* */
	stack_for_each(&strStack, iterate_string_binary);
	
	/* numbers -> total (double)* */
	unsigned int * nc = malloc (sizeof (unsigned int));
	nc = &numCounter;
	fwrite(nc,sizeof(unsigned int),1,f);
	stack_for_each(&numStack, iterate_double_binary);
	
	/* userfunctions -> total (userfunc)* */
	unsigned int * uc = malloc (sizeof (unsigned int));
	uc = &userCounter;
	fwrite(uc,sizeof(unsigned int),1,f);
	/* userfunc -> address localsize id */
	stack_for_each(&userFuncStack, iterate_symbol_binary);
	
	/* libfunctions -> strings */
	/* strings -> total (string)* */
	/* total -> unsigned int */
	unsigned int * lc = malloc (sizeof (unsigned int));
	lc = &libCounter;
	fwrite(lc,sizeof(unsigned int),1,f);
	/* string -> size (char*)* */
	stack_for_each(&libFuncStack, iterate_string_binary);
	
	/* code -> total (instruction)* */
	unsigned int * codeCounter = malloc (sizeof (unsigned int));
	int ci = currInstr -1;
	codeCounter = &ci;
	fwrite(codeCounter,sizeof(unsigned int),1,f);
	
	int i = 0;
	for (i = 0; i< currInstr; i++){
		int op = instractions[i].opcode;
		fwrite(&op, sizeof(int), 1 , f);
		
			if(instractions[i].result != NULL){
				int type = (instractions[i].result)->type;
				unsigned int val = (instractions[i].result)->val;
				fwrite(&type, sizeof(int), 1, f);
				fwrite(&val, sizeof(unsigned int), 1, f);
			}
			
			if(instractions[i].arg1 != NULL){
				int type = (instractions[i].arg1)->type;
				unsigned int val = (instractions[i].arg1)->val;
				fwrite(&type, sizeof(int), 1, f);
				fwrite(&val, sizeof(unsigned int), 1, f);
			}
			
			if(instractions[i].arg2 != NULL){
				int type = (instractions[i].arg2)->type;
				unsigned int val = (instractions[i].arg2)->val;
				fwrite(&type, sizeof(int), 1, f);
				fwrite(&val, sizeof(unsigned int), 1, f);
			}
		
	}
	
	fclose(f);
	
}
