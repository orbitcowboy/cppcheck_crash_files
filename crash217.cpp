 { __allocator_base < _Tp > { } ( ) template < > { ( ) ( ) ( ) } > struct type ( ) ( ) = ( ) }
/*
 Checking crash217.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0826a070 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) bt
#0  0x0826a070 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
#1  0x0826a085 in std::string::_M_rep (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:301
#2  0x0826aabb in std::string::size (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:725
#3  0x082690db in std::string::compare (this=0x31, __s=0x848e79b "(")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
#4  0x08268aa1 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x848e79b "(")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#5  0x0840aa2b in Tokenizer::simplifyEmptyNamespaces (this=0xbfffe308)
    at lib/tokenize.cpp:4127
#6  0x08407ae9 in Tokenizer::simplifyTokenList1 (this=0xbfffe308, 
    FileName=0x854cf24 "crash217.cpp") at lib/tokenize.cpp:3555
#7  0x083fce8d in Tokenizer::tokenize (this=0xbfffe308, code=..., 
    FileName=0x854cf24 "crash217.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1592
#8  0x0835fe50 in CppCheck::checkFile (this=0xbfffeb0c, code=..., 
    FileName=0x854cf24 "crash217.cpp", checksums=...) at lib/cppcheck.cpp:360
#9  0x0835e739 in CppCheck::processFile (this=0xbfffeb0c, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:238
#10 0x0835d124 in CppCheck::check (this=0xbfffeb0c, path=...)
*/
