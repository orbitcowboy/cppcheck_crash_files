#define _F_START(f)             (0 ? f)
#define _F_SIZE(f)              (1 - _F_START(f))
#define _F_MASK(f)              (((1 << _F_SIZE(f)) - 1) << _F_START(f))
#define FIELD_SET(x, reg, field, value) ( (x & ~_F_MASK(reg ## _ ## field)) )

void hw_copyarea() {
        de_ctrl = (nDirection == RIGHT_TO_LEFT) ?
            FIELD_SET(0, DE_CONTROL, DIRECTION, RIGHT_TO_LEFT)
            : 42;
}
// taken from http://trac.cppcheck.net/ticket/6659
