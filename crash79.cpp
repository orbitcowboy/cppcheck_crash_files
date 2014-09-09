// reduced version of from /gcc/gcc/testsuite/g++.dg/lto/20101010-2_0.C

template < int n > struct B
{
    unsigned _M[n];
};

template < int > class bitset: B < (((sizeof (unsigned)) ? : 1)) >
{};

int main()
{
    bitset < 1 > z;
    return 0;
}
