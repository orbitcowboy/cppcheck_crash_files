
enum
{
    A = SOME_VALUE,
    B = A
};

/*
s$ cppcheck --enable=all crash133.cpp
Checking crash133.cpp...
segmentation fault
*/

