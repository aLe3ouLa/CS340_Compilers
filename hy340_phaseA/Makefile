CC = gcc
TARGET = al
RM = rm 

flex: 
	flex --outfile=$(TARGET).c al.l

all: $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c 


clean: 
	$(RM) $(TARGET)  $(TARGET).c  
