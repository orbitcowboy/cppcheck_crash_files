enum { e = { } } ( ) { { enum { } } } { e } 
/*(gdb) run crash195_reduced.cpp
Starting program: /home/martin/cppcheck/cppcheck crash195_reduced.cpp
Checking crash195_reduced.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000066004c in Token::next (this=0x0) at lib/token.h:357
357	        return _next;
(gdb) backtrace
#0  0x000000000066004c in Token::next (this=0x0) at lib/token.h:357
#1  0x000000000082633c in Tokenizer::simplifyEnum (this=0x7fffffffc600)
    at lib/tokenize.cpp:7602
#2  0x000000000080deb7 in Tokenizer::simplifyTokenList1 (this=0x7fffffffc600, 
    FileName=0xb60ee8 "crash195_reduced.cpp") at lib/tokenize.cpp:3237
#3  0x0000000000804163 in Tokenizer::tokenize (this=0x7fffffffc600, code=..., 
    FileName=0xb60ee8 "crash195_reduced.cpp", configuration=...)
    at lib/tokenize.cpp:1596
#4  0x000000000075af1e in CppCheck::checkFile (this=0x7fffffffd440, code=..., 
    FileName=0xb60ee8 "crash195_reduced.cpp") at lib/cppcheck.cpp:354
#5  0x000000000075952e in CppCheck::processFile (this=0x7fffffffd440, 
    filename=..., fileContent=...) at lib/cppcheck.cpp:234
#6  0x0000000000757f5b in CppCheck::check (this=0x7fffffffd440, path=...)
    at lib/cppcheck.cpp:65
#7  0x00000000006405b5 in CppCheckExecutor::check_internal (
    this=0x7fffffffdd70, cppCheck=..., argv=0x7fffffffdf18)
    at cli/cppcheckexecutor.cpp:418
#8  0x000000000063fa25 in CppCheckExecutor::check (this=0x7fffffffdd70, 
    argc=2, argv=0x7fffffffdf18) at cli/cppcheckexecutor.cpp:179
#9  0x0000000000649e22 in main (argc=2, argv=0x7fffffffdf18)
    at cli/main.cpp:117
*/
