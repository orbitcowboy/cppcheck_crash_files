namespace Foo {
  enum BarConfig {
    eBitOne = (1 << 0),
    eBitTwo = (1 << 1),
// this will work
//    eAll        = (eBitOne|eBitTwo)
// and this will crash
    eAll        = eBitOne|eBitTwo
  };
}

void checkModifed(Foo::BarConfig flags)
{
}

void test()
{
  checkModifed(Foo::eAll);
}

// from ticket http://trac.cppcheck.net/ticket/5212
