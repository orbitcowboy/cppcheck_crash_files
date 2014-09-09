template<class
T> constexpr T value(T t = T())
{
    return t;
}
enum us_enum { us_item = value<short>() };
void func(us_enum
          n)
{
    switch (n)
    {
    case value(us_item):
        ;
    default:
        ;
    }
}