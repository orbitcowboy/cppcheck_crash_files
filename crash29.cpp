X #define
{
    (
        for(  #endif typedef typedef cb[N] )
        ca[N]; =  cb[i]
    )
    }
/*
$ cppcheck --debug crash29.cpp
Checking crash29.cpp...
/usr/local/lib/gcc/i686-pc-linux-gnu/4.6.0/../../../../include/c++/4.6.0/bits/stl_stack.h:158:
    error: attempt to access an element in an empty container.

Objects involved in the operation:
sequence "this" @ 0x0xbfb61ab0 {
  type = St5stackIP5TokenNSt7__debug5dequeIS1_SaIS1_EEEE;
}
Aborted

*/
