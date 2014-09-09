 ; struct S { enum { f = { } ; } ; = S::f ; }
 
 /*
 Checking crash209.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x08267ac0 in std::string::_M_data (this=0x18)
    at /usr/include/c++/4.9/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x08267ac0 in std::string::_M_data (this=0x18)
    at /usr/include/c++/4.9/bits/basic_string.h:293
#1  0x08267ad5 in std::string::_M_rep (this=0x18)
    at /usr/include/c++/4.9/bits/basic_string.h:301
#2  0x0826850b in std::string::size (this=0x18)
    at /usr/include/c++/4.9/bits/basic_string.h:725
#3  0x08266b2b in std::string::compare (this=0x18, __s=0x8489f5b "(")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
#4  0x082664f1 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8489f5b "(")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#5  0x0840da11 in Tokenizer::simplifyVarDecl (this=0xbfffe338, 
    tokBegin=0x8549348, tokEnd=0x0, only_k_r_fpar=false)
    at lib/tokenize.cpp:5259
#6  0x0840d8a4 in Tokenizer::simplifyVarDecl (this=0xbfffe338, 
    only_k_r_fpar=false) at lib/tokenize.cpp:5237
#7  0x0840553d in Tokenizer::simplifyTokenList1 (this=0xbfffe338, 
    FileName=0x8546f24 "crash209.cpp") at lib/tokenize.cpp:3480
#8  0x083faec7 in Tokenizer::tokenize (this=0xbfffe338, code=..., 
    FileName=0x8546f24 "crash209.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1592
#9  0x0835d8b4 in CppCheck::checkFile (this=0xbfffeb3c, code=..., 
    FileName=0x8546f24 "crash209.cpp") at lib/cppcheck.cpp:355
*/
