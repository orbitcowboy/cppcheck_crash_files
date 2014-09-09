#include <cstdio>
void f(char *dummy)
{
    int iVal;
    sscanf(dummy, "%d%c", &iVal);
}

void f1(char *dummy)
{
    int iVal;
    sscanf(dummy, "%d%c%f", &iVal);
}

void f2(char *dummy)
{
    int iVal;
    sscanf(dummy, "%d%c%.3f%X", &iVal);
}
