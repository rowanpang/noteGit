#include <stdio.h>
extern void print();
extern void print_internal();

int main()
{
    print();
    print_internal();
}
