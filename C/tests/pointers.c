#include <stdio.h>

void first_func(int *f)
{
    *f -= 140;
}

void second_func(int *f)
{
    *f += 135;
}

int main()
{
    int f = 150;
    first_func(&f);
    second_func(&f);
    printf("%d\n", f);
    return 0;
}