#include "matchable/matchable_fwd.h"

#if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION) || defined(_MSVC_STL_VERSION)
#error "matchable_fwd.h includes a C++ standard library header"
#endif


MATCHABLE_FWD(Shape)
namespace outer_ns { namespace inner_ns { MATCHABLE_FWD(Size) } }

Shape::Type pick_shape(Shape::Flags const &);
outer_ns::inner_ns::Size::Type pick_size(outer_ns::inner_ns::Size::Flags const &, Shape::Type const &);


int main()
{
    return 0;
}
