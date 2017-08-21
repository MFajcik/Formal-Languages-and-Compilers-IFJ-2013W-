#init
#### info
#
# 'make'        build executable file 'ifj13'
# 'make clean'  removes all .o and executable files
#
CC=gcc
CFLAGS = -std=c99 -Wall  -pedantic  -Wextra  
OBJ = ial.o ilist.o scanfun.o scanner.o main.o structs.o parser.o interpret.o collector.o
HEADERS = error.h ial.h ilist.h scanfun.h scanner.h structs.h parser.h interpret.h collector.h

%.o : %.c $(HEADERS)
	$(CC)  -c -o $@ $< $(CFLAGS)

ifj13   : $(OBJ)
	$(CC)  -o $@ $^ $(CFLAGS) 
	
clean :
	@rm -f *.o *~ ifj13
