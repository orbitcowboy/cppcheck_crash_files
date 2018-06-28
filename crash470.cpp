int a()
{
    void b(char **dst)
    {
        *dst = malloc(50);
    }
}

// https://trac.cppcheck.net/ticket/8644
