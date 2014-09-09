
void foo (int* arrayPtr)
{
    if (pipe (arrayPtr) < 0)
    {}
}


// from /gcc/fixincludes/procopen.c
/*

$ cppcheck --enable=all --std=posix --debug crash159.cpp
Checking crash159.cpp...


##file crash159.cpp
1:
2: void foo ( int * arrayPtr@1 )
3: {
4: if ( pipe ( arrayPtr@1 ) < 0 )
5: { }
6: }

/usr/include/c++/4.7/debug/vector:343:error: attempt to subscript container
    with out-of-bounds index 0, but container only holds 0 elements.

Objects involved in the operation:
sequence "this" @ 0x0x1bea7a8 {
  type = NSt7__debug6vectorI9DimensionSaIS1_EEE;
}
Abgebrochen (Speicherabzug geschrieben)


*/
