
#define LT_STMT_START        (void)(
#define LT_STMT_END          )

#define LT_DLFREE(p)						\
	    LT_STMT_START { if (p) (p) = (lt_dlfree (p), (lt_ptr) 0); } LT_STMT_END

void f(char ** dest)

{
    const char *end   = NULL;

    LT_DLFREE (*dest);
}

// rose compiler --> cppcheck crash
