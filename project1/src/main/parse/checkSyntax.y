%{
#include <stdio.h>
void csyerror(const char *);
%}

%define api.value.type {char*}
%define api.prefix {csy}
%define parse.error verbose

%start line
%token USER READ WRITE BOTH

%%

line		: USER accessBit '\n'		{;}
		| USER accessBit '\n' line	{printf("USER accessbit\n");}
		;

accessBit	: READ				{;}
		| WRITE				{;}
		| BOTH				{;}
		;


%%

/*int main(void)
*{
*    int result;
*    result = csyparse();
*    printf("RESULT: %d\n", result);
*    return result;
*}*/

void csyerror(const char *s) {fprintf(stderr, "%s\n",s);}
