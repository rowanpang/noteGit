#include <stdio.h>

extern int hello(char* msg);
extern int simplefloor(double x);

int main(int argc, char** argv)
{

    printf("main func\n");

    hello("hello by extern hello");

    simplefloor(3.33);

    return 0;
}
