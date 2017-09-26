#include <set>
#include <inttypes.h>
#include <cstddef>
#include <bitset>
#include <limits>
#include <iostream>
template <typename T> class H {
public:
    static std::multiset<int> Bar() {
        std::multiset<int> result;
        for (T i = 0; i < (std::numeric_limits<T>::max)(); ++i) {
            for (T j = 0; j < (std::numeric_limits<T>::max)(); ++j) {
                if (i == j) {
                    continue;
                }
            }
        }
        return result;
    }
};
int main(void) {
    const std::multiset<int> result = H<int>::Bar();
    return 0;
}
