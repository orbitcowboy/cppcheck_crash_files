#include <cstdio>
#include <cstdlib>
int f()
{
    int ret_val;

    if ( true )
    {
        do
        {
            exit( 1 );
        }
        while ( true );
    }
    return ret_val ;
}
/*
 $ cppcheck crash166.cpp
Checking crash166.cpp...
cppcheck: lib/checkuninitvar.cpp:1357: bool CheckUninitVar::checkScopeForVariable(const Scope*, const Token*, const Variable&, bool*, bool*, const string&): Assertion `Token::simpleMatch(tok, "} while (")' failed.
Aborted (core dumped)


Program received signal SIGABRT, Aborted.
0xb7fdd424 in __kernel_vsyscall ()
(gdb) backtrace
#0  0xb7fdd424 in __kernel_vsyscall ()
#1  0xb7cf4b1f in raise () from /lib/i386-linux-gnu/libc.so.6
#2  0xb7cf80b3 in abort () from /lib/i386-linux-gnu/libc.so.6
#3  0xb7ced877 in ?? () from /lib/i386-linux-gnu/libc.so.6
#4  0xb7ced927 in __assert_fail () from /lib/i386-linux-gnu/libc.so.6
#5  0x08125848 in CheckUninitVar::checkScopeForVariable (this=0xbfffe648,
    scope=0x82e6bf0, tok=0x82e6b88, var=..., possibleInit=0x0, noreturn=0x0,
    membervar=...) at lib/checkuninitvar.cpp:1357
#6  0x081247b2 in CheckUninitVar::checkScopeForVariable (this=0xbfffe648,
    scope=0x82e6bf0, tok=0x82e58f0, var=..., possibleInit=0x0, noreturn=0x0,
    membervar=...) at lib/checkuninitvar.cpp:1184
#7  0x08123f6c in CheckUninitVar::checkScope (this=0xbfffe648, scope=0x82e6bf0)
    at lib/checkuninitvar.cpp:1094
#8  0x08123b68 in CheckUninitVar::check (this=0xbfffe648)
    at lib/checkuninitvar.cpp:1058
#9  0x08127b8b in CheckUninitVar::runSimplifiedChecks (
    this=0x82df128 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe7a8,
    settings=0xbfffef00, errorLogger=0xbfffeee8) at lib/checkuninitvar.h:54
#10 0x08141fc8 in CppCheck::checkFile (this=0xbfffeee8, code=...,
    FileName=0x82e55d4 "crash166.cpp") at lib/cppcheck.cpp:397
#11 0x081401f4 in CppCheck::processFile (this=0xbfffeee8, filename=...)
    at lib/cppcheck.cpp:237
#12 0x0813ecae in CppCheck::check (this=0xbfffeee8, path=...)

*/











