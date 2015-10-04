#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include <stdlib.h>
#include <pwd.h>

const char *getUserName();
const char *getRealUserName();
int checkOwner(char *path);

#define MAX_INPUT_LEN 256
#define DEBUG 0

extern FILE *csyin;
extern FILE *cacin;

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

	if (strlen(argv[1]) > MAX_INPUT_LEN || strlen(argv[2]) > MAX_INPUT_LEN) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Input strings too long\n");
		}
		return 1;
	}

	/* init file variables */
	FILE *source_fp;
    FILE *destination_fp;
    char *source_p = (char*) malloc(sizeof(char) * strlen(argv[1]));
    char *dest_p = (char*) malloc(sizeof(char) * strlen(argv[2]));
    strncpy(source_p, argv[1], strlen(argv[1]) + 1);
    strncpy(dest_p, argv[2], strlen(argv[2]) + 1);
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

    source_fp  = fopen(source_p, "r");

    if (source_fp == NULL) {
    	printf("silent exit\n");
    	if (DEBUG) {
			printf("Source file doesn't exist\n");
    	}
    	return 1;
    }

	/* if source isn't owned by owner, silent exit */
	if (!checkOwner(source_p)) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Source is not owned by real owner\n");
		}
		return 1;
	}

	csyin = fopen(acl_path, "r");
	cacin = fopen(acl_path, "r");

	/* If the ACL file does not exist, exit silently */
	if (csyin == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("ACL file doesn't exist\n");
		}
        return 1;
    }

	if (cacin == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Error opening ACL file\n");
		}
		return 1;
	}

	destination_fp = fopen(dest_p, "w+");

	if (destination_fp == NULL) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Destination file doesn't exist\n");
		}
		return 1;
	}

	/* malformed ACL entry causes a silent exit */
	if (csyparse()) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("malformed ACL entry\n");
		}
		return 1;
	}

	// Get the Effective UID geteuid()
	const char *user_name = getUserName();

	if (cacparse(user_name, 0)) {
		printf("silent exit\n");
		if (DEBUG) {
			printf("Access denied by ACL\n");
		}
		return 1;
	}

	int ch;
	while (1) {
		ch = fgetc(source_fp);

		if (ch == EOF)
			break;
		else
			putc(ch, destination_fp);
	}

	/* set the owner of the destination file to the user */
	chown(dest_p, getuid(), getuid());

	fclose(source_fp);
	fclose(destination_fp);
	fclose(csyin);
	fclose(cacin);
    return 0;
}

/**
* determines if the file pointed to by path
* is owned by the real user id
*/
int checkOwner(char *path) {
	struct stat buf;
	int x;

    x = lstat(path, &buf);
    if (x < 0) {
		return 0;
    }

    if (buf.st_uid == getuid()) {
		return 1;
    }
    return 0;
}

/**
* returns the real user name
*/
const char *getRealUserName() {
	uid_t ruid = getuid();
	struct passwd *pw = getpwuid(ruid);
	if (pw) {
		return pw->pw_name;
	}
	return "";
}

/**
* returns the effective user name
*/
const char *getUserName() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw) {
    return pw->pw_name;
  }

  return "";
}