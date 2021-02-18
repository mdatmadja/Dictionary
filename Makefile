#Matthew Atmadja
#matmadja
#PA5


CLIENT          = Lex
ADT1            = Dictionary
SOURCE          = $(CLIENT).c
OBJECT          = $(CLIENT).o
ADT1_SOURCE     = $(ADT1).c
ADT1_OBJECT     = $(ADT1).o
ADT1_HEADER     = $(ADT1).h
COMPILE         = gcc -c -std=c11 -Wall
LINK            = gcc -o
REMOVE          = rm -f
MEMCHECK        = valgrind --leak-check=full --show-leak-kinds=all

$(CLIENT) : $(OBJECT) $(ADT1_OBJECT)
	$(LINK) $(CLIENT) $(OBJECT) $(ADT1_OBJECT)

$(OBJECT) : $(SOURCE) $(ADT1_HEADER)
	$(COMPILE) $(SOURCE)

$(ADT1_OBJECT) : $(ADT1_SOURCE) $(ADT1_HEADER)
	$(COMPILE) $(ADT1_SOURCE)

clean :
	$(REMOVE) $(CLIENT) $(OBJECT) $(ADT1_OBJECT)

memcheck : $(CLIENT)
	$(MEMCHECK) $(CLIENT) in out
