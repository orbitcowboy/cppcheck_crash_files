int << {  main ( ) << return "x" }

    /*
    $ cppcheck --enable=all crash162.cpp
    Checking crash162.cpp...
    Segmentation fault (core dumped)
    */
    /*
    (gdb) run --enable=all crash162.cpp
    Starting program: /usr/bin/cppcheck --enable=all crash162.cpp
    Checking crash162.cpp...

    Program received signal SIGSEGV, Segmentation fault.
    0x080534de in std::string::_M_data (this=0x18)
        at /usr/include/c++/4.7/bits/basic_string.h:290
    290	      { return  _M_dataplus._M_p; }
    (gdb) backtrace
    #0  0x080534de in std::string::_M_data (this=0x18)
        at /usr/include/c++/4.7/bits/basic_string.h:290
    #1  0x08052feb in std::string::_M_rep (this=0x18)
        at /usr/include/c++/4.7/bits/basic_string.h:298
    #2  0x08052b7f in std::string::size (this=0x18)
        at /usr/include/c++/4.7/bits/basic_string.h:713
    #3  0x080524d9 in std::string::compare (this=0x18, __s=0x8214d3d ";")
        at /usr/include/c++/4.7/bits/basic_string.tcc:954
    #4  0x0805236c in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8214d3d ";")
        at /usr/include/c++/4.7/bits/basic_string.h:2518
    #5  0x0806f50f in std::operator!=<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8214d3d ";")
        at /usr/include/c++/4.7/bits/basic_string.h:2555
    #6  0x080f64b5 in CheckOther::checkCommaSeparatedReturn (this=0xbfffe80c)
        at lib/checkother.cpp:1889
    #7  0x080ba73d in CheckOther::runChecks (
        this=0x82cef84 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe968,
        settings=0xbfffef38, errorLogger=0xbfffef20) at lib/checkother.h:76
    #8  0x0813c481 in CppCheck::checkFile (this=0xbfffef20, code=...,
        FileName=0x82d5be4 "crash162.cpp") at lib/cppcheck.cpp:352
    #9  0x0813ab37 in CppCheck::processFile (this=0xbfffef20, filename=...)
        at lib/cppcheck.cpp:215
     */
