void f ( )
{
        = a ;
    if ( 1 ) if = ( 0 ) ;
}
/*
$ cppcheck crash161.cpp
Checking crash161.cpp...
[crash161.cpp:1]: (error) syntax error
Segmentation fault (core dumped)
*/

/*
(gdb) run crash161.cpp
Starting program: /usr/bin/cppcheck crash161.cpp
Checking crash161.cpp...
[crash161.cpp:1]: (error) syntax error

Program received signal SIGSEGV, Segmentation fault.
0x0805333e in std::string::_M_data (this=0x18)
    at /usr/include/c++/4.7/bits/basic_string.h:290
290	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x0805333e in std::string::_M_data (this=0x18)
    at /usr/include/c++/4.7/bits/basic_string.h:290
#1  0x08052e4b in std::string::_M_rep (this=0x18)
    at /usr/include/c++/4.7/bits/basic_string.h:298
#2  0x080529df in std::string::size (this=0x18)
    at /usr/include/c++/4.7/bits/basic_string.h:713
#3  0x08052339 in std::string::compare (this=0x18, __s=0x82252a3 "}")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#4  0x080521cc in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x82252a3 "}")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#5  0x0806e943 in std::operator!=<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x82252a3 "}")
    at /usr/include/c++/4.7/bits/basic_string.h:2555
#6  0x081c1dd9 in Tokenizer::simplifyAddBracesPair (this=0xbfffea38,
    tok=0x82d0328, commandWithCondition=true) at lib/tokenize.cpp:4122
#7  0x081c1ba5 in Tokenizer::simplifyAddBracesToCommand (this=0xbfffea38,
    tok=0x82d0328) at lib/tokenize.cpp:4072
#8  0x081c1934 in Tokenizer::simplifyAddBraces (this=0xbfffea38)
    at lib/tokenize.cpp:4032
#9  0x081b56cc in Tokenizer::tokenize (this=0xbfffea38, code=...,
    FileName=0x82cf844 "crash161.cpp", configuration=...)
    at lib/tokenize.cpp:1650
*/
