%{
#include <stdio.h>
#include <string.h>
void cacerror(const char *, int, const char *);
%}

%define api.value.type {char*}
%define api.prefix {cac}
%define parse.error verbose
%parse-param {char *username} {int accBit}

%start begin
%token USER READ WRITE BOTH

%%

begin		: line				{YYABORT;}

line		: USER accessBit '\n' line	{if(!accBit && 
						    !strncmp(username,$1,strlen(username)) &&
						    (*$2 == 'r' || *$2 == 'b') )
						 { //get (read), found username in file
						     printf("%s found, read\n",username);
						     printf("%s %c\n",$1,*$2);
						     YYACCEPT;
						 }else if( accBit &&
							   !strncmp(username,$1,strlen(username)) &&
							   (*$2 == 'w' || *$2 == 'b') )
						 {
						     printf("%s found, write\n",username);
						     printf("%s %c\n",$1,*$2);
						     YYACCEPT;
						 }
						;}
		| USER accessBit '\n' 		{if(!accBit &&
                                                    !strncmp(username,$1,strlen(username)) &&
                                                    (*$2 == 'r' || *$2 == 'b') )
                                                 { //get (read), found username in file
                                                     printf("%s found, read\n",username);
						     printf("%s %c\n",$1,*$2);
                                                     YYACCEPT;
                                                 }else if( accBit &&
                                                           !strncmp(username,$1,strlen(username)) &&
                                                           (*$2 == 'w' || *$2 == 'b') )
                                                 {
                                                     printf("%s found, write\n",username);
						     printf("%s %c\n",$1,*$2);
                                                     YYACCEPT;
                                                 }
                                                ;}
		;

accessBit	: READ				{$$ = "r";}
		| WRITE				{$$ = "w";}
		| BOTH				{$$ = "b";}
		;


%%

/*int main(void)
*{
*    int result;
*    result = cacparse();
*    printf("RESULT: %d\n", result);
*    return result;
*}*/

void cacerror(const char *un, int accBit, const char *s) {fprintf(stderr, "%s, %d, %s\n",un, accBit, s);}
