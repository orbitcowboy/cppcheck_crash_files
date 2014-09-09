{
    {
        void foo()
        {
            struct
                };
        template <typename> struct S { Used x; void bar() } auto f = [this] { };
    }
};

/*
 Checking crash191.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x08053b1e in std::string::_M_data (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x08053b1e in std::string::_M_data (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x08053b33 in std::string::_M_rep (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x08054345 in std::string::size (this=0x2c)
   at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x08052d29 in std::string::compare (this=0x2c, __s=0x826841f "else")
   at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x08052bec in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x826841f "else")
   at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x08127b40 in CheckStl::iterators (this=0xbfffe39c) at lib/checkstl.cpp:211
#6  0x08130bf6 in CheckStl::runSimplifiedChecks (
   this=0x8334124 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe458,
   settings=0xbfffeb88, errorLogger=0xbfffeb70) at lib/checkstl.h:54
#7  0x08159760 in CppCheck::checkFile (this=0xbfffeb70, code=...,
   FileName=0x833afac "/home/martin/projects/cppcheck_testfiles/bugs/crash191.cpp") at lib/cppcheck.cpp:390
#8  0x08157bac in CppCheck::processFile (this=0xbfffeb70, filename=...,
   fileContent=...) at lib/cppcheck.cpp:234
#9  0x081566bd in CppCheck::check (this=0xbfffeb70, path=...)
   at lib/cppcheck.cpp:65
#10 0x0805bb44 in CppCheckExecutor::check_internal (this=0xbffff048, */
