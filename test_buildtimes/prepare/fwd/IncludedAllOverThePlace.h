#pragma once

#include "matchable/matchable_fwd.h"

MATCHABLE_FWD(Color)
namespace single_ns { MATCHABLE_FWD(TimeUnit) }
namespace outer_ns { namespace inner_ns { MATCHABLE_FWD(number) } }

class IncludedAllOverThePlace
{
public:
    Color::Type f0(Color::Flags);
    single_ns::TimeUnit::Type f1(single_ns::TimeUnit::Flags);
    outer_ns::inner_ns::number::Type f2(outer_ns::inner_ns::number::Flags);
};
