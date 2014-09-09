return f() {}

// gcc/gcc/testsuite/g++.old-deja/g++.benjamin/p13417.C
// cppcheck crashes on this code
//$ cppcheck crash81.cpp
//Checking crash81.cpp...
// segmentation fault
