void f()
{
    const size_t kDefaultStackQuota = 128 * sizeof(size_t) * 1024;
    const size_t kTrustedScriptBuffer = sizeof(size_t) * 12800;
    (void) kDefaultStackQuota;
}
// https://trac.cppcheck.net/ticket/8579
