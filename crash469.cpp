class MyClass
{
    struct myLess
    {
        bool operator() ( const MyClass& __x)
        {
            return __x.comp(_mx);
        }
        int _mx;
    };
	bool comp( int) const;
};

using namespace NS;

// refer to https://github.com/danmar/cppcheck/commit/e4a70f87e67efaede1f118cdd99599920be9b415#commitcomment-29498492
