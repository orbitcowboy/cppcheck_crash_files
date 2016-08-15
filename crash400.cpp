int x __attribute__((aligned(3))); // expected-error {{requested alignment is not a power of 2}}
char a = 0;
char a3[sizeof(a) == 1? : -1] = { 0 };
