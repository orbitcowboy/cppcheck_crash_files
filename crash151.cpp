enum { D = 1  struct  { } ; }  s.b = D;

/*
$ cppcheck crash151.cpp
Checking crash151.cpp...
segmentation fault
*/
