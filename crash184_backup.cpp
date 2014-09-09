A
I> struct template<int
{
    static const int zero = 0;
    static const int minus_one = -1;
};
template<int N> struct
B
{
    int x[A<N>::zero];
    int y[A<N>::minus_one];
};
B<0> b;