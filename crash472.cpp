template <class T1, class T2>
void f()
  {
  using T3 = typename T1::template T3<T2>;
  T3 t;
}
// from https://trac.cppcheck.net/ticket/8663
