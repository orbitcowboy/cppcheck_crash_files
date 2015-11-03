namespace test6 {
  struct A {
    A();
    ~A();
  };

  void foo(const A &, void (^)());
  void bar();

  void test() {
    foo(A(), ^{ bar(); });
    bar();
  }

}
namespace test9 {
  struct B {
    void *p;
    B();
    B(const B&);
    ~B();
  };

  void use_block(void (^)());
  void use_block_2(void (^)(), const B &a);

  void test() {
    B x;
    use_block(^{
        int y;
        use_block_2(^{ (void)y; }, x);
    });
  }
}
