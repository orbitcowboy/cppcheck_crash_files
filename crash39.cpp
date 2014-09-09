int x;
void f()
{
    #pragma omp atomic
    x &= 1;
}
/*
$g++ -c crash39.cpp -lgomp
$

$ cppcheck --enable=all crash39.cpp
Checking crash39.cpp...
Segmentation fault

*/
