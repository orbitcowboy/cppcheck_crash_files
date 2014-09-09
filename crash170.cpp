void f(int* bp)
{
    bp =
}

/*
 $ cppcheck --enable=all crash170.cpp
Checking crash170.cpp...
Segmentation fault (core dumped)


(gdb) run --enable=all crash170.cpp
Starting program: /usr/bin/cppcheck --enable=all crash170.cpp
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
Checking crash170.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0000000000436850 in Token::varId (this=0x0) at lib/token.h:364
364	        return _varId;
(gdb) backtrace
#0  0x0000000000436850 in Token::varId (this=0x0) at lib/token.h:364
#1  0x0000000000437d71 in variableIsUsedInScope (start=0x0, varId=1,
    scope=0x8e4db0) at lib/checkautovariables.cpp:105
#2  0x00000000004380d5 in CheckAutoVariables::autoVariables (
    this=0x7fffffffc8c0) at lib/checkautovariables.cpp:135
#3  0x0000000000439985 in CheckAutoVariables::runSimplifiedChecks (
    this=0x8d71c0 <(anonymous namespace)::instance>, tokenizer=0x7fffffffcb30,
    settings=0x7fffffffda30, errorLogger=0x7fffffffda00)
    at lib/checkautovariables.h:52
#4  0x000000000051bc50 in CppCheck::checkFile (this=0x7fffffffda00, code=...,
    FileName=0x8e0958 "crash170.cpp") at lib/cppcheck.cpp:390
#5  0x0000000000519c96 in CppCheck::processFile (this=0x7fffffffda00,
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#6  0x00000000005186b7 in CppCheck::check (this=0x7fffffffda00, path=...)
    at lib/cppcheck.cpp:65
#7  0x00000000004133a7 in CppCheckExecutor::check (this=0x7fffffffdf60,
    argc=3, argv=0x7fffffffe108) at cli/cppcheckexecutor.cpp:179
#8  0x000000000041c30e in main (argc=3, argv=0x7fffffffe108) at cli/main.cpp:97
*/
