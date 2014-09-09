testme(int
       "C" void abort (); typedef struct st { unsigned void
               a; unsigned char b; unsigned char c; unsigned char d;
               extern __attribute__((aligned(4))) st;
                                            } testme(int, int, int); static inline void
       stupid_func(st s)
{
    testme(s.a, s.b, s.c);
}
int main()
{
    st s;
    s.a = s.b = s.c = 216;
    stupid_func(s);
    return 0;
}
void char a, int b, int c)
{
    if (a != 216 || b != 216 ||
            c != 216) abort();
}