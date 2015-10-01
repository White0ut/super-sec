#include "stdio.h"
#include "unistd.h"

/**
* Silent exists emit "silent exit" to the terminal before exiting
* 
* Usage: get <source> <destination>
*/
int main(int argc, char** argv) {
	printf("We are working here\n");

	printf("argc: %d\n", argc);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	/* If the ACL file does not exist, exit silently */

	/* malformed ACL entry causes a silent exit (FLEX file ?? ) */

	/* if the ACL is a sym link, exit silently */

	/* if the protection for the ACL allows world or group access exit silently */

	/* if the file is not an ordinary file, exit silently */

	// Get the Effective UID geteuid()
	// Get the Real UID getuid()

	
}
