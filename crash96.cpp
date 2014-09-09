void f()
{
    typedef void (* x)(long);

    std::vector<x>::const_iterator it = callbacklist.begin(),	end = callbacklist.end();
}



