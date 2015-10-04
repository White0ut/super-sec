#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"

#define DEBUG 1

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
    /* TODO: if the protection for the ACL allows world or group access exit silently */
    struct stat buf;
	int x;

	x = lstat(acl_path, &buf);
	if (x < 0) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Can't read file info\n");
		}
		return 1;
	}

	if (S_ISLNK(buf.st_mode)) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("ACL is a sym link\n");
		}
		return 1;
	}

    source_fp  = fopen(argv[1], "r");

    if (source_fp == NULL) {
    	printf("silent exit\n");
    	if (DEBUG) {
			printf("Source file doesn't exist\n");
    	}
    	return 1;
    }

    acl_fp = fopen(acl_path, "r");

	/* If the ACL file does not exist, exit silently */
	if (acl_fp == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("ACL file doesn't exist\n");
		}
        return 1;
    }

	destination_fp = fopen(argv[2], "w+");

	if (destination_fp == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Destination file doesn't exist\n");
		}
		return 1;
	}

	/* malformed ACL entry causes a silent exit (FLEX file ?? ) */
	if (csyparse()) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("malformed ACL entry\n");
		}
		return 1;
	}

	/* if the file is not an ordinary file, exit silently */

	// Get the Effective UID geteuid()
	uid_t euid = geteuid();

	uid_t ruid = getuid();

	printf("euid:%u\nruid:%u\n", euid, ruid);
	// Get the Real UID getuid()

	fclose(source_fp);
	fclose(destination_fp);
	fclose(acl_fp);
    return 0;
}

int csyparse() {
	return 0;
}

