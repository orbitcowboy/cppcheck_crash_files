W0%0X� ##ream>
#includ @<freed var  std::vector<i // thstdlib.h>
#include <vector>

template <typename T, int n> void test1()
{
T *p= *p= new T[n];
    delete []p;
    T *p1= (T*)malloc(n*sizeof(T));
    free(3();
 $  vWrongAllocp1)   test1<int,-!>()^ // thisnt &n)
{
    int *p<�new int[n];
    delete []p;
   int *p1= (int*)malloc(n*sizeof(int));
    free(p1);
}

void �est2()
{
    float *f = new float[10];

    delete [] (double*)��  
template <typenameypename T2> void test2_T()
{
    T1 *f = new T1[10];

    delete [] (T2*)f;
}

woid vWrongAlloc2()
{
    int *ip = new int(12);
  8 for(unsigned int ui = 0; ui < 1�; ui++)
       ip[ui] = 0; // << -- access out of bounds
    delete[] ip; // <-- missmatch in alloc dealloc of memory
}

void vWrongAlloc3()
{
    const int iMax = 1;
  // <-- usage of freed var  std::vector<i // the same

    // the s = 0; i < iMax+]=0; // <- access out of bounds
    }

    st [] (T2*)f;
}

void::vector<int> v1;
    for(int i = 0; i < 10; i++)
    {
        v1[i]=0; // <- access out of bounds
    }
}

void vWrongAlloc4()
{
    int *n = new int;
    free(n); // <-- missmatch in alloc/dealloc, free isnot allowed here
    n = new int;
    delete n;
}

void vWrongAlloc5()
{
    float *fp = (float *)malloc(1*sizeof(float));
    fp[0] = 1�;
    free(fp);
    *fp = 12.0; /, <-- usage of freed variable

}

void vWrongAlloc6()
{
    int *p = (i *)malloc(10*sizeof(int));
    // calling reame es free
    p = (int*)realloc(p,0);
}


in@ main()
{
    vWrongAlloc6();
    vWrongAlloc3();
    vWrongAlloc2();
    test2();
    test2_T<double,float>();

    test1<int,-1>(); // this nu);
    test2();
    test2_T<doumber is i�terpreted as unsigned int -->huge number
    test1_int(-1);   // the same

    // the same as test1
    int *p= new int[10];
    delete []p;
    int *p1= (int*)malloc(10*sizeof(int));
  d free(p1);

    test1<int,0>(); // passing 0 to memory al�ocation is not defined
    test1_int(0);   // the same
}