void struct ( )
{
    if ( 1 )
    }
int main ( ) { }
/*$ cppcheck crash164.cpp
Checking crash164.cpp...
Segmentation fault (core dumped)
*/

/*
 run crash164.cpp
Starting program: /usr/bin/cppcheck crash164.cpp
Checking crash164.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0817fccb in SymbolDatabase::SymbolDatabase (this=0x82d2958,
    tokenizer=0xbfffea38, settings=0xbffff000, errorLogger=0xbfffefe8)
    at lib/symboldatabase.cpp:745
745	            if (scope->isClassOrStruct() && scope->definedType->needInitialization == Type::Unknown) {
(gdb) backtrace
#0  0x0817fccb in SymbolDatabase::SymbolDatabase (this=0x82d2958,
    tokenizer=0xbfffea38, settings=0xbffff000, errorLogger=0xbfffefe8)
    at lib/symboldatabase.cpp:745
#1  0x081e14a9 in Tokenizer::createSymbolDatabase (this=0xbfffea38)
    at lib/tokenize.cpp:9315
#2  0x081b629d in Tokenizer::tokenize (this=0xbfffea38, code=...,
    FileName=0x82d0844 "crash164.cpp", configuration=...)
    at lib/tokenize.cpp:2018
#3  0x0813b60a in CppCheck::checkFile (this=0xbfffefe8, code=...,
    FileName=0x82d0844 "crash164.cpp") at lib/cppcheck.cpp:335
#4  0x08139eaf in CppCheck::processFile (this=0xbfffefe8, filename=...)
    at lib/cppcheck.cpp:215
#5  0x08138d46 in CppCheck::check (this=0xbfffefe8, path=...)
    at lib/cppcheck.cpp:65
#6  0x0805845d in CppCheckExecutor::check (this=0xbffff208, argc=2,
    argv=0xbffff304) at cli/cppcheckexecutor.cpp:178
#7  0x08060210 in main (argc=2, argv=0xbffff304) at cli/main.cpp:97
*/
