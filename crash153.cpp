struct A {};

struct B : A
{
    B() : A {}
};

/*
$ cppcheck crash153.cpp
Checking crash153.cpp...
segmentation fault
*/

// from gcc/gcc/testsuite/g++.dg/parse/ctor3.C
