long f ()
{
    return a >> extern
}
long a = 1 ;
long b = 1 ;

/*$ cppcheck --enable=all crash168.cpp
Checking crash168.cpp...
[crash168.cpp:1]: (style) Statements following return, break, continue, goto or throw will never be executed.
Segmentation fault (core dumped)


(gdb) run --enable=all crash168.cpp
Starting program: /usr/bin/cppcheck --enable=all crash168.cpp
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
Checking crash168.cpp...
[crash168.cpp:1]: (style) Statements following return, break, continue, goto or throw will never be executed.

Program received signal SIGSEGV, Segmentation fault.
0x000000000040d2c6 in std::string::_M_data (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:290
290	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040d2c6 in std::string::_M_data (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:290
#1  0x000000000040ccd2 in std::string::_M_rep (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:298
#2  0x000000000040c7d8 in std::string::size (this=0x30)
    at /usr/include/c++/4.7/bits/basic_string.h:713
#3  0x000000000040bffe in std::string::compare (this=0x30, __s=0x610dd3 "{")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#4  0x000000000040be1b in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x610dd3 "{")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#5  0x00000000004ca06f in CheckOther::checkVariableScope (this=0x7fffffffc810)
    at lib/checkother.cpp:1760
#6  0x0000000000489fc7 in CheckOther::runChecks (
    this=0x8d4d60 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffcb20,
    settings=0x7fffffffd9e0, errorLogger=0x7fffffffd9b0) at lib/checkother.h:69
#7  0x00000000005199b4 in CppCheck::checkFile (this=0x7fffffffd9b0, code=...,
    FileName=0x8dd958 "crash168.cpp") at lib/cppcheck.cpp:374
#8  0x0000000000517c8a in CppCheck::processFile (this=0x7fffffffd9b0,
    filename=...) at lib/cppcheck.cpp:237
#9  0x00000000005166c5 in CppCheck::check (this=0x7fffffffd9b0, path=...)
    at lib/cppcheck.cpp:65
#10 0x00000000004133a7 in CppCheckExecutor::check (this=0x7fffffffdf60,
*/
