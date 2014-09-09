 { } int foo ( struct , x1 , struct x2 , x3 , int , x5 , x6 , x7 ) { ( foo ( s , , 2 , , , 5 , , 7 ) ) abort ( ) }

/*(gdb) run crash202.cpp 
Starting program: /usr/bin/cppcheck crash202.cpp
warning: the debug information found in "/lib64/ld-2.19.so" does not match "/lib64/ld-linux-x86-64.so.2" (CRC mismatch).

Checking crash202.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0000000000679074 in Token::varId (this=0x0) at lib/token.h:419
419	        return _varId;
(gdb) backtrace
#0  0x0000000000679074 in Token::varId (this=0x0) at lib/token.h:419
#1  0x000000000085b806 in valueFlowSubFunction (tokenlist=0x7fffffffc6a0, 
    errorLogger=0x7fffffffd4b0, settings=0x7fffffffd4e0)
    at lib/valueflow.cpp:1059
#2  0x000000000085bb19 in ValueFlow::setValues (tokenlist=0x7fffffffc6a0, 
    errorLogger=0x7fffffffd4b0, settings=0x7fffffffd4e0)
    at lib/valueflow.cpp:1083
#3  0x000000000080b620 in Tokenizer::tokenize (this=0x7fffffffc6a0, code=..., 
    FileName=0xb5deb8 "crash202.cpp", configuration=...)
    at lib/tokenize.cpp:1613
#4  0x000000000075e8ca in CppCheck::checkFile (this=0x7fffffffd4b0, code=..., 
    FileName=0xb5deb8 "crash202.cpp") at lib/cppcheck.cpp:354
#5  0x000000000075d153 in CppCheck::processFile (this=0x7fffffffd4b0, 
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#6  0x000000000075bb97 in CppCheck::check (this=0x7fffffffd4b0, path=...)
    at lib/cppcheck.cpp:65
#7  0x0000000000652c23 in CppCheckExecutor::check_internal (
    this=0x7fffffffdd70, cppcheck=..., argv=0x7fffffffdf18)
    at cli/cppcheckexecutor.cpp:433
#8  0x0000000000651f1c in CppCheckExecutor::check (this=0x7fffffffdd70, 
    argc=2, argv=0x7fffffffdf18) at cli/cppcheckexecutor.cpp:180
#9  0x000000000065be5a in main (argc=2, argv=0x7fffffffdf18)
    at cli/main.cpp:122
*/
