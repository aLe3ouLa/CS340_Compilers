#ifndef FUNCS_CODE_H_
#define FUNCS_CODE_H_

#include "alpha_vm.h"

#define execute_ADD execute_arithmetic
#define execute_SUB execute_arithmetic
#define execute_MUL execute_arithmetic
#define execute_DIV execute_arithmetic
#define execute_MOD execute_arithmetic

#define execute_JLE execute_comparison
#define execute_JGE execute_comparison
#define execute_JLT execute_comparison
#define execute_JGT execute_comparison

void execute_ASSIGN (instruction*);
void execute_ADD (instruction*);
void execute_SUB (instruction*);
void execute_MUL (instruction*);
void execute_DIV (instruction*);
void execute_MOD (instruction*);
void execute_UMINUS (instruction*);
void execute_AND (instruction*);
void execute_OR (instruction*);
void execute_NOT (instruction*);
void execute_JEQ (instruction*);
void execute_JNE (instruction*);
void execute_JLE (instruction*);
void execute_JGE (instruction*);
void execute_JLT (instruction*);
void execute_JGT (instruction*);
void execute_CALL (instruction*);
void execute_PUSHARG (instruction*);
void execute_RET (instruction*);
void execute_GETRETVAL (instruction*);
void execute_FUNCENTER (instruction*);
void execute_FUNCEXIT (instruction*);
void execute_JUMP (instruction*);
void execute_NEWTABLE (instruction*);
void execute_TABLEGETELEM (instruction*); 
void execute_TABLESETELEM (instruction*); 
void execute_NOP (); 

void execute_arithmetic (instruction* instr);

double add_impl(double x, double y);
double sub_impl(double x, double y);
double mul_impl(double x, double y);
double div_impl(double x, double y);
double mod_impl(double x, double y);

void execute_comparison (instruction* instr);

bool jle_impl(double x, double y);
bool jge_impl(double x, double y);
bool jlt_impl(double x, double y);
bool jgt_impl(double x, double y);

unsigned char number_tobool (avm_memcell* m);
unsigned char string_tobool (avm_memcell* m);
unsigned char bool_tobool (avm_memcell* m);
unsigned char table_tobool (avm_memcell* m);
unsigned char userfunc_tobool (avm_memcell* m);
unsigned char libfunc_tobool (avm_memcell* m);
unsigned char nil_tobool (avm_memcell* m);
unsigned char undef_tobool (avm_memcell* m);
unsigned char undef_tobool (avm_memcell* m);


void avm_calllibfunc(char* id);

void libfunc_print(void);
void libfunc_input(void);
void libfunc_objectmemberkeys(void);
void libfunc_objecttotalmembers(void);
void libfunc_objectcopy(void);
void libfunc_totalarguments(void);
void libfunc_argument(void);
void libfunc_typeof(void);
void libfunc_strtonum(void);
void libfunc_sqrt(void);
void libfunc_cos(void);
void libfunc_sin(void);

char * avm_tostring (avm_memcell * m);

char* number_tostring (avm_memcell* m);
char* string_tostring (avm_memcell* m);
char* bool_tostring (avm_memcell* m);
char* table_tostring (avm_memcell* m);
char* userfunc_tostring (avm_memcell* m);
char* libfunc_tostring (avm_memcell* m);
char* nil_tostring (avm_memcell* m);
char* undef_tostring (avm_memcell* m);


#endif
