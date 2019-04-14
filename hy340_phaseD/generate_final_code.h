#ifndef GENERATE_FINAL_CODE_H_
#define GENERATE_FINAL_CODE_H_

#include "ExpressionTable.h"
#include "vstack.h"

#define EXPAND_SIZE_INSTR     1024
#define CURR_SIZE_INSTR       (totalInstr*sizeof(struct instruction))
#define NEW_SIZE_INSTR        (EXPAND_SIZE_INSTR*sizeof(struct instruction)+CURR_SIZE_INSTR)

extern quad*  quads;
extern unsigned currQuad;
extern FILE *fp_instr, *f;
extern unsigned programVarOffset;

/**********************************************************************************************************/
/* -------------- STRUCTURES -------------- */

typedef enum vmopcode{
        assign_v,              add_v,                 sub_v,
        mul_v,                 div_v,                 mod_v,
        uminus_v,              and_v,                 or_v,
        not_v,                 jeq_v,                 jne_v,
        jle_v,                 jge_v,                 jlt_v,
        jgt_v,                 call_v,                pusharg_v,
        ret_v,                 getretval_v,           funcenter_v,
        funcexit_v,            newtable_v,		      jump_v,
        tablegetelem_v,   	   tablesetelem_v, 		  nop_v
}vmopcode_t;

typedef enum vmargt{
		label_a    =0,
		global_a   =1,
		formal_a   =2,
		local_a    =3,
		number_a   =4,
		string_a   =5,
		bool_a     =6,
		nil_a      =7,
		userfunc_a =8,
		libfunc_a  =9,
		retval_a   =10
		
}vmarg_t;

typedef struct vmarg {
	vmarg_t type;
	unsigned val;
}vmarg;

typedef struct instruction {
	vmopcode_t opcode;
	vmarg *result;
	vmarg *arg1;
	vmarg *arg2;
	unsigned srcLine;
} instruction;

typedef struct returnlist {
	int returnValue;
	struct returnlist * next;
}returnlist;

typedef struct userfunc {
	unsigned address;
	unsigned localSize;
	char *id;
	SymbolTableEntry_t *symbol;
	returnlist *rl;
	struct userfunc * next;
} userfunc;

typedef struct incomplete_jump{
	unsigned instrNo; // The jump instruction number
	unsigned iaddress; // The i-code jump-target address.
	struct incomplete_jump* next; // A trivial linked list.
}incomplete_jump;

/**********************************************************************************************************/
/* -------------- FUNCTIONS -------------- */

/* for instruction array --same as functions from quad*/
void expand_instr (void);
void emit_instr(instruction * instr);

/* make operand from quad*/
void make_numberoperand (vmarg* arg, double val);
void make_booloperand(vmarg* arg, unsigned val);
void make_retvaloperand(vmarg* arg);
void make_operand (expr* e, vmarg* arg);

unsigned nextInstructionLabel(void);
unsigned currProcessedQuad(void);

void add_incomplete_jump(unsigned instrNo, unsigned iaddress);
void patch_incomplete_jumps(void);

/* Instruction generators */
void generate(vmopcode_t opcode ,quad* quad_);
void generate_relational(vmopcode_t opcode ,quad* quad_);

void generate_ASSIGN (quad*);
void generate_ADD (quad*);
void generate_SUB (quad*);
void generate_MUL (quad*);
void generate_DIV (quad*);
void generate_MOD (quad*);
void generate_UMINUS (quad*);
void generate_AND (quad*);
void generate_OR (quad*);
void generate_NOT (quad*);
void generate_IF_EQ (quad*);
void generate_IF_NOTEQ (quad*);
void generate_IF_LESSEREQ (quad*);
void generate_IF_GREATEREQ (quad*);
void generate_IF_LESS (quad*);
void generate_IF_GREATER (quad*);
void generate_CALL (quad*);
void generate_PARAM (quad*);
void generate_RET (quad*);
void generate_GETRETVAL (quad*);
void generate_FUNCSTART (quad*);
void generate_FUNCEND (quad*);
void generate_TABLECREATE (quad*);
void generate_JUMP (quad*);
void generate_TABLEGETELEM (quad*); 
void generate_TABLESETELEM (quad*); 
void generate_NOP (); 

/* Stack functions -- not used much || TODO? */
void pushfunc(SymbolTableEntry_t * sym);
userfunc *topfunc(void);
userfunc *popfunc(void);

/* This functions add to return list and patch the list -- not used much || TODO? */
void append( returnlist **rl, int label );
void backpatch_rl(returnlist *rl, int label);

/* Arrays of const and functions */
unsigned consts_newstring(char * s);
unsigned consts_newnumber (double n);
unsigned libfuncs_newused(char* s);
unsigned userfuncs_newfunc(SymbolTableEntry_t * sym);

/* This function generates instructions from quads*/
void generate_instructions (void);

/* Returns the name of the symbol -- arg has only type and offset */
char * lookup_all_scopes_addr (SymTable_t* symbolTable, vmarg * arg);

/* Printing functions*/
void printInstructions(void);
void printINBytes(void);

#endif
