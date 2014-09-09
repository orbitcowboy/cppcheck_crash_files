T::X <typename T> struct A
{
    typedef X;
};
template operator X();
typename template <typename T> A<T>::operator typename A<T>::X () {}
/*$ cppcheck crash182.cpp
Checking crash182.cpp...
Segmentation fault (core dumped)
*/
/*(gdb) run crash182.cpp
Starting program: /usr/bin/cppcheck crash182.cpp
Checking crash182.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x080535cb in std::string::size (this=0x8317ee0) at /usr/include/c++/4.8/bits/basic_string.h:716
716	      { return _M_rep()->_M_length; }
(gdb) backtrace
#0  0x080535cb in std::string::size (this=0x8317ee0) at /usr/include/c++/4.8/bits/basic_string.h:716
#1  0x08052627 in std::string::compare (this=0x8317ee0, __s=0x8256fec ">")
    at /usr/include/c++/4.8/bits/basic_string.tcc:952
#2  0x08052484 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=...,
    __rhs=0x8256fec ">") at /usr/include/c++/4.8/bits/basic_string.h:2521
#3  0x081c7a32 in TemplateSimplifier::useDefaultArgumentValues (templates=...,
    templateInstantiations=0xbfffe268) at lib/templatesimplifier.cpp:519
#4  0x081cc71f in TemplateSimplifier::simplifyTemplates (tokenlist=..., errorlogger=..., _settings=0xbfffec78,
    _codeWithTemplates=@0xbfffe558: true) at lib/templatesimplifier.cpp:1282
#5  0x081e3425 in Tokenizer::simplifyTemplates (this=0xbfffe4f8) at lib/tokenize.cpp:2654
#6  0x081dfee9 in Tokenizer::tokenize (this=0xbfffe4f8, code=..., FileName=0x83165f4 "crash182.cpp",
    configuration=...) at lib/tokenize.cpp:2007
#7  0x08159d1c in CppCheck::checkFile (this=0xbfffec60, code=..., FileName=0x83165f4 "crash182.cpp")
    at lib/cppcheck.cpp:354
#8  0x081584c6 in CppCheck::processFile (this=0xbfffec60, filename=..., fileContent=...)
    at lib/cppcheck.cpp:234
#9  0x08156fd7 in CppCheck::check (this=0xbfffec60, path=...) at lib/cppcheck.cpp:65
#10 0x08058681 in CppCheckExecutor::check (this=0xbffff078, argc=2, argv=0xbffff174)
    at cli/cppcheckexecutor.cpp:180
#11 0x08060798 in main (argc=2, argv=0xbffff174) at cli/main.cpp:97
*/
