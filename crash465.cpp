class C;

void f() {
  C* c = new C{};
  boost::shared_ptr<C> a{c, [](C*) {}};
}
// https://trac.cppcheck.net/ticket/8577
