# CS5950 ASSIGNMENT 1

## ACCESS CONTROL LISTS

### Contributions
_Kendrick Cline_
	- makefile
	- get.c
	- project layout

_Dillon Daudert_
	- checkSyntax.y
	- accessTokens(2).l
	- checkAccess.y
	- put.c
	

get and put are used by the owner of certain files to grant access of them to other users. 

The owner sets the setuid bit in the permissions. These files thus have elevated privileges while they execute.

Before doing anything, they check that the owner owns the files granting access to other users AND that the specific user has said access.

### Security decisions:
The user has access to a program with elevated privileges, so we check lengths on input to prevent buffer overflow attacks.

The syntax of the .access file is guaranteed through the use of a lexical and syntax parser implemented usign flex and bison.
