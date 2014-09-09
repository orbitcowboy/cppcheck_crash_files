struct x foo_t;
foo_t typedef y;

/*
$ cppcheck crash136.cpp
Checking crash136.cpp...
segmentation fault
*/
