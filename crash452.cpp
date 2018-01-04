template <int> struct Tag {};

template <int ID>
using SPtr = std::shared_ptr<void(Tag<ID>)>;

int main() {
    SPtr<0> s; // OK
}
// https://trac.cppcheck.net/ticket/8315
