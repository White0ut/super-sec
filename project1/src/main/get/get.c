#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"

#define DEBUG 1

int is_sym_lnk(char* path);

/**
* Silent exists emit "silent exit" to the terminal before exiting
* 
* Usage: get <source> <destination>
*/
int main(int argc, char** argv) {
	// printf("We are working here\n");

	if (argc != 3) {
		printf("Error, invalid params\nUsage: get <source> <destination>\n");
		return 1;
	}

	/* init file variables */
	FILE *source_fp;
    FILE *acl_fp;
    FILE *destination_fp;
    char *acl_path = strncat(argv[1], ".access", 7);
   
    /* if the ACL file is a sym link, exit silently */
    if (is_sym_lnk(acl_path)) {
    	printf("silent exit\n");
    	if (DEBUG) {
    		printf("ACL is a sym link\n");
    	}
    	return 1;
    }

    source_fp  = fopen(argv[1], "r");
    acl_fp = fopen(acl_path, "r");

	/* If the ACL file does not exist, exit silently */
	if (acl_fp == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("ACL file doesn't exist\n");
		}
        return 1;
    }

	destination_fp = fopen(argv[2], "w");

	/* malformed ACL entry causes a silent exit (FLEX file ?? ) */
	
	/* if the protection for the ACL allows world or group access exit silently */

	/* if the file is not an ordinary file, exit silently */

	// Get the Effective UID geteuid()
	// Get the Real UID getuid()

	fclose(source_fp);
	fclose(destination_fp);
	fclose(acl_fp);
    return 0;
}

/**
* if the file point to by *path* is a sym link, return 1
* otherwise return 0
*/
int is_sym_lnk(char *path) {
	struct stat buf;
	int x;

	x = lstat(path, &buf);
	if (x < 0) return 0;

	if (S_ISLNK(buf.st_mode)) return 1;
	else return 0;
}