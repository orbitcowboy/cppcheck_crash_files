 *(*const (size_t); foo) { }
/*(gdb) run --enable=all 20060208-10.cpp
Starting program: /usr/bin/cppcheck --enable=all 20060208-10.cpp
warning: File "/usr/lib/i386-linux-gnu/libstdc++.so.6.0.20-gdb.py" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
Checking 20060208-10.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x0826c08a in std::string::_Rep::_M_is_leaked (this=0x69536e69)
    at /usr/include/c++/4.9/bits/basic_string.h:192
192	        { return this->_M_refcount < 0; }
(gdb) bt
#0  0x0826c08a in std::string::_Rep::_M_is_leaked (this=0x69536e69)
    at /usr/include/c++/4.9/bits/basic_string.h:192
#1  0x0826a4af in std::string::_Rep::_M_grab (this=0x69536e69, __alloc1=..., 
    __alloc2=...) at /usr/include/c++/4.9/bits/basic_string.h:228
#2  0x08268b82 in std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string (this=0xbfffe0ec, __str=...)
    at /usr/include/c++/4.9/bits/basic_string.tcc:173
#3  0x08334b9f in std::pair<std::string, unsigned int>::pair<unsigned int&, void> (this=0xbfffe0ec, __x=..., __y=@0xbfffe198: 0)
    at /usr/include/c++/4.9/bits/stl_pair.h:139
#4  0x08334c03 in std::make_pair<std::string const&, unsigned int&> (__x=..., 
    __y=@0xbfffe198: 0) at /usr/include/c++/4.9/bits/stl_pair.h:281
#5  0x08330253 in CheckStl::string_c_str (this=0xbfffe1fc)
    at lib/checkstl.cpp:1123
#6  0x083336ff in CheckStl::runSimplifiedChecks (
    this=0x8544fa4 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe2c8, 
    settings=0xbfffeae4, errorLogger=0xbfffeacc) at lib/checkstl.h:59
#7  0x0835fda9 in CppCheck::checkFile (this=0xbfffeacc, code=..., 
    FileName=0x854c15c "20060208-10.cpp", checksums=...)
    at lib/cppcheck.cpp:417
#8  0x0835df07 in CppCheck::processFile (this=0xbfffeacc, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:238
#9  0x0835c8f2 in CppCheck::check (this=0xbfffeacc, path=...)
---Type <return> to continue, or q <return> to quit---q
 at lib/cppcheck.cpQuit
*/
