struct template<int
{
    =
};
> struct B { };
B < 0 > b;

/*
 (gdb) run crash184.cpp
Starting program: /usr/bin/cppcheck crash184.cpp
Checking crash184.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000040ce86 in std::string::_M_data (this=0x59) at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040ce86 in std::string::_M_data (this=0x59) at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x000000000040cea4 in std::string::_M_rep (this=0x59) at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x000000000040d854 in std::string::size (this=0x59) at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x000000000040c318 in std::string::compare (this=0x59, __s=0x6625e3 "(") at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x000000000040c16f in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x6625e3 "(") at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x00000000005e4f23 in Tokenizer::simplifyEmptyNamespaces (this=0x7fffffffc7c0) at lib/tokenize.cpp:3967
#6  0x00000000005e26d3 in Tokenizer::simplifyTokenList1 (this=0x7fffffffc7c0) at lib/tokenize.cpp:3513
#7  0x00000000005d82da in Tokenizer::tokenize (this=0x7fffffffc7c0, code=..., FileName=0x923928 "crash184.cpp", configuration=...) at lib/tokenize.cpp:1592
#8  0x0000000000535fba in CppCheck::checkFile (this=0x7fffffffd5e0, code=..., FileName=0x923928 "crash184.cpp") at lib/cppcheck.cpp:354
#9  0x00000000005345ca in CppCheck::processFile (this=0x7fffffffd5e0, filename=..., fileContent=...) at lib/cppcheck.cpp:234
#10 0x0000000000532ff7 in CppCheck::check (this=0x7fffffffd5e0, path=...) at lib/cppcheck.cpp:65
#11 0x000000000041472d in CppCheckExecutor::check (this=0x7fffffffde10, argc=2, argv=0x7fffffffdfb8) at cli/cppcheckexecutor.cpp:203
#12 0x000000000041ee96 in main (argc=2, argv=0x7fffffffdfb8) at cli/main.cpp:117
*/
