void f()
{
    enum {x} a;
    (void) sizeof !! (a == 1);
}

// from: /home/martin/Downloads/linux-3.11.2/drivers/staging/lustre/lustre/obdclass/cl_page.c

/*
$ cppcheck --debug crash173.cpp Checking crash173.cpp...
Segmentation fault (core dumped)
*/

/* backtrace
#0  0x08052bfd in std::string::size (this=0x82fa8e0)
    at /usr/include/c++/4.7/bits/basic_string.h:713
#1  0x08052557 in std::string::compare (this=0x82fa8e0, __s=0x82435d9 "(")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#2  0x080523ea in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x82435d9 "(")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#3  0x081d7ac3 in Tokenizer::simplifyRealloc (this=0xbfffe7c8)
    at lib/tokenize.cpp:3807
#4  0x081d631e in Tokenizer::simplifyTokenList (this=0xbfffe7c8)
    at lib/tokenize.cpp:3536
#5  0x0814b8f1 in CppCheck::checkFile (this=0xbfffef10, code=...,
    FileName=0x82f75d4 "crash173.cpp") at lib/cppcheck.cpp:379
#6  0x08149d53 in CppCheck::processFile (this=0xbfffef10, filename=...,
    fileContent=...) at lib/cppcheck.cpp:234
#7  0x08148859 in CppCheck::check (this=0xbfffef10, path=...)
    at lib/cppcheck.cpp:65
#8  0x0805865e in CppCheckExecutor::check (this=0xbffff208, argc=2,
    argv=0xbffff304) at cli/cppcheckexecutor.cpp:179
#9  0x08060410 in main (argc=2, argv=0xbffff304) at cli/main.cpp:97
*/
