class Test {
	union Bla {
		struct {}
	} Bla;
	enum class Foo : unsigned char { qux } baz;
	Test &operator=(Test &&ttest) {
		switch (baz) {
			case Foo::qux:
		}
	}
	buzz *bbla() && { assert(baz == Foo::buzz); }
};
