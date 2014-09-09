// from gcc/testsuite/g++.dg/cpp0x/variadic-nondeduce2.C
// PR c++/55220
// { dg-do compile { target c++11 } }

template <typename ...> struct something_like_tuple
{};
template <typename, typename> struct is_last {
  static const bool value = false;
};
template <typename T, template <typename ...> class Tuple, typename ... Head>
struct is_last<T, Tuple<Head ..., T>>
{
  static const bool value = true;
};

#define SA(X) static_assert (X, #X)

typedef something_like_tuple<char, int, float> something_like_tuple_t;
SA ((is_last<float, something_like_tuple_t>::value == false));
SA ((is_last<int, something_like_tuple_t>::value == false));

/*
 Checking crash211.cpp...

Program received signal SIGSEGV, Segmentation fault.
__memcpy_ssse3_rep () at ../sysdeps/i386/i686/multiarch/memcpy-ssse3-rep.S:1287
1287	../sysdeps/i386/i686/multiarch/memcpy-ssse3-rep.S: No such file or directory.
(gdb) bt
#0  __memcpy_ssse3_rep ()
    at ../sysdeps/i386/i686/multiarch/memcpy-ssse3-rep.S:1287
#1  0x0825fddb in std::char_traits<char>::copy (__s1=0xaf72d014 "char", 
    __s2=0x8541ca4 "char", __n=139729944)
    at /usr/include/c++/4.9/bits/char_traits.h:275
#2  0x0826398d in std::string::_M_copy (__d=0xaf72d014 "char", 
    __s=0x8541ca4 "char", __n=139729944)
    at /usr/include/c++/4.9/bits/basic_string.h:361
#3  0x08263ab1 in std::string::_Rep::_M_clone (this=0x8541c98, __alloc=..., 
    __res=0) at /usr/include/c++/4.9/bits/basic_string.tcc:631
#4  0x08261cce in std::string::_Rep::_M_grab (this=0x8541c98, __alloc1=..., 
    __alloc2=...) at /usr/include/c++/4.9/bits/basic_string.h:229
#5  0x08260362 in std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string (this=0xbfffdf58, __str=...)
    at /usr/include/c++/4.9/bits/basic_string.tcc:173
#6  0x083d59fe in TemplateSimplifier::simplifyTemplateInstantiations (
    tokenlist=..., errorlogger=0xbfffeb3c, _settings=0xbfffeb54, 
    tok=0x853dfc8, templateInstantiations=..., expandedtemplates=...)
    at lib/templatesimplifier.cpp:1232
#7  0x083d69d8 in TemplateSimplifier::simplifyTemplates (tokenlist=..., 
    errorlogger=0xbfffeb3c, _settings=0xbfffeb54, 
    _codeWithTemplates=@0xbfffe39c: true) at lib/templatesimplifier.cpp:1379
#8  0x083f5d1f in Tokenizer::simplifyTemplates (this=0xbfffe338)
*/
