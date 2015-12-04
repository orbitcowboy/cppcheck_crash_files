#include <cassert>
class ArgumentSource {
    union Storage {
        struct {
            char Value;
        } TheLV;
    } Storage;
    enum class Kind {
        RValue,
    } StoredKind;
    ArgumentSource() {
        switch (StoredKind) {
        case Kind::RValue:
	  ;
        }
    }
    char asKnownExpr() && {
        assert(StoredKind == Kind::RValue);
        return 0;
    }
}
