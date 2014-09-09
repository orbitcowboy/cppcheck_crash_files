void exit (int);
void f ^ { return } int main ( )
{
    exit (0);
}
/*$ cppcheck crash167.cpp
Checking crash167.cpp...
/usr/include/c++/4.7/bits/stl_stack.h:160:error: attempt to access an
    element in an empty container.

Objects involved in the operation:
sequence "this" @ 0x0xbfc8fa28 {
  type = St5stackIP5TokenNSt7__debug5dequeIS1_SaIS1_EEEE;
}
Aborted (core dumped)
*/

/*Starting program: /usr/bin/cppcheck crash167.cpp
Checking crash167.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x08052bcd in std::string::size (this=0x82e5e00)
    at /usr/include/c++/4.7/bits/basic_string.h:713
713	      { return _M_rep()->_M_length; }
(gdb) backtrace
#0  0x08052bcd in std::string::size (this=0x82e5e00)
    at /usr/include/c++/4.7/bits/basic_string.h:713
#1  0x08052527 in std::string::compare (this=0x82e5e00, __s=0x8233d26 ":")
    at /usr/include/c++/4.7/bits/basic_string.tcc:954
#2  0x080523ba in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8233d26 ":")
    at /usr/include/c++/4.7/bits/basic_string.h:2518
#3  0x0807101f in std::operator!=<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x8233d26 ":")
    at /usr/include/c++/4.7/bits/basic_string.h:2555
#4  0x081c6460 in setVarIdClassDeclaration (startToken=0x82e6630,
    variableId=..., scopeStartVarId=0, structMembers=0xbfffe474,
    _varId=0xbfffe804) at lib/tokenize.cpp:2744
#5  0x081c7318 in Tokenizer::setVarId (this=0xbfffe7a8)
    at lib/tokenize.cpp:2884
#6  0x081c2811 in Tokenizer::tokenize (this=0xbfffe7a8, code=...,
    FileName=0x82e55d4 "crash167.cpp", configuration=...)
    at lib/tokenize.cpp:2033
#7  0x08141a7e in CppCheck::checkFile (this=0xbfffeee8, code=...,
    FileName=0x82e55d4 "crash167.cpp") at lib/cppcheck.cpp:357
#8  0x081401f4 in CppCheck::processFile (this=0xbfffeee8, filename=...)
    at lib/cppcheck.cpp:237
#9  0x0813ecae in CppCheck::check (this=0xbfffeee8, path=...)
*/
