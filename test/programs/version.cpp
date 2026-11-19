#include "matchable/matchable.h"
#include "test_ok.h"



#if MATCHABLE_VERSION_MAJOR != CMAKE_VERSION_MAJOR
#error "MATCHABLE_VERSION_MAJOR differs from the CMake project version"
#endif

#if MATCHABLE_VERSION_MINOR != CMAKE_VERSION_MINOR
#error "MATCHABLE_VERSION_MINOR differs from the CMake project version"
#endif

#if MATCHABLE_VERSION_PATCH != CMAKE_VERSION_PATCH
#error "MATCHABLE_VERSION_PATCH differs from the CMake project version"
#endif



int main()
{
    test_ok ok;

    TEST_EQ(ok, MATCHABLE_VERSION_MAJOR, CMAKE_VERSION_MAJOR);
    TEST_EQ(ok, MATCHABLE_VERSION_MINOR, CMAKE_VERSION_MINOR);
    TEST_EQ(ok, MATCHABLE_VERSION_PATCH, CMAKE_VERSION_PATCH);

    return ok();
}
