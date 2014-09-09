 ( ) template < T1 = typename = unused> struct Args { } main ( ) { foo < int > ( ) }
 
 /*(gdb) run crash196.cpp 
Starting program: /home/martin/cppcheck/cppcheck crash196.cpp
Checking crash196.cpp...
*** Error in `/home/martin/cppcheck/cppcheck': double free or corruption (fasttop): 0x0853aa00 ***
======= Backtrace: =========
/lib/i386-linux-gnu/libc.so.6(+0x767c2)[0xb7d377c2]
/lib/i386-linux-gnu/libc.so.6(+0x77510)[0xb7d38510]
/usr/lib/i386-linux-gnu/libstdc++.so.6(_ZdlPv+0x1f)[0xb7f1ea3f]
/home/martin/cppcheck/cppcheck(_ZN9__gnu_cxx13new_allocatorIcE10deallocateEPcj+0x11)[0x8255e49]
/home/martin/cppcheck/cppcheck(_ZNSs4_Rep10_M_destroyERKSaIcE+0x40)[0x8254480]
/home/martin/cppcheck/cppcheck(_ZNSs4_Rep10_M_disposeERKSaIcE+0x4e)[0x8252536]
/home/martin/cppcheck/cppcheck(_ZNSsD1Ev+0x35)[0x82510bb]
/home/martin/cppcheck/cppcheck(_ZN5TokenD1Ev+0x22)[0x83dfc1c]
/home/martin/cppcheck/cppcheck(_ZN5Token10deleteNextEm+0x32)[0x83e0602]
/home/martin/cppcheck/cppcheck(_ZN5Token11eraseTokensEPS_PKS_+0x2b)[0x83e2d55]
/home/martin/cppcheck/cppcheck(_ZN18TemplateSimplifier24useDefaultArgumentValuesERKNSt7__debug4listIP5TokenSaIS3_EEEPS5_+0xab8)[0x83d04e4]
/home/martin/cppcheck/cppcheck(_ZN18TemplateSimplifier17simplifyTemplatesER9TokenListR11ErrorLoggerPK8SettingsRb+0x1c9)[0x83d471d]
/home/martin/cppcheck/cppcheck(_ZN9Tokenizer17simplifyTemplatesEv+0x141)[0x83f1bd1]
/home/martin/cppcheck/cppcheck(_ZN9Tokenizer18simplifyTokenList1EPKc+0xf5c)[0x83f84b0]
/home/martin/cppcheck/cppcheck(_ZN9Tokenizer8tokenizeERSiPKcRKSs+0x109)[0x83eea05]
/home/martin/cppcheck/cppcheck(_ZN8CppCheck9checkFileERKSsPKc+0x344)[0x8357f8c]
/home/martin/cppcheck/cppcheck(_ZN8CppCheck11processFileERKSsS1_+0x100e)[0x835684a]
/home/martin/cppcheck/cppcheck(_ZN8CppCheck5checkERKSs+0x45)[0x835535b]
/home/martin/cppcheck/cppcheck(_ZN16CppCheckExecutor14check_internalER8CppCheckiPKPKc+0x5bc)[0x825a558]
/home/martin/cppcheck/cppcheck(_ZN16CppCheckExecutor5checkEiPKPKc+0xdf)[0x8259a5f]
/home/martin/cppcheck/cppcheck(main+0x30)[0x82629ec]
/lib/i386-linux-gnu/libc.so.6(__libc_start_main+0xf5)[0xb7cda905]
/home/martin/cppcheck/cppcheck[0x824b701]
======= Memory map: ========
08048000-08531000 r-xp 00000000 08:01 262163     /home/martin/cppcheck/cppcheck
08531000-08532000 r--p 004e8000 08:01 262163     /home/martin/cppcheck/cppcheck
08532000-08533000 rw-p 004e9000 08:01 262163     /home/martin/cppcheck/cppcheck
08533000-08576000 rw-p 00000000 00:00 0          [heap]
b7cbf000-b7cc1000 rw-p 00000000 00:00 0 
b7cc1000-b7e6f000 r-xp 00000000 08:01 1186597    /lib/i386-linux-gnu/libc-2.17.so
b7e6f000-b7e71000 r--p 001ae000 08:01 1186597    /lib/i386-linux-gnu/libc-2.17.so
b7e71000-b7e72000 rw-p 001b0000 08:01 1186597    /lib/i386-linux-gnu/libc-2.17.so
b7e72000-b7e75000 rw-p 00000000 00:00 0 
b7e75000-b7e90000 r-xp 00000000 08:01 1180850    /lib/i386-linux-gnu/libgcc_s.so.1
b7e90000-b7e91000 r--p 0001a000 08:01 1180850    /lib/i386-linux-gnu/libgcc_s.so.1
b7e91000-b7e92000 rw-p 0001b000 08:01 1180850    /lib/i386-linux-gnu/libgcc_s.so.1
b7e92000-b7e93000 rw-p 00000000 00:00 0 
b7e93000-b7ed4000 r-xp 00000000 08:01 1186613    /lib/i386-linux-gnu/libm-2.17.so
b7ed4000-b7ed5000 r--p 00040000 08:01 1186613    /lib/i386-linux-gnu/libm-2.17.so
b7ed5000-b7ed6000 rw-p 00041000 08:01 1186613    /lib/i386-linux-gnu/libm-2.17.so
b7ed6000-b7fb3000 r-xp 00000000 08:01 5119907    /usr/lib/i386-linux-gnu/libstdc++.so.6.0.18
b7fb3000-b7fb7000 r--p 000dc000 08:01 5119907    /usr/lib/i386-linux-gnu/libstdc++.so.6.0.18
b7fb7000-b7fb8000 rw-p 000e0000 08:01 5119907    /usr/lib/i386-linux-gnu/libstdc++.so.6.0.18
b7fb8000-b7fbf000 rw-p 00000000 00:00 0 
b7fd9000-b7fdd000 rw-p 00000000 00:00 0 
b7fdd000-b7fde000 r-xp 00000000 00:00 0          [vdso]
b7fde000-b7ffe000 r-xp 00000000 08:01 1186589    /lib/i386-linux-gnu/ld-2.17.so
b7ffe000-b7fff000 r--p 0001f000 08:01 1186589    /lib/i386-linux-gnu/ld-2.17.so
b7fff000-b8000000 rw-p 00020000 08:01 1186589    /lib/i386-linux-gnu/ld-2.17.so
bffdf000-c0000000 rw-p 00000000 00:00 0          [stack]

Program received signal SIGABRT, Aborted.
0xb7fdd424 in __kernel_vsyscall ()
(gdb) backtrace
#0  0xb7fdd424 in __kernel_vsyscall ()
#1  0xb7cefaff in __GI_raise (sig=sig@entry=6)
    at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
#2  0xb7cf3083 in __GI_abort () at abort.c:90
#3  0xb7d2ca95 in __libc_message (do_abort=do_abort@entry=2, 
    fmt=fmt@entry=0xb7e300d0 "*** Error in `%s': %s: 0x%s ***\n")
    at ../sysdeps/unix/sysv/linux/libc_fatal.c:199
#4  0xb7d377c2 in malloc_printerr (action=<optimized out>, 
    str=<optimized out>, ptr=0x853aa00) at malloc.c:4923
#5  0xb7d38510 in _int_free (av=0xb7e71440 <main_arena>, p=0x853a9f8, 
    have_lock=0) at malloc.c:3779
#6  0xb7f1ea3f in operator delete(void*) ()
   from /usr/lib/i386-linux-gnu/libstdc++.so.6
#7  0x08255e49 in __gnu_cxx::new_allocator<char>::deallocate (this=0xbfffdfdb, 
    __p=0x853aa00 "") at /usr/include/c++/4.8/ext/new_allocator.h:110
#8  0x08254480 in std::string::_Rep::_M_destroy (this=0x853aa00, __a=...)
    at /usr/include/c++/4.8/bits/basic_string.tcc:449
#9  0x08252536 in std::string::_Rep::_M_dispose (this=0x853aa00, __a=...)
    at /usr/include/c++/4.8/bits/basic_string.h:249
#10 0x082510bb in std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string (this=0x853a9c4, __in_chrg=<optimized out>)
    at /usr/include/c++/4.8/bits/basic_string.h:539
#11 0x083dfc1c in Token::~Token (this=0x853a998, __in_chrg=<optimized out>)
---Type <return> to continue, or q <return> to quit---
    at lib/token.cpp:64
#12 0x083e0602 in Token::deleteNext (this=0x853a918, index=0)
    at lib/token.cpp:184
#13 0x083e2d55 in Token::eraseTokens (begin=0x853a918, end=0x853aaa0)
    at lib/token.cpp:1017
#14 0x083d04e4 in TemplateSimplifier::useDefaultArgumentValues (templates=..., 
    templateInstantiations=0xbfffe224) at lib/templatesimplifier.cpp:607
#15 0x083d471d in TemplateSimplifier::simplifyTemplates (tokenlist=..., 
    errorlogger=..., _settings=0xbfffeba8, 
    _codeWithTemplates=@0xbfffe4c8: true) at lib/templatesimplifier.cpp:1285
#16 0x083f1bd1 in Tokenizer::simplifyTemplates (this=0xbfffe468)
    at lib/tokenize.cpp:2175
#17 0x083f84b0 in Tokenizer::simplifyTokenList1 (this=0xbfffe468, 
    FileName=0x8539f0c "crash196.cpp") at lib/tokenize.cpp:3358
#18 0x083eea05 in Tokenizer::tokenize (this=0xbfffe468, code=..., 
    FileName=0x8539f0c "crash196.cpp", configuration=...)
    at lib/tokenize.cpp:1596
#19 0x08357f8c in CppCheck::checkFile (this=0xbfffeb90, code=..., 
    FileName=0x8539f0c "crash196.cpp") at lib/cppcheck.cpp:354
#20 0x0835684a in CppCheck::processFile (this=0xbfffeb90, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:234
#21 0x0835535b in CppCheck::check (this=0xbfffeb90, path=...)
    at lib/cppcheck.cpp:65

*/
