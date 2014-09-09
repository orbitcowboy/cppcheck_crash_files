namespace boost
{
namespace iostreams
{
namespace test
{

// Simple exception class with error code built in to type
template<int Code>
struct operation_error { };

class operation_sequence;


inline std::string operation_sequence::message() const
{
    using namespace std;
    if (success_)
        return "success";
    std::string msg = failed_ ?
                      "operations occurred out of order: " :
                      "operation sequence is incomplete: ";
    typedef vector<int>::size_type size_type;
    for (size_type z = 0, n = log_.size(); z < n; ++z)
    {
        msg += lexical_cast<string>(log_[z]);
        if (z < n - 1)
            msg += ',';
    }
    return msg;
}

inline void operation_sequence::reset()
{
    log_.clear();
    total_executed_ = 0;
    last_executed_ = INT_MIN;
    success_ = false;
    failed_ = false;
}


}
}
} // End namespace boost::iostreams::test.

#endif // #ifndef BOOST_IOSTREAMS_TEST_OPERATION_SEQUENCE_HPP_INCLUDED




using namespace std;
using namespace boost::iostreams;
using namespace boost::iostreams::detail;
using namespace boost::iostreams::test;
using boost::unit_test::test_suite;
namespace io = boost::iostreams;

const int max_length = 30;
const unsigned int pattern_length = 100;
const unsigned int pattern_reps = 100;

template<typename Codecvt>
bool valid_char(typename codecvt_intern<Codecvt>::type c)
{
    typedef typename codecvt_state<Codecvt>::type   state_type;
    typedef typename codecvt_intern<Codecvt>::type  intern_type;
    Codecvt             cvt;
    state_type          state = state_type();
    const intern_type*  nint;
    char*               next;
    char                buf[max_length];

    return cvt.out( state, &c, &c + 1, nint,
                    buf, buf + max_length, next )
           ==
           codecvt_base::ok;
}

template<typename Codecvt>
basic_string<
BOOST_DEDUCED_TYPENAME
codecvt_intern<Codecvt>::type
>
test_string()
{
    typedef typename codecvt_intern<Codecvt>::type intern_type;
    std::basic_string<intern_type> pattern, result;
    for (intern_type c = 255; pattern.size() < pattern_length; --c)
        if (valid_char<Codecvt>(c))
            pattern += c;
    result.reserve(pattern.size() * pattern_reps);
    for (unsigned int w = 0; w < pattern_reps; ++w)
        result += pattern;
    return result;
}

// Como can't compile file_descriptor.cpp in strict mode; this failure
// is detected by file_descriptor_test.cpp.
#if !defined(__COMO__) || !defined(BOOST_COMO_STRICT)
typedef io::file_descriptor_source  classic_file_source;
typedef io::file_descriptor_sink    classic_file_sink;
#else
struct classic_file_source : io::source
{
    classic_file_source(const std::string& path)
        : file_(new filebuf)
    {
        file_->pubimbue(locale::classic());
        file_->open(path.c_str(), BOOST_IOS::in | BOOST_IOS::binary);
    }
    streamsize read(char* s, streamsize n)
    {
        return file_->sgetn(s, n);
    }
    boost::shared_ptr<filebuf> file_;
};

struct classic_file_sink : io::sink
{
    classic_file_sink(const std::string& path)
        : file_(new filebuf)
    {
        file_->pubimbue(locale::classic());
        file_->open(path.c_str(), BOOST_IOS::out | BOOST_IOS::binary);
    }
    streamsize write(const char* s, streamsize n)
    {
        return file_->sputn(s, n);
    }
    boost::shared_ptr<filebuf> file_;
};
#endif

template<typename Codecvt>
bool codecvt_test1()
{
    typedef basic_string<
    BOOST_DEDUCED_TYPENAME
    codecvt_intern<Codecvt>::type
    >                                             string_type;
    typedef code_converter<classic_file_source, Codecvt>  wide_file_source;
    typedef code_converter<classic_file_sink, Codecvt>    wide_file_sink;

    BOOST_CHECK(Codecvt().max_length() <= max_length);
    temp_file                 temp;
    string_type               test = test_string<Codecvt>();
    stream<wide_file_sink>    out(temp.name());
    out.write(test.data(), static_cast<streamsize>(test.size()));
    out.close();

    stream<wide_file_source>  in(temp.name());
    string_type               test2;
    io::copy(in, io::back_inserter(test2));

    return test == test2;
}

template<typename Codecvt>
bool codecvt_test2()
{
    typedef basic_string<
    BOOST_DEDUCED_TYPENAME
    codecvt_intern<Codecvt>::type
    >                                    string_type;
    typedef code_converter<classic_file_source>  wide_file_source;
    typedef code_converter<classic_file_sink>    wide_file_sink;

    // Set global locale.
    locale loc = add_facet(locale(), new Codecvt);
    locale::global(loc);

    temp_file                 temp;
    string_type               test = test_string<Codecvt>();
    stream<wide_file_sink>    out(temp.name());
    out.write(test.data(), static_cast<streamsize>(test.size()));
    out.close();

    stream<wide_file_source>  in(temp.name());
    string_type               test2;
    io::copy(in, io::back_inserter(test2));

    return test == test2;
}

template<typename Codecvt>
bool codecvt_test()
{
    return codecvt_test1<Codecvt>() && codecvt_test2<Codecvt>();
}


test_suite* init_unit_test_suite(int, char* [])
{
    test_suite* test = BOOST_TEST_SUITE("code_converter test");
    test->add(BOOST_TEST_CASE(&code_converter_test));
    //test->add(BOOST_TEST_CASE(&close_test));
    return test;
}
