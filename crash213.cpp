 struct true template < > { = } > struct Types "s" ; static_assert < int > ;

/*Checking crash213.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0826a020 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) bt
#0  0x0826a020 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
#1  0x0826a035 in std::string::_M_rep (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:301
#2  0x0826aa6b in std::string::size (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:725
#3  0x0826908b in std::string::compare (this=0x31, __s=0x848dc1b "(")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
#4  0x08268a51 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x848dc1b "(")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#5  0x0840a001 in Tokenizer::simplifyEmptyNamespaces (this=0xbfffe338)
    at lib/tokenize.cpp:4097
#6  0x084070bf in Tokenizer::simplifyTokenList1 (this=0xbfffe338, 
    FileName=0x854bf24 "crash213.cpp") at lib/tokenize.cpp:3525
#7  0x083fc5ff in Tokenizer::tokenize (this=0xbfffe338, code=..., 
    FileName=0x854bf24 "crash213.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1592
#8  0x0835f616 in CppCheck::checkFile (this=0xbfffeb3c, code=..., 
    FileName=0x854bf24 "crash213.cpp", checksums=...) at lib/cppcheck.cpp:360
#9  0x0835deff in CppCheck::processFile (this=0xbfffeb3c, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:238
#10 0x0835c8ea in CppCheck::check (this=0xbfffeb3c, path=...)
*/
