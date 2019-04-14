CC = gcc
TARGET = al
RM = rm 

yacc: 
	bison -v  --yacc --defines --output=parser.c parser.y
flex: 
	flex --outfile=$(TARGET).c $(TARGET).l

all: $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c  parser.c symtable.c


clean: 
	$(RM) $(TARGET)  $(TARGET).c  
