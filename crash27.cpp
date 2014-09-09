class C : public B1
{
    B1()
    {} C(int) : B1() class
        };
/*
$ cppcheck --debug crash27.cpp
Checking crash27.cpp...
Segmentation fault

*/
