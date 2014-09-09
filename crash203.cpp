#include <vector>
typedef std::vector<char> X;
X unicode_cast<X>(const X &in) {
	const X s = unicode_cast<X>(in);
	return unicode_cast<X>(s);
}

/*
Program received signal SIGSEGV, Segmentation fault.
0x0000000000672ab8 in Token::previous (this=0x0) at lib/token.h:414
414	        return _previous;
(gdb) backtrace
#0  0x0000000000672ab8 in Token::previous (this=0x0) at lib/token.h:414
#1  0x0000000000718fa3 in CheckOther::checkRedundantCopy (this=0x7fffffffc510)
    at lib/checkother.cpp:3192
#2  0x00000000006d517b in CheckOther::runSimplifiedChecks (
    this=0xb54d20 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffc690, 
    settings=0x7fffffffd4d0, errorLogger=0x7fffffffd4a0)
    at lib/checkother.h:108
#3  0x000000000075eb22 in CppCheck::checkFile (this=0x7fffffffd4a0, code=..., 
    FileName=0xb5de58 "desktop_util.cpp") at lib/cppcheck.cpp:395
#4  0x000000000075cf28 in CppCheck::processFile (this=0x7fffffffd4a0, 
    filename=..., fileContent=...) at lib/cppcheck.cpp:239
#5  0x000000000075b949 in CppCheck::check (this=0x7fffffffd4a0, path=...)
    at lib/cppcheck.cpp:66
#6  0x00000000006530ff in CppCheckExecutor::check_internal (
    this=0x7fffffffdd60, cppcheck=..., argv=0x7fffffffdf08)
    at cli/cppcheckexecutor.cpp:530
#7  0x000000000065205e in CppCheckExecutor::check (this=0x7fffffffdd60, 
    argc=3, argv=0x7fffffffdf08) at cli/cppcheckexecutor.cpp:180
#8  0x000000000065c346 in main (argc=3, argv=0x7fffffffdf08)
    at cli/main.cpp:122
*/
