 {  } * const ( ) { }
 /*(gdb) run crash198.cpp 
Starting program: /usr/bin/cppcheck crash198.cpp
warning: the debug information found in "/lib64/ld-2.19.so" does not match "/lib64/ld-linux-x86-64.so.2" (CRC mismatch).

Checking crash198.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x00000000006708bc in Token::previous (this=0x5) at lib/token.h:405
405	        return _previous;
(gdb) backtrace
#0  0x00000000006708bc in Token::previous (this=0x5) at lib/token.h:405
#1  0x00000000007fac6f in Token::tokAt (this=0xb60920, index=-2)
    at lib/token.cpp:288
#2  0x000000000072ca83 in CheckStl::string_c_str (this=0x7fffffffc520)
    at lib/checkstl.cpp:1142
#3  0x000000000072ffb1 in CheckStl::runSimplifiedChecks (
    this=0xb53120 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffc6a0, 
    settings=0x7fffffffd4e0, errorLogger=0x7fffffffd4b0) at lib/checkstl.h:60
#4  0x000000000075d1ae in CppCheck::checkFile (this=0x7fffffffd4b0, code=..., 
    FileName=0xb5beb8 "crash198.cpp") at lib/cppcheck.cpp:390
#5  0x000000000075b5b5 in CppCheck::processFile (this=0x7fffffffd4b0, 
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#6  0x0000000000759ff9 in CppCheck::check (this=0x7fffffffd4b0, path=...)
    at lib/cppcheck.cpp:65
#7  0x0000000000651e5d in CppCheckExecutor::check_internal (
    this=0x7fffffffdd70, cppcheck=..., argv=0x7fffffffdf18)
    at cli/cppcheckexecutor.cpp:438
#8  0x00000000006511bc in CppCheckExecutor::check (this=0x7fffffffdd70, 
    argc=2, argv=0x7fffffffdf18) at cli/cppcheckexecutor.cpp:180
#9  0x000000000065b094 in main (argc=2, argv=0x7fffffffdf18)
    at cli/main.cpp:122
*/
