D		[0-9]
NZ		[1-9]
L		[a-zA-Z_]
A		[a-zA-Z_0-9]

%{
#include <stdlib.h>
#include "checkSyntax.tab.h"
%}

%x COMMENT

%%
\#			{BEGIN COMMENT;}
<COMMENT>\n		{BEGIN 0;}
<COMMENT>.		;
[ \t]			;		//ignore whitespace
b			{return BOTH;}
w			{return WRITE;}
r			{return READ;}
{L}{A}*                 {csylval = malloc(strlen(csytext) + 1);
                        strcpy(csylval, yytext);
                        return USER;}
[\n]			{return yytext[0];}
.			{ECHO; csyerror("Unexpected character");}

%%
int csywrap(void)	{return 1;}

