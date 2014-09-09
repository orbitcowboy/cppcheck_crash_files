void foo(int *P)
{
    *P
}
/*
(gdb) run --enable=all crash186.cpp
Starting program: /usr/bin/cppcheck --enable=all crash186.cpp
Checking crash186.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000043262e in Token::next (this=0x0) at lib/token.h:336
warning: Source file is more recent than executable.
336
(gdb) backtrace
#0  0x000000000043262e in Token::next (this=0x0) at lib/token.h:336
#1  0x00000000004cc04d in CheckOther::clarifyStatement (this=0x7fffffffc4b0)
    at lib/checkother.cpp:344
#2  0x00000000004965d3 in CheckOther::runSimplifiedChecks (
    this=0x8fbea0 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffc720,
    settings=0x7fffffffd680, errorLogger=0x7fffffffd650) at lib/checkother.h:86
#3  0x000000000052b7d6 in CppCheck::checkFile (this=0x7fffffffd650, code=...,
    FileName=0x9048f8 "crash186.cpp") at lib/cppcheck.cpp:390
#4  0x00000000005297ec in CppCheck::processFile (this=0x7fffffffd650,
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#5  0x0000000000528219 in CppCheck::check (this=0x7fffffffd650, path=...)
    at lib/cppcheck.cpp:65
#6  0x00000000004131c2 in CppCheckExecutor::check (this=0x7fffffffddf0,
    argc=3, argv=0x7fffffffdf98) at cli/cppcheckexecutor.cpp:180
#7  0x000000000041c508 in main (argc=3, argv=0x7fffffffdf98) at cli/main.cpp:97
*/
