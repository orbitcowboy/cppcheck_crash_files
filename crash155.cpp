// from /llvm/tools/clang/test/SemaCXX/cxx0x-cursory-default-delete.cpp

struct except_spec_d_good : except_spec_a, except_spec_b
{
    ~except_spec_d_good();
};

struct S
{
    S();
};
S::S() __attribute((pure)) = default;

/*
$ cppcheck crash155.cpp
Checking crash155.cpp...
segmentation fault
*/
