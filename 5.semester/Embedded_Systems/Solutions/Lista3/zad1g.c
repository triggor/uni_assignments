//Wersja grupowa ;)
#include <stdio.h>
#include <stdlib.h>

void mult(long long F[2][2], long long M[2][2])
{
    long long f00 = F[0][0] * M[0][0] + F[0][1] * M[1][0];
    long long f01 = F[0][0] * M[0][1] + F[0][1] * M[1][1];
    long long f10 = F[1][0] * M[0][0] + F[1][1] * M[1][0];
    long long f11 = F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = f00;
    F[0][1] = f01;
    F[1][0] = f10;
    F[1][1] = f11;
}

void power(long long F[2][2], int n)
{
    if( n == 0 || n == 1)
        return;
    long long M[2][2] = {{1,1},{1,0}};

    power(F, n/2);
    mult(F, F);

    if (n%2 != 0)
        mult(F, M);
}

long long fib3(int n)
{
    long long F[2][2] = {{1,1},{1,0}};
    if (n == 0)
        return 0;
    power(F, n-1);
    return F[0][0];
}

long long fib1(int n)
{
    if(n < 3) return 1;
    return fib1(n-1) + fib1(n-2);
}

long long fib2(int n)
{
    if(n < 3) return 1;
    long long a = 1;
    long long b = 1;
    while(--n > 0)
    {
        a = a + b;

        //swap(a, b)
        a = a ^ b;
        b = a ^ b;
        a = b ^ a;
    }
    return a;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        return 0;
    int number = atoi(argv[1]);
    printf("%lli", fib3(number));
    return 0;
}
