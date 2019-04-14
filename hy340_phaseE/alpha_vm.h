#ifndef ALPHA_VM_CODE_H_
#define ALPHA_VM_CODE_H_

#include "generate_final_code.h"

#define AVM_STACKSIZE 4096
#define AVM_WIPEOUT(m) memset(&(m), 0 ,sizeof(m))
#define AVM_TABLE_HASHSIZE 211

#define AVM_STACKENV_SIZE 4

#define AVM_ENDING_PC codeCounter
#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

typedef enum avm_memcell_t{
	number_m    = 0,
	string_m    = 1,
	bool_m      = 2,
	table_m     = 3,
	userfunc_m  = 4,
	libfunc_m   = 5,
	nil_m       = 6,
	undef_m     = 7
}avm_memcell_t;

typedef struct avm_table {
	unsigned refCounter;
	struct avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* boolIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* tableIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* userFuncIndexed[AVM_TABLE_HASHSIZE];
	struct avm_table_bucket* libFuncIndexed[AVM_TABLE_HASHSIZE];
	unsigned total;
}avm_table;

typedef struct avm_memcell{
	avm_memcell_t type;
	union{
		double numVal;
		char* strVal;
		unsigned char boolVal;
		avm_table* tableVal;
		unsigned funcVal;
		char* libfuncVal;
	} data;
}avm_memcell;

typedef struct avm_table_bucket{
	avm_memcell key;
	avm_memcell value;
	struct avm_table_bucket* next;
}avm_table_bucket;

static void avm_initstack(void);

avm_table*  	avm_tablenew(void);
void 			avm_tabledestroy(avm_table* t);
//avm_memcell* 	avm_tablegetelem (avm_memcell* key);
//void 			avm_tablesetelem(avm_memcell* key, avm_memcell* value);

void 			avm_tableincrefcounter(avm_table* t);
void 			avm_tabledecrefcounter(avm_table* t);
void 			avm_tablebucketsinit(avm_table_bucket** p);
void 			avm_tablebucketsdestroy(avm_table_bucket** p);

void 			avm_memcellclear(avm_memcell* m);

double 	 		consts_getnumber (unsigned index);
char*    		consts_getstring (unsigned index);
char*    		libfuncs_getused (unsigned index);
avm_memcell*    avm_translate_operand (vmarg* arg, avm_memcell* reg);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);


#endif


