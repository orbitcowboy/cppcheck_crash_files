template <typename T>
bool foo(){return true;}
struct A {
template<int n>
void t_func()
{
     if( n != 0 || foo<int>());
}
void t_caller()
{
    t_func<0>();
    t_func<1>();
}
};
// from https://trac.cppcheck.net/ticket/8683
