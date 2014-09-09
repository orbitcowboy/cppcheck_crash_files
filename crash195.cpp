int ScopedEnum { template<typename T> { { e = T::error }; };
    ScopedEnum1<int> se1;
    {
        enum class E : T { e = 0 = e ScopedEnum2<void*> struct UnscopedEnum3 { T { e = 4 }; }; arr[(int)E::e]; };
        UnscopedEnum3<int> e2 = f()
        {
            {
                e = e1;
                T::error
            }
            int test1 ue2;
            g()
            {
                enum class E
                {
                    e = T::error
                };
                return E::e;
            }
            int test2 =
        }
        namespace UnscopedEnum { template<typename T> struct UnscopedEnum1 { E { e = T::error }; }; UnscopedEnum1<int> { enum E : { e = 0 }; }; UnscopedEnum2<void*> ue3; template<typename T> struct UnscopedEnum3 { enum { }; }; int arr[E::e]; };
        UnscopedEnum3<int> namespace template<typename T> int f()
        {
            enum E { e };
            T::error
        };
        return (int)E();
    } int test1 int g()
    {
        enum E { e = E };
        E::e;
    }
    int test2 = g<int>();
}

/*
 (gdb) backtrace
#0  0x082756b2 in Token::next (this=0x0) at lib/token.h:357
#1  0x0840f694 in Tokenizer::simplifyEnum (this=0xbfffe428)
    at lib/tokenize.cpp:7602
#2  0x083f7e87 in Tokenizer::simplifyTokenList1 (this=0xbfffe428,
    FileName=0x853a024 "/media/martin/0c1d854e-681c-43b7-9bbc-1ee5005b6c46/projects/cppcheck_testfiles/bugs/crash195.cpp") at lib/tokenize.cpp:3237
#3  0x083eea05 in Tokenizer::tokenize (this=0xbfffe428, code=...,
    FileName=0x853a024 "/media/martin/0c1d854e-681c-43b7-9bbc-1ee5005b6c46/projects/cppcheck_testfiles/bugs/crash195.cpp", configuration=...)
    at lib/tokenize.cpp:1596
#4  0x08357f8c in CppCheck::checkFile (this=0xbfffeb50, code=...,
    FileName=0x853a024 "/media/martin/0c1d854e-681c-43b7-9bbc-1ee5005b6c46/projects/cppcheck_testfiles/bugs/crash195.cpp") at lib/cppcheck.cpp:354
#5  0x0835684a in CppCheck::processFile (this=0xbfffeb50, filename=...,
    fileContent=...) at lib/cppcheck.cpp:234
#6  0x0835535b in CppCheck::check (this=0xbfffeb50, path=...)
    at lib/cppcheck.cpp:65
#7  0x0825a558 in CppCheckExecutor::check_internal (this=0xbffff028,
    cppCheck=..., argv=0xbffff124) at cli/cppcheckexecutor.cpp:418
#8  0x08259a5f in CppCheckExecutor::check (this=0xbffff028, argc=2,
    argv=0xbffff124) at cli/cppcheckexecutor.cpp:179
#9  0x082629ec in main (argc=2, argv=0xbffff124) at cli/main.cpp:117
*/
