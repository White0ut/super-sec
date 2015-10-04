all: depend
	gcc checkSyntax.tab.c lex.csy.c checkAccess.tab.c lex.cac.c -c
	gcc checkSyntax.tab.o lex.csy.o checkAccess.tab.o lex.cac.o testProg.c -o testProg

depend: bison flex

bison:
	bison -d checkSyntax.y
	bison -d checkAccess.y

flex:
	flex -Pcsy accessTokens.l
	flex -Pcac accessTokens2.l

clean:
	rm checkSyntax.tab.c checkSyntax.tab.h lex.csy.c checkAccess.tab.c checkAccess.tab.h lex.cac.c checkSyntax.tab.o lex.csy.o checkAccess.tab.o lex.cac.o testProg
