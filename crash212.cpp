 ; ; template < class... > struct const { = } ( A < void > ( void ) A void ) ;
/*Checking crash212.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x08265190 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) bt
#0  0x08265190 in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:293
#1  0x082651a5 in std::string::_M_rep (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:301
#2  0x08265bdb in std::string::size (this=0x31)
    at /usr/include/c++/4.9/bits/basic_string.h:725
#3  0x082641fb in std::string::compare (this=0x31, __s=0x8486e5b "(")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
#4  0x08263bc1 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8486e5b "(")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#5  0x084050e9 in Tokenizer::simplifyEmptyNamespaces (this=0xbfffe338)
    at lib/tokenize.cpp:4090
#6  0x084021a7 in Tokenizer::simplifyTokenList1 (this=0xbfffe338, 
    FileName=0x8543f24 "crash212.cpp") at lib/tokenize.cpp:3518
#7  0x083f796b in Tokenizer::tokenize (this=0xbfffe338, code=..., 
    FileName=0x8543f24 "crash212.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1592
#8  0x0835aa64 in CppCheck::checkFile (this=0xbfffeb3c, code=..., 
    FileName=0x8543f24 "crash212.cpp", checksums=...) at lib/cppcheck.cpp:360
#9  0x0835934d in CppCheck::processFile (this=0xbfffeb3c, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:238
#10 0x08357d38 in CppCheck::check (this=0xbfffeb3c, path=...)
    at lib/cppcheck.cpp:66
#11 0x0826d885 in CppCheckExecutor::check_internal (this=0xbffff008, cppcheck=..., 
*/
*/
