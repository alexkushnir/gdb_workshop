#include <stdio.h>

int Transform(int x) 
{
    int y = x * 3;
    int z = y + 5;
    int w = z * 2;
    return w;
}

int main(void) 
{
    int value = 7;
    int result = Transform(value);
    printf("result = %d\n", result);
    return 0;
}