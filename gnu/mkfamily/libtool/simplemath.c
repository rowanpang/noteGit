#include <stdio.h>
#include <math.h>

int simplefloor(double x)
{

    double down,up;
    down=floor(x);
    up=ceil(x);

    printf("orgnum: %5.2lf,round down: %5.2lf,round up: %5.2lf\n", x,down,up);

    return 0;
}
