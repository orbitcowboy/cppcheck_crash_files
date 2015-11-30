template<typename... Ts> auto unary_right_comma (Ts... ts) { return (ts , ...); }
template<typename T, typename... Ts> auto binary_left_comma (T x, Ts... ts) { return (x , ... , ts); }
int main() {
  unary_right_comma (a);
}
