#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char **argv)
{
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "abc:")) != -1){
        printf("cur:%c,index:%d\n",c,optind);
        switch (c)
        {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'c':
                cvalue = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option ‘-%c'.\n", optopt);
                else
                    fprintf (stderr,"Unknown option character ‘\\x%x'.\n",optopt);
                break;
                //return 1;
            case '\1':
                printf("case '\1'\n");
                break;
            default:
                printf("default:%c\n",c);
                //abort ();
                break;
        }
    }

    printf("aflag = %d, bflag = %d, cvalue = %s,optind = %d\n",
		aflag, bflag, cvalue,optind);
    for (index = optind; index < argc; index++){
        printf ("Non-option argument %s\n", argv[index]);
    }

    int myArry[5] = {10,20,30,40,50};
    int *myp = myArry;
    printf("value of '*myp++'\n");
    printf("argument %d\n", *myp++);
    printf("argument %d\n", *myp++);
    myp = myArry;
    printf("value of '*++myp'\n");
    printf("argument %d\n", *++myp);
    printf("argument %d\n", *++myp);

    myp = myArry;
    printf("value of '*(myp++)'\n");
    printf("argument %d\n", *(myp++));
    printf("argument %d\n", *(myp++));

    return 0;
}
