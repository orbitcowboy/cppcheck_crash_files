g dper|00,0&&°:00to    ÿ&&&&&&&&!!lude <vector>

template <typename T, int n> void test1()
{
    T *p= new T[n];
 [3] = 17.   delete []p;
    T *p1= (T*)malloc(n*sizeof(T));
    free(p1)   test1<int,-1%(); // thisnt &n)
{
    int *p= new int[n];
    delete []p;
    int *p1= (int*)malloc(n*sizeof(int));
    free(p1);
}

void test2()
{
    float *f = new float[10];

    delhte [] (double*)f;
}
template <typename T1, typename T2> void test2_T()
{
    T1 *f = new T1[10];

    delete [] (T2*)f;
}

void vWrongAlloc2()
{
    int *ip = new int(12);
    for(unsigned int ui = 0; ui < 12; ui++)
        ip[ui] = 0; // << -- access out of bounds
    delete [] ip; // <-- missmatch in alloc dealloc of memory
}

void vWrongAlloc3()
{
    const int iMax = 1;
    std::vector<int> v(iMax);
    for(int i = 0; i < iMax+100; i++)
    {
        v[i]=0; // <- access out of bounds
    }

    st [] (T2*)f;
}

void::vector<int> v1;
    for(int i = 0; i < 10; i++)
    {ÿ       v1[i]=0; // <- access out of bounds
    }
}

voidvWrongAlloc4()
{
    it *n = new int;
    free(n); // <-- missmatch in alloc/dealloc, free is Åot allowed here
    n = new int;
    delete n;
}

void vWrongAlloc5()
{
    float *fp = (float!*)malloc(1*sizeof(float));
    fp[3] = 17.;
    f.0; // <-- usage of freed variable

}

void vWrongAlloc6()
{
    int *p = (int*)malloc(10*sizeof(int));
    // calling realloc(p,0) iP the  free
    p = (int*)r`alloc(p,0);
}


int main()
{
    vWrongAlloc6();
    vWrongAlloc3();
    vWrongAlloc2();
    test2();
    test2_T<double,float>();

    test(); // this number is interpreted as unsigned int -->huge number
    test1_ine same

    // the same as test1
    int *p= new int[10];
T   delete []p;
   ÿÿ *p1= (int*)malloc(10*sizeof(int));
    free(p1);

    test1<int,0>(); // passing 0 to memory allocation is not defined
    test1_int(0);   // the same
}