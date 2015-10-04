D		[0-9]
NZ		[1-9]
L		[a-zA-Z_]
A		[a-zA-Z_0-9]

%{
#include <stdlib.h>
#include "checkAccess.tab.h"
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
{L}{A}*                 {caclval = malloc(strlen(cactext) + 1);
                        strcpy(caclval, cactext);
                        return USER;}
[\n]			{return yytext[0];}
.			{ECHO; cacerror("Unexpected character");}

%%
int cacwrap(void)	{return 1;}

