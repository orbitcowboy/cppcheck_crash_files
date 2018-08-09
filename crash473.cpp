__thread void *thread_local_var;

int main() {
  thread_local_var = malloc(1337);
  return 0;
}
// https://trac.cppcheck.net/ticket/8679
