void set_cur_cpu_spec()
{
        struct cpu_spec *t = &the_cpu_spec;
        t = PTRRELOC(t);
}
struct cpu_spec * __init setup_cpu_spec()
{
        struct cpu_spec *t = &the_cpu_spec;
        *PTRRELOC(&cur_cpu_spec) = &the_cpu_spec;
        return t;
}

// taken from https://trac.cppcheck.net/ticket/8527
