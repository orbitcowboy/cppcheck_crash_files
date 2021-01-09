typedef void (*func) (addr) ;
void bar(void)
{
    func f ;
    f &  = (func) 42 ;
}
