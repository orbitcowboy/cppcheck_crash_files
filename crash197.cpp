args->d
void abort (void);
typedef struct
{
    long r[(19 + sizeof
            (long))/(sizeof (long))];
    extern realvaluetype;
    typedef void *tree;
    static realvaluetype real_value_from_int_cst
    (tree x, tree y)
    {
        realvaluetype r;
        int i;
        for
        (i = 0; i < sizeof(r.r)/sizeof(long); ++i) r.r[i] = -1;
        return r;
    } struct brfic_args
    {
        tree type;
        tree i;
        realvaluetype d;
    };
    static void build_real_from_int_cst_1 (data) void * data;
    {
        struct brfic_args *args = (struct brfic_args *) data;
    }
        = real_value_from_int_cst (args->type, args->i);
} int main()
{
    struct brfic_args
            args;
    __builtin_memset (&args, 0, sizeof(args));
    build_real_from_int_cst_1 (&args);
    if (args.d.r[0] ==
            0) abort ();
    return 0;
}

/*
 (gdb) run crash197.cpp
Starting program: /home/martin/cppcheck/cppcheck crash197.cpp
Checking crash197.cpp...
[crash197.cpp:9]: (error) syntax error

Program received signal SIGFPE, Arithmetic exception.
0xb7e7cd73 in __divdi3 () from /lib/i386-linux-gnu/libgcc_s.so.1
(gdb) backtrace
#0  0xb7e7cd73 in __divdi3 () from /lib/i386-linux-gnu/libgcc_s.so.1
#1  0x08439af1 in execute (expr=0x853d3d0, programMemory=0xbfffe214,
    result=0xbfffe1a8, error=0xbfffe1f5) at lib/valueflow.cpp:683
#2  0x0843955b in execute (expr=0x853af10, programMemory=0xbfffe214,
    result=0xbfffe208, error=0xbfffe1f5) at lib/valueflow.cpp:635
#3  0x0843a1b0 in valueFlowForLoop2 (tok=0x853d7d0, memory1=0xbfffe318,
    memory2=0xbfffe33c) at lib/valueflow.cpp:757
#4  0x0843a8a8 in valueFlowForLoop (tokenlist=0xbfffe468,
    errorLogger=0xbfffeb90, settings=0xbfffeba8) at lib/valueflow.cpp:838
#5  0x0843b248 in ValueFlow::setValues (tokenlist=0xbfffe468,
    errorLogger=0xbfffeb90, settings=0xbfffeba8) at lib/valueflow.cpp:923
#6  0x083eeb95 in Tokenizer::tokenize (this=0xbfffe468, code=...,
    FileName=0x8539f0c "crash197.cpp", configuration=...)
    at lib/tokenize.cpp:1611
#7  0x08357f8c in CppCheck::checkFile (this=0xbfffeb90, code=...,
    FileName=0x8539f0c "crash197.cpp") at lib/cppcheck.cpp:354
#8  0x0835684a in CppCheck::processFile (this=0xbfffeb90, filename=...,
    fileContent=...) at lib/cppcheck.cpp:234
#9  0x0835535b in CppCheck::check (this=0xbfffeb90, path=...)
    at lib/cppcheck.cpp:65
#10 0x0825a558 in CppCheckExecutor::check_internal (this=0xbffff068,
    cppCheck=..., argv=0xbffff164) at cli/cppcheckexecutor.cpp:418
#11 0x08259a5f in CppCheckExecutor::check (this=0xbffff068, argc=2,*/
