#include <stdio.h>
#include <signal.h>

void test_hander(int scno)
{
    (void)printf("Get a signal, scno=%d\n", scno);
}

int main(int argc, char const* argv[])
{
    (void)signal(SIGIO, test_hander);
    (void)raise(SIGIO);
    (void)printf("Test end\n");
    return 0;
}
