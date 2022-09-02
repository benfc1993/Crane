#ifdef CR_ENABLE_ASSERTS
#define CR_ASSERT(x, ...)                                   \
    {                                                       \
        if (!(x))                                           \
        {                                                   \
            CR_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
        }                                                   \
    }
#define CR_CORE_ASSERT(x, ...)                                   \
    {                                                            \
        if (!(x))                                                \
        {                                                        \
            CR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
        }                                                        \
    }
#else
#define CR_ASSERT(x, ...)
#define CR_CORE_ASSERT(x, ...)
#endif