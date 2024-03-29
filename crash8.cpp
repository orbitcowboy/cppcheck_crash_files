// { dg-options "-std=gnu++0x" }

struct Something {};
Something ___;

template <class F>
struct Trial
{
    F f;
public:
    Trial() : f() {}
    Trial( const F& ff ) : f(ff) { }
    template <typename... Args>
    struct Sig
    {
        typedef int ResultType;
    };

    template <typename... Args>
    struct Sig<Something,Args...>
    {
        typedef int ResultType;
    };

    template <typename... Args>
    typename Sig<Something,Args...>::ResultType operator()(const Something& s,
            const Args&... args) const
    {
        return f(args...);
    }
    template <typename... Args>
    typename Sig<Args...>::ResultType operator()(const Args&... args) const
    {
        return f(args...);
    }
};

struct Internal
{

    template <typename... Args>
    struct Sig
    {
        typedef int ResultType;
    };

    template <typename... Args>
    struct Sig<Something,Args...>
    {
        typedef int ResultType;
    };

    template <typename... Args>
    int operator()(const Args&... args) const
    {
        int n = sizeof...(Args);
        return n;
    }

    static Trial<Internal>& full()
    {
        static Trial<Internal> f;
        return f;
    }
};

static Trial<Internal>& internal = Internal::full();

int main()
{
    int n = 0;
    n = internal(___,1,2,3);
    return 0;
}
