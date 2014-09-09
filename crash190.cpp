A template < int { int = -1 ; } template < int N > struct B
{
    int [ A < N > :: zero ] ;
} ;
B < 0 > b ;
/*
 (gdb) run crash190.cpp
Starting program: /media/martin/0c1d854e-681c-43b7-9bbc-1ee5005b6c46/projects/cppcheck_testfiles/bugs/../cppcheck/cppcheck crash190.cpp
Checking crash190.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0805309c in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x0805309c in std::string::_M_data (this=0x31)
    at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x080530b1 in std::string::_M_rep (this=0x31)
    at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x08053901 in std::string::size (this=0x31)
    at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x0805268d in std::string::compare (this=0x31, __s=0x826ad9b "(")
    at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x08052550 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x826ad9b "(")
    at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x081f667e in Tokenizer::simplifyEmptyNamespaces (this=0xbfffe428)
    at lib/tokenize.cpp:3883
#6  0x081f4161 in Tokenizer::simplifyTokenList1 (this=0xbfffe428)
    at lib/tokenize.cpp:3428
#7  0x081ea646 in Tokenizer::tokenize (this=0xbfffe428, code=...,
    FileName=0x8329c8c "crash190.cpp", configuration=...)
    at lib/tokenize.cpp:1592
#8  0x08158bfc in CppCheck::checkFile (this=0xbfffeb1c, code=...,
    FileName=0x8329c8c "crash190.cpp") at lib/cppcheck.cpp:354
#9  0x081574ba in CppCheck::processFile (this=0xbfffeb1c, filename=...,
    fileContent=...) at lib/cppcheck.cpp:234
#10 0x08155fcb in CppCheck::check (this=0xbfffeb1c, path=...)
*/
