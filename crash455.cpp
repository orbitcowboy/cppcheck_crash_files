struct A
{
  int f(const);               // { dg-error "" } ansi forbids no type
};
// from https://trac.cppcheck.net/ticket/8333
