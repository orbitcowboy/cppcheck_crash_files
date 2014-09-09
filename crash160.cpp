// from /home/martin/compiler/gcc/gcc/testsuite/g++.dg/cpp0x/pr51547.C

template <class T>
struct vector
{
    T*
    begin()
    {
        return &member;
    }

    const T*
    begin() const
    {
        return &member;
    }

    T member;
};

struct Bar
{
    int x;
};

struct Foo
{
    const vector<Bar>& bar() const
    {
        return bar_;
    }

    vector<Bar> bar_;
};

template <class X>
struct Y
{
    void foo()
    {
        Foo a;
        auto b = a.bar().begin();
        auto&& c = b->x;
    }
};

/*
./cppcheck --enable=all --debug ../bugs/crash160.cpp
Checking ../bugs/crash160.cpp...
segmentation fault

*/
