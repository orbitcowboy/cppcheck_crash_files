int a = int b = ( 0 ? ? ) 1 : 0 ;

/*$ cppcheck crash179.cpp
Checking crash179.cpp...
Segmentation fault (core dumped)
*/
/*
 (gdb) run crash179.cpp
Starting program: /usr/bin/cppcheck crash179.cpp
Checking crash179.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x080535cb in std::string::size (this=0x8317470) at /usr/include/c++/4.8/bits/basic_string.h:716
716	      { return _M_rep()->_M_length; }
(gdb) backtrace
#0  0x080535cb in std::string::size (this=0x8317470) at /usr/include/c++/4.8/bits/basic_string.h:716
#1  0x08052627 in std::string::compare (this=0x8317470, __s=0x8258af9 "(")
    at /usr/include/c++/4.8/bits/basic_string.tcc:952
#2  0x08052484 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=...,
    __rhs=0x8258af9 "(") at /usr/include/c++/4.8/bits/basic_string.h:2521
#3  0x081ed12c in Tokenizer::simplifyConditionOperator (this=0xbfffe4f8) at lib/tokenize.cpp:4450
#4  0x081e962d in Tokenizer::simplifyTokenList (this=0xbfffe4f8) at lib/tokenize.cpp:3651
#5  0x0815a083 in CppCheck::checkFile (this=0xbfffec60, code=..., FileName=0x83165f4 "crash179.cpp")
    at lib/cppcheck.cpp:379
#6  0x081584c6 in CppCheck::processFile (this=0xbfffec60, filename=..., fileContent=...)
    at lib/cppcheck.cpp:234
#7  0x08156fd7 in CppCheck::check (this=0xbfffec60, path=...) at lib/cppcheck.cpp:65
#8  0x08058681 in CppCheckExecutor::check (this=0xbffff078, argc=2, argv=0xbffff174)
    at cli/cppcheckexecutor.cpp:180
#9  0x08060798 in main (argc=2, argv=0xbffff174) at cli/main.cpp:97
*/
