 ( * ) ; { int i ; for ( i = 0 ; = 123 ; ) - ; }
/*
 Checking crash210.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0828bd56 in Token::varId (this=0x0) at lib/token.h:442
442	        return _varId;
(gdb) bt
#0  0x0828bd56 in Token::varId (this=0x0) at lib/token.h:442
#1  0x0844177e in valueFlowForLoop2 (tok=0x853d9a0, memory1=0xbfffe194, 
    memory2=0xbfffe1b8, memoryAfter=0xbfffe1dc) at lib/valueflow.cpp:1269
#2  0x084429be in valueFlowForLoop (tokenlist=0xbfffe338, 
    errorLogger=0xbfffeb3c, settings=0xbfffeb54) at lib/valueflow.cpp:1430
#3  0x08443aaa in ValueFlow::setValues (tokenlist=0xbfffe338, 
    errorLogger=0xbfffeb3c, settings=0xbfffeb54) at lib/valueflow.cpp:1582
#4  0x083f25ad in Tokenizer::tokenize (this=0xbfffe338, code=..., 
    FileName=0x853cf24 "crash210.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1607
#5  0x0835700a in CppCheck::checkFile (this=0xbfffeb3c, code=..., 
    FileName=0x853cf24 "crash210.cpp") at lib/cppcheck.cpp:355
#6  0x083559b0 in CppCheck::processFile (this=0xbfffeb3c, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:235
#7  0x083543fa in CppCheck::check (this=0xbfffeb3c, path=...)
    at lib/cppcheck.cpp:66
#8  0x08269ef5 in CppCheckExecutor::check_internal (this=0xbffff008, 
    cppcheck=..., argv=0xbffff114) at cli/cppcheckexecutor.cpp:742
#9  0x082690cf in CppCheckExecutor::check (this=0xbffff008, argc=2, 
    argv=0xbffff114) at cli/cppcheckexecutor.cpp:188
#10 0x08272315 in main (argc=2, argv=0xbffff114) at cli/main.cpp:122
*/
