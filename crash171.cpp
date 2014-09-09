void f()
{
    double* p1= new double[1];
    double* p2= new double[1];
    double* p3= new double[1];
    double* pp[3] = {p1,p2,p3};
}
/*
$ cppcheck --enable=all --debug crash171.cpp
Checking crash171.cpp...


##file crash171.cpp
1: void f ( ) {
2: double * p1@1 ; p1@1 = new double [ 1 ] ;
3: double * p2@2 ; p2@2 = new double [ 1 ] ;
4: double * p3@3 ; p3@3 = new double [ 1 ] ;
5:
6: double * pp@4 [ 3 ] = { p1@1 , p2@2 , p3@3 } ;
7:
8: }

Segmentation fault (core dumped)
*/
/*
(gdb) run --enable=all crash171.cpp
Starting program: /usr/bin/cppcheck --enable=all crash171.cpp
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
Checking crash171.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0000000000431cae in Token::function (this=0x0) at lib/token.h:489
489	        return _type == eFunction ? _function : 0;
(gdb) backtrace
#0  0x0000000000431cae in Token::function (this=0x0) at lib/token.h:489
#1  0x00000000004ce0d7 in CheckOther::checkZeroDivisionOrUselessCondition (
    this=0x7fffffffc950) at lib/checkother.cpp:2195
#2  0x000000000048b80d in CheckOther::runSimplifiedChecks (
    this=0x8d7d60 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffcbc0,
    settings=0x7fffffffdac0, errorLogger=0x7fffffffda90) at lib/checkother.h:93
#3  0x000000000051c06c in CppCheck::checkFile (this=0x7fffffffda90, code=...,
    FileName=0x8e0958 "crash171.cpp") at lib/cppcheck.cpp:390
#4  0x000000000051a0b2 in CppCheck::processFile (this=0x7fffffffda90,
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#5  0x0000000000518ad3 in CppCheck::check (this=0x7fffffffda90, path=...)
    at lib/cppcheck.cpp:65
#6  0x00000000004133a7 in CppCheckExecutor::check (this=0x7fffffffdff0,
    argc=3, argv=0x7fffffffe198) at cli/cppcheckexecutor.cpp:179
#7  0x000000000041c30e in main (argc=3, argv=0x7fffffffe198) at cli/main.cpp:97
*/
