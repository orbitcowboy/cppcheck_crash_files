{ };
void namespace A::f;
{
    g()
    {
        int
    }
}

/*
  (gdb) run /home/martin/projects/cppcheck_testfiles/bugs/crash193.cpp
Starting program: /home/martin/cppcheck/./cppcheck /home/martin/projects/cppcheck_testfiles/bugs/crash193.cpp
Checking /home/martin/projects/cppcheck_testfiles/bugs/crash193.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x082524ae in std::string::_M_data (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x082524ae in std::string::_M_data (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x082524c3 in std::string::_M_rep (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x08252cd5 in std::string::size (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x082516b9 in std::string::compare (this=0x2c, __s=0x847252c ";")
   at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x0825157c in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x847252c ";")
   at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x083aa7e5 in SymbolDatabase::SymbolDatabase (this=0x853d060,
   tokenizer=0xbfffe448, settings=0xbfffeb88, errorLogger=0xbfffeb70)
   at lib/symboldatabase.cpp:84
#6  0x0841ff35 in Tokenizer::createSymbolDatabase (this=0xbfffe448)
   at lib/tokenize.cpp:9918
#7  0x083eea18 in Tokenizer::tokenize (this=0xbfffe448, code=...,
   FileName=0x8539fac "/home/martin/projects/cppcheck_testfiles/bugs/crash193.cpp", configuration=...) at lib/tokenize.cpp:1597
#8  0x08357f8c in CppCheck::checkFile (this=0xbfffeb70, code=...,
   FileName=0x8539fac "/home/martin/projects/cppcheck_testfiles/bugs/crash193.cpp") at lib/cppcheck.cpp:354
#9  0x0835684a in CppCheck::processFile (this=0xbfffeb70, filename=...,*/
