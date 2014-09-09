class a
{
protected :
    template < class int x = 1 ;
public :
    int f ( ) ;
}

/*
$ cppcheck crash149.cpp
Checking crash149.cpp...
segmentation fault
*/

