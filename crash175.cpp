typedef struct ct_data_s
{
    union
    {
        char  freq;
        char  code;
    } fc;
} ct_data;

typedef struct internal_state
{

    struct ct_data_s dyn_ltree[10];
} deflate_state;

void f(deflate_state *s)
{
    s->dyn_ltree[0].fc.freq++;
}

// from python-3.3.2 sources
/*
 * $ cppcheck --enable=all crash175.cpp
Checking crash175.cpp...
Segmentation fault (core dumped)

*/
/*
 (gdb) run --enable=all crash175.cpp
Starting program: /usr/bin/cppcheck --enable=all crash175.cpp
Checking crash175.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7b8492d in __gnu_debug::_Safe_iterator_base::_M_attach(__gnu_debug::_Safe_sequence_base*, bool) () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
(gdb) backtrace
#0  0x00007ffff7b8492d in __gnu_debug::_Safe_iterator_base::_M_attach(__gnu_debug::_Safe_sequence_base*, bool) () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x000000000040b84a in __gnu_debug::_Safe_iterator_base::_Safe_iterator_base (this=0x7fffff7ff360, __seq=0x7fffffffc420, __constant=false) at /usr/include/c++/4.8/debug/safe_base.h:89
#2  0x00000000004e8d6f in __gnu_debug::_Safe_iterator<std::_Rb_tree_iterator<std::pair<unsigned int const, std::__debug::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > > >, std::__debug::map<unsigned int, std::__debug::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::__debug::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > > > > >::_Safe_iterator (this=0x7fffff7ff360, __i=..., __seq=0x7fffffffc3f0)
    at /usr/include/c++/4.8/debug/safe_iterator.h:152
#3  0x00000000004e5f00 in std::__debug::map<unsigned int, std::__debug::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::__debug::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > > > >::find (this=0x7fffffffc3f0, __x=@0x7fffff7ff31c: 0)
    at /usr/include/c++/4.8/debug/map.h:382
#4  0x00000000004cc316 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:599
#5  0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#6  0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#7  0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#8  0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#9  0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#10 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#11 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#12 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#13 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#14 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#15 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#16 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#17 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#18 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#19 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
#20 0x00000000004cc3f9 in eraseMemberAssignments (varId=0, membervars=..., varAssignments=...) at lib/checkother.cpp:604
* /
