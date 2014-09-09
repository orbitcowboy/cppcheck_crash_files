static f ( )
{
    int i;
    int source[1] = { 1 };
    for (i = 0 ; i < 4 ; i++ ) (u , if ( y u.x e) )
        }

/*
 Checking /home/martin/projects/cppcheck_testfiles/bugs/crash189.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x000000000040d9f4 in std::string::_M_data (this=0x58)
    at /usr/include/c++/4.8/bits/basic_string.h:293
293	      { return  _M_dataplus._M_p; }
(gdb) backtrace
#0  0x000000000040d9f4 in std::string::_M_data (this=0x58)
    at /usr/include/c++/4.8/bits/basic_string.h:293
#1  0x000000000040da12 in std::string::_M_rep (this=0x58)
    at /usr/include/c++/4.8/bits/basic_string.h:301
#2  0x000000000040e392 in std::string::size (this=0x58)
    at /usr/include/c++/4.8/bits/basic_string.h:716
#3  0x000000000040ca0e in std::string::compare (this=0x58,
    __s=0x63e71d "break") at /usr/include/c++/4.8/bits/basic_string.tcc:952
#4  0x000000000040c865 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=..., __rhs=0x63e71d "break")
    at /usr/include/c++/4.8/bits/basic_string.h:2521
#5  0x000000000044a6cf in bailoutIfSwitch (tok=0x0, varid=2)
    at lib/checkbufferoverrun.cpp:278
#6  0x000000000044b699 in CheckBufferOverrun::parse_for_body (
    this=0x7fffffffc420, tok=0x92ec30, arrayInfo=..., strindex=...,
    condition_out_of_bounds=true, counter_varid=1, min_counter_value=...,
    max_counter_value=...) at lib/checkbufferoverrun.cpp:524
#7  0x000000000044df2b in CheckBufferOverrun::checkScopeForBody (
    this=0x7fffffffc420, tok=0x92e050, arrayInfo=...,
    bailout=@0x7fffffffc034: false) at lib/checkbufferoverrun.cpp:897
#8  0x0000000000450a76 in CheckBufferOverrun::checkScope (this=0x7fffffffc420,
    tok=0x92e050, arrayInfo=...) at lib/checkbufferoverrun.cpp:1273
#9  0x0000000000451d14 in CheckBufferOverrun::checkGlobalAndLocalVariable (
* */
