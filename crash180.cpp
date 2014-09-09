a f ( r ) int * r ;
{
    {
        int s[2] ;
        f ( s ) ;
        if ( )
        }
}
/*(gdb) run crash180.cpp
Starting program: /usr/bin/cppcheck crash180.cpp
Checking crash180.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000040d72a in std::string::_M_data (this=0x30) at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040d72a in std::string::_M_data (this=0x30) at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x000000000040cc96 in std::string::_M_rep (this=0x30) at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x000000000040d384 in std::string::size (this=0x30) at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x000000000040c14a in std::string::compare (this=0x30, __s=0x622038 "break")
    at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x000000000040bf2b in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=...,
    __rhs=0x622038 "break") at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x0000000000445c49 in bailoutIfSwitch (tok=0x0, varid=1) at lib/checkbufferoverrun.cpp:234
#6  0x00000000004480a9 in CheckBufferOverrun::checkFunctionParameter (this=0x7fffffffc4f0, tok=..., par=1,
    arrayInfo=..., callstack=...) at lib/checkbufferoverrun.cpp:658
#7  0x0000000000448cfe in CheckBufferOverrun::checkFunctionCall (this=0x7fffffffc4f0, tok=0x90de30, arrayInfo=...,
    callstack=...) at lib/checkbufferoverrun.cpp:758
#8  0x000000000044c0cc in CheckBufferOverrun::checkScope (this=0x7fffffffc4f0, tok=0x90de30, arrayInfo=...)
    at lib/checkbufferoverrun.cpp:1219
#9  0x000000000044d390 in CheckBufferOverrun::checkGlobalAndLocalVariable (this=0x7fffffffc4f0)
    at lib/checkbufferoverrun.cpp:1413
#10 0x000000000044e74c in CheckBufferOverrun::bufferOverrun (this=0x7fffffffc4f0) at lib/checkbufferoverrun.cpp:1678
#11 0x0000000000450c45 in CheckBufferOverrun::runSimplifiedChecks (this=0x903420 <_ZN12_GLOBAL__N_18instanceE>,
    tokenizer=0x7fffffffc760, settings=0x7fffffffd6c0, errorLogger=0x7fffffffd690) at lib/checkbufferoverrun.h:64
#12 0x0000000000531d0a in CppCheck::checkFile (this=0x7fffffffd690, code=..., FileName=0x90c4a8 "crash180.cpp")
    at lib/cppcheck.cpp:390
#13 0x000000000052fd20 in CppCheck::processFile (this=0x7fffffffd690, filename=..., fileContent=...)
    at lib/cppcheck.cpp:234
#14 0x000000000052e74d in CppCheck::check (this=0x7fffffffd690, path=...) at lib/cppcheck.cpp:65
#15 0x00000000004133b4 in CppCheckExecutor::check (this=0x7fffffffde30, argc=2, argv=0x7fffffffdfd8)
    at cli/cppcheckexecutor.cpp:180
#16 0x000000000041c6fa in main (argc=2, argv=0x7fffffffdfd8) at cli/main.cpp:97
* /
