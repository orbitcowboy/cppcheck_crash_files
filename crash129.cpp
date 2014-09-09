void h(int l)
{
    while
}

/*
$ cppcheck --debug crash129.cpp
Checking crash129.cpp...


##file crash129.cpp
1: void h ( int l@1 )
2: {
3: while
4: }

Speicherzugriffsfehler (Speicherabzug geschrieben)

*/
