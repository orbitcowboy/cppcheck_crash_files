void f(void *a, void **b, size_t *c)
{
}

/*$ cppcheck crash131.cpp
Checking crash131.cpp...
Segmentation fault (core dumped)
*/

/*$ gdb cppcheck
GNU gdb (GDB) 7.5.91.20130417-cvs-ubuntu
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /usr/bin/cppcheck...done.
(gdb) run crash131.cpp
Starting program: /usr/bin/cppcheck crash131.cpp
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
Checking crash131.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0000000000406210 in __gnu_cxx::__exchange_and_add_single (
    __mem=0xfffffffffffffff8, __val=-1)
    at /usr/include/c++/4.7/ext/atomicity.h:66
66	    _Atomic_word __result = *__mem;
(gdb) backtrace
#0  0x0000000000406210 in __gnu_cxx::__exchange_and_add_single (
    __mem=0xfffffffffffffff8, __val=-1)
    at /usr/include/c++/4.7/ext/atomicity.h:66
#1  0x000000000040628a in __gnu_cxx::__exchange_and_add_dispatch (
    __mem=0xfffffffffffffff8, __val=-1)
    at /usr/include/c++/4.7/ext/atomicity.h:83
#2  0x000000000040cdda in std::string::_Rep::_M_dispose (
    this=0xffffffffffffffe8, __a=...)
    at /usr/include/c++/4.7/bits/basic_string.h:242
#3  0x000000000040cefa in std::string::assign (this=0x8dfab0, __str=...)
    at /usr/include/c++/4.7/bits/basic_string.tcc:252
#4  0x000000000040be45 in std::string::operator= (this=0x8dfab0, __str=...)
    at /usr/include/c++/4.7/bits/basic_string.h:544
#5  0x00000000005dac89 in Token::originalName (this=0x8dfa40, name=...)
    at lib/token.h:571
#6  0x00000000005bc8fb in Tokenizer::simplifyPlatformTypes (
    this=0x7fffffffcb40) at lib/tokenize.cpp:5370
#7  0x00000000005aa145 in Tokenizer::tokenize (this=0x7fffffffcb40, code=...,
    FileName=0x8de508 "crash131.cpp", configuration=...)
    at lib/tokenize.cpp:1847
#8  0x000000000051a256 in CppCheck::checkFile (this=0x7fffffffda10, code=...,
    FileName=0x8de508 "crash131.cpp") at lib/cppcheck.cpp:354
#9  0x000000000051872e in CppCheck::processFile (this=0x7fffffffda10,
*/



















