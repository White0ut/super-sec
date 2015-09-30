#include <stdio.h>

extern FILE *csyin, *cacin;

int main(int argc, char** argv)
{
    int result, result2;
    csyin = fopen("testfile.txt", "r");

    printf("Testing first syntax parser ...\n");
    result = csyparse();

    cacin = fopen("testfile.txt", "r");

    printf("Testing second syntax parser ...\n");
    result2 = cacparse();

    printf("Results: %d, %d\n", result, result2);

    return 0;

}
