main x { = ( ) ; return *y }
/*
 $ cppcheck --enable=all crash24.cpp
Checking crash24.cpp...
Segmentation fault (core dumped)
 */

/*
 (gdb) run --enable=all crash24.cpp
Starting program: /usr/bin/cppcheck --enable=all crash24.cpp
Checking crash24.cpp...

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
#3  0x08052339 in std::string::compare (this=0x18, __s=0x82112dd ";")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#4  0x080521cc in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x82112dd ";")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#5  0x0806e943 in std::operator!=<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x82112dd ";")
    at /usr/include/c++/4.7/bits/basic_string.h:2555
#6  0x080f35b9 in CheckOther::checkCommaSeparatedReturn (this=0xbfffe8cc)
    at lib/checkother.cpp:1889
#7  0x080bb125 in CheckOther::runChecks (
    this=0x82c9064 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffea28,
    settings=0xbfffeff0, errorLogger=0xbfffefd8) at lib/checkother.h:76
#8  0x0813b089 in CppCheck::checkFile (this=0xbfffefd8, code=...,
    FileName=0x82cfa74 "crash24.cpp") at lib/cppcheck.cpp:352
#9  0x0813973f in CppCheck::processFile (this=0xbfffefd8, filename=...)
    at lib/cppcheck.cpp:215

 */
