namespace test16 {
  // CHECK-DAG: define linkonce_odr void @_ZN6test163zedIZNS_3fooIiE3barEvE1SEEvv(
  template <class T> void zed() {}
  template <class T> struct foo {
    static void *bar();
  };
  template <class T> void *foo<T>::bar() {
    class S {
    };
    return reinterpret_cast<void *>(zed<S>);
  }
  void *test() { return foo<int>::bar(); }
}
