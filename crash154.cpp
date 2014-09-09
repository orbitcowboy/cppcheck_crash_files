// from gcc/gcc/testsuite/gcc.dg/cpp/arith-3.c


#if -9223372036854775808UL / -1
#endif


/*
$ cppcheck crash154.cpp
Checking crash154.cpp...
segmentation fault
*/
