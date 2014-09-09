#include <unistd.h>
static struct
{
    int i[2];
} p;

void foo()
{
    write(p.i[1], "", 1);
}

/*
$ cppcheck --std=posix --debug  crash158.cpp
Checking crash158.cpp...


##file crash158.cpp
1:
2: struct Anonymous0 {
3: int pipefd@1 [ 2 ] ;
4: } ; static struct Anonymous0 pr_wp@2 ;
5:
6: void foo ( )
7: {
8: write ( pr_wp@2 . pipefd@3 [ 1 ] , "" , 1 ) ;
9: }

cppcheck: lib/token.cpp:168: std::string Token::strValue() const: Assertion `_type == eString' failed.
Abgebrochen (Speicherabzug geschrieben)

*/
