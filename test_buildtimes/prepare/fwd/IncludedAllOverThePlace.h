#pragma once

#include "matchable_fwd.h"

MATCHABLE_FWD(TimeUnit)
MATCHABLES_MERGE_SPREADS_FWD(Unit)

class IncludedAllOverThePlace
{
public:
    TimeUnit::Type foo();
    Unit::Type another_foo();
};
