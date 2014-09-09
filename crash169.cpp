struct V :
{
public case {} ;
    struct U :
            U  void
    {
        V *f (int x) (x)
    }
}

/*$ cppcheck crash169.cpp
Checking crash169.cpp...
Segmentation fault (core dumped)
*/

/*
  run crash169.cpp
Starting program: /usr/bin/cppcheck crash169.cpp
Checking crash169.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7b843c3 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
(gdb) backtrace
#0  0x00007ffff7b843c3 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x00007ffff7b84469 in __gnu_debug::_Safe_sequence_base::_M_get_mutex() ()
   from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#2  0x00007ffff7b84688 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#3  0x000000000040b8ec in __gnu_debug::_Safe_iterator_base::_Safe_iterator_base
    (this=0x7fffff7ff360, __seq=0x907228, __constant=true)
    at /usr/include/c++/4.8/debug/safe_base.h:89
#4  0x000000000046f293 in __gnu_debug::_Safe_iterator<std::__cxx1998::_List_const_iterator<Function>, std::__debug::list<Function, std::allocator<Function> > >::_Safe_iterator (this=0x7fffff7ff360, __i=..., __seq=0x907218)
    at /usr/include/c++/4.8/debug/safe_iterator.h:152
#5  0x000000000046b343 in std::__debug::list<Function, std::allocator<Function> >::begin (this=0x907218) at /usr/include/c++/4.8/debug/list:189
#6  0x000000000058d6ce in Scope::findFunction (this=0x9071f0, tok=0x902c30)
    at lib/symboldatabase.cpp:2487
#7  0x000000000058d99f in Scope::findFunction (this=0x9071f0, tok=0x902c30)
    at lib/symboldatabase.cpp:2513
#8  0x000000000058d99f in Scope::findFunction (this=0x9071f0, tok=0x902c30)
    at lib/symboldatabase.cpp:2513
#9  0x000000000058d99f in Scope::findFunction (this=0x9071f0, tok=0x902c30)
    at lib/symboldatabase.cpp:2513
#10 0x000000000058d99f in Scope::findFunction (this=0x9071f0, tok=0x902c30)
    at lib/symboldatabase.cpp:2513
*/
