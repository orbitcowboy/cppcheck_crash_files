#ifdef MACRO
char a;
MACRO (a)
#endif
/*
	from glibc-2.16.0/nptl/tst-tls5moda.c

$ cppcheck crash132.cpp
Checking crash132.cpp...
segmentation fault

*/
