CC = gcc
TARGET = al
RM = rm 

yacc: 
	bison -v  --yacc --defines --output=parser.c parser.y
flex: 
	flex --outfile=$(TARGET).c $(TARGET).l
avm: 	alpha_vm.c
	gcc -o vm alpha_vm.c arithm_impl.c  assign_impl.c jump_impl.c  func_impl.c  table_impl.c -lm

all: $(TARGET).c
	$(CC) -g -o $(TARGET) $(TARGET).c  parser.c symtable.c ExpressionTable.c stack.c list.c vlist.c generate_final_code.c vstack.c

clean: 
	$(RM) $(TARGET)  $(TARGET).c  instr.bin instructions.txt quads.txt parser.c parser.h parser.output vm func_impl.o
