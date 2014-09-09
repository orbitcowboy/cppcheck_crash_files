{ for ( ; ; ) if c = 1 ; c = 1 ; }

/*
$ cppcheck  crash163.cpp Checking crash163.cpp...
[crash163.cpp:3]: (error) syntax error
Segmentation fault (core dumped)
*/
/*
(gdb) run crash163.cpp
Starting program: /usr/bin/cppcheck crash163.cpp
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
bChecking crash163.cpp...
a[crash163.cpp:1]: (error) syntax error

Program received signal SIGSEGV, Segmentation fault.
0x000000000040d08a in std::string::_M_data (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:290
290	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040d08a in std::string::_M_data (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:290
#1  0x000000000040ca96 in std::string::_M_rep (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:298
#2  0x000000000040c5c6 in std::string::size (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:713
#3  0x000000000040bdec in std::string::compare (this=0x30, __s=0x61122b "}")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#4  0x000000000040bc09 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x61122b "}")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#5  0x000000000042cb20 in std::operator!=<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x61122b "}")
    at /usr/include/c++/4.7/bits/basic_string.h:2555
#6  0x00000000005a407d in Tokenizer::simplifyAddBracesPair (
    this=0x7fffffffd060, tok=0x8c0d20, commandWithCondition=true)
    at lib/tokenize.cpp:4122
#7  0x00000000005a3c7f in Tokenizer::simplifyAddBracesToCommand (
    this=0x7fffffffd060, tok=0x8c0d20) at lib/tokenize.cpp:4045
#8  0x00000000005a3bb3 in Tokenizer::simplifyAddBraces (this=0x7fffffffd060)
    at lib/tokenize.cpp:4032
#9  0x0000000000596ef4 in Tokenizer::tokenize (this=0x7fffffffd060, code=...,
    FileName=0x8c0458 "crash163.cpp", configuration=...)
*/
