( asm : ; void : );
/*
 run crash178.cpp
Starting program: /usr/bin/cppcheck crash178.cpp
Checking crash178.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x080734a4 in Token::next (this=0x0) at lib/token.h:336
336	        return _next;
(gdb) backtrace
#0  0x080734a4 in Token::next (this=0x0) at lib/token.h:336
#1  0x081efc3f in Tokenizer::simplifyVarDecl (this=0xbfffe4f8, tokBegin=0x8314f90,
    tokEnd=0x0, only_k_r_fpar=false) at lib/tokenize.cpp:5201
#2  0x081ef9d5 in Tokenizer::simplifyVarDecl (this=0xbfffe4f8, only_k_r_fpar=false)
    at lib/tokenize.cpp:5177
#3  0x081ded63 in Tokenizer::tokenize (this=0xbfffe4f8, code=...,
    FileName=0x83145f4 "crash178.cpp", configuration=...) at lib/tokenize.cpp:1980
#4  0x081592b8 in CppCheck::checkFile (this=0xbfffec60, code=...,
    FileName=0x83145f4 "crash178.cpp") at lib/cppcheck.cpp:354
#5  0x08157a62 in CppCheck::processFile (this=0xbfffec60, filename=..., fileContent=...)
    at lib/cppcheck.cpp:234
#6  0x08156573 in CppCheck::check (this=0xbfffec60, path=...) at lib/cppcheck.cpp:65
#7  0x08058681 in CppCheckExecutor::check (this=0xbffff078, argc=2, argv=0xbffff174)
    at cli/cppcheckexecutor.cpp:180
#8  0x08060798 in main (argc=2, argv=0xbffff174) at cli/main.cpp:97
*/
