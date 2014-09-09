{ enum struct : };
/*(gdb) run crash187.cpp
Starting program: /usr/bin/cppcheck crash187.cpp
Checking crash187.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000040d538 in std::string::_M_data (this=0x3c) at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040d538 in std::string::_M_data (this=0x3c) at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x000000000040caa4 in std::string::_M_rep (this=0x3c) at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x000000000040d192 in std::string::size (this=0x3c) at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x000000000040bf58 in std::string::compare (this=0x3c, __s=0x643c61 "(") at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x000000000040bd39 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x643c61 "(")
    at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x00000000005cc1f1 in Tokenizer::simplifyEmptyNamespaces (this=0x7fffffffc740) at lib/tokenize.cpp:3933
#6  0x00000000005c09ec in Tokenizer::tokenize (this=0x7fffffffc740, code=..., FileName=0x9044a8 "crash187.cpp",
    configuration=...) at lib/tokenize.cpp:2081
#7  0x000000000052b330 in CppCheck::checkFile (this=0x7fffffffd670, code=..., FileName=0x9044a8 "crash187.cpp")
    at lib/cppcheck.cpp:354
#8  0x00000000005297ec in CppCheck::processFile (this=0x7fffffffd670, filename=..., fileContent=...) at lib/cppcheck.cpp:234
#9  0x0000000000528219 in CppCheck::check (this=0x7fffffffd670, path=...) at lib/cppcheck.cpp:65
#10 0x00000000004131c2 in CppCheckExecutor::check (this=0x7fffffffde10, argc=2, argv=0x7fffffffdfb8)
    at cli/cppcheckexecutor.cpp:180
#11 0x000000000041c508 in main (argc=2, argv=0x7fffffffdfb8) at cli/main.cpp:97
*/
