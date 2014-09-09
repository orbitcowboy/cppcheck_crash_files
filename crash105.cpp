void f()
{
    const char *cx16 = has_cmpxchg16b ? " -mcx16" : " -mno-cx16";
}

// gcc testsuite: /gcc/gcc/config/i386/driver-i386.c
// segfault of cppcheck
