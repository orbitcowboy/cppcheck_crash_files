#define DEFINE_PRIMITIVE_TYPE_ADAPTER(NativeType, JNIType, JNIName) \
    \
    template<> struct TypeAdapter<NativeType> { \
        using JNI##Type = JNIType; \
    \
        static constexpr auto Call = &JNIEnv::Call ## JNIName ## MethodA; \
    \
        static JNIType FromNative(JNIEnv*, NativeType val) { \
            return static_cast<JNIType>(val); \
        } \
    }


DEFINE_PRIMITIVE_TYPE_ADAPTER(bool,     jboolean, Boolean);

#undef DEFINE_PRIMITIVE_TYPE_ADAPTER
// from https://trac.cppcheck.net/ticket/8331
