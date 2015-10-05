#define _POSIX_C_SOURCE		200120L
#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define DEBUG 1
#define ERROR(str)		fprintf(stderr, "%s" #str "\n",strerror(errno))
#define MAX_PATH		256

extern FILE *csyin, *cacin;

/*
 *setuid program
 *
 */
int main(int argc, char** argv)
{

    //Check that correct number of args sent in
    if(argc != 3){
        printf("Incorrect number of arguments, try:\n");
        printf("put <source> <destination>\n");
    }

    char source[MAX_PATH];
    char dest[MAX_PATH];
    char destacc[MAX_PATH];
    //Ensure that paths are short enough, destination.access
    if( (strlen(argv[1]) >= MAX_PATH) || (strlen(argv[2]) >= MAX_PATH-8) ){
        printf("source or destination path too long\n");
        exit(EXIT_FAILURE);
    }
    strcpy(source, argv[1]);
    strcpy(dest, argv[2]);
    strcpy(destacc, dest); strcat(destacc, ".access");
    uid_t owner = geteuid(); //Get owner
    uid_t user = getuid(); //Get user
    char *username;

    //Get user name
    username = getpwuid(user)->pw_name;

    //Allocate a new stat struct for dest, dest.access info
    struct stat dstat = {0};
    struct stat astat = {0};

    //Populate stat block of destination
    if(lstat(dest, &dstat) != 0){
        //Error
        printf("silent exit\n");
        if(DEBUG){
            ERROR(" in lstat(dest) block");
            printf("%s\n",dest);
        }
        exit(EXIT_FAILURE);
    }

    //Check that owner (euid) owns destination
    if(dstat.st_uid != owner){
        printf("silent exit\n");
        if(DEBUG){
            ERROR(", put owner does not own that file");
        }
        exit(EXIT_FAILURE);
    }

    //Check that owner has write access to destination
    if( !(dstat.st_mode & S_IWUSR) ){
        printf("silent exit\n");
        if(DEBUG){
            ERROR(", owner does not have write priveleges");
        }
        exit(EXIT_FAILURE);
    }

    //Check that dest.access exists and population astat block
    if(lstat(destacc, &astat) != 0){
        //Error
        printf("silent exit\n");
        if(DEBUG) ERROR(" in lstat(destacc) block");
        exit(EXIT_FAILURE);
    }

    //Check that .access is not a symbolic link
    if(S_ISLNK(astat.st_mode) ){
        printf("silent exit\n");
        if(DEBUG) ERROR(" .access is a symbolic link");
        exit(EXIT_FAILURE);
    }

    //Check that .access is owned by owner and no one else has access
    if(astat.st_uid != owner || (astat.st_mode & (S_IRWXG | S_IRWXO)) ){
        printf("silent exit\n");
        if(DEBUG) ERROR(" owner doesn't own or someone else has access\n");
        exit(EXIT_FAILURE);
    }

    //Open the access file using fopen for syntax parser
    csyin = fopen(destacc, "r");

    //Check the syntax of the access file
    if(csyparse() != 0){
        printf("silent exit\n");
        if(DEBUG) ERROR(" .access syntax is wrong");
        exit(EXIT_FAILURE);
    }

    //Open the access file for access checker
    cacin = fopen(destacc, "r");

    //Check that username has write (1) privileges
    if(cacparse(username, 1) != 0){
        printf("silent exit\n");
        if(DEBUG) ERROR(" username does not have privileges");
        exit(EXIT_FAILURE);
    }

    /*Lower privileges to real uid (user) ***********************************/
    seteuid(user);

    //Check that user has access to source
    if(access(source, R_OK) != 0){
        printf("silent exit\n");
        if(DEBUG) ERROR(" user does not have read privs of source\n");
        exit(EXIT_FAILURE);
    }

    /*Raise privileges back to owner*****************************************/
    seteuid(owner);

    //Construct arg string
    char* const args[] = {"/bin/cp", "-i", source, dest, NULL};

    //Execute mv, prompts overwrite
    execv("/bin/cp", args);

}
