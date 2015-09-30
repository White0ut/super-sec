%{
#include <stdio.h>
void cacerror(const char *);
%}

%define api.value.type {char*}
%define api.prefix {cac}
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
*    result = cacparse();
*    printf("RESULT: %d\n", result);
*    return result;
*}*/

void cacerror(const char *s) {fprintf(stderr, "%s\n",s);}
