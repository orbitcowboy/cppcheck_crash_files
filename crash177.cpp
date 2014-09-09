void * f { } void b ( )
{
    * f
}
/*(gdb) run --enable=all crash177.cpp
Starting program: /usr/bin/cppcheck --enable=all crash177.cpp
Checking crash177.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x080734a4 in Token::next (this=0x0) at lib/token.h:336
336	        return _next;
(gdb) backtrace
#0  0x080734a4 in Token::next (this=0x0) at lib/token.h:336
#1  0x080fdcd6 in CheckOther::clarifyStatement (this=0xbfffe3dc)
    at lib/checkother.cpp:329
#2  0x080cc9d4 in CheckOther::runSimplifiedChecks (
    this=0x830e004 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe4e8,
    settings=0xbfffec68, errorLogger=0xbfffec50) at lib/checkother.h:86
#3  0x0815972a in CppCheck::checkFile (this=0xbfffec50, code=...,
    FileName=0x83145f4 "crash177.cpp") at lib/cppcheck.cpp:390
#4  0x08157a62 in CppCheck::processFile (this=0xbfffec50, filename=...,
    fileContent=...) at lib/cppcheck.cpp:234
#5  0x08156573 in CppCheck::check (this=0xbfffec50, path=...)
    at lib/cppcheck.cpp:65
#6  0x08058681 in CppCheckExecutor::check (this=0xbffff068, argc=3,
    argv=0xbffff164) at cli/cppcheckexecutor.cpp:180
#7  0x08060798 in main (argc=3, argv=0xbffff164) at cli/main.cpp:97
* */
