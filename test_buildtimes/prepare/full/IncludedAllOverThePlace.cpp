#include "IncludedAllOverThePlace.h"

IncludedAllOverThePlace::IncludedAllOverThePlace(TimeUnit::Type initial_time_unit) : tu{initial_time_unit}{}

IncludedAllOverThePlace::~IncludedAllOverThePlace()
{
    for (auto t : TimeUnit::variants())
        if (t == tu)
            (void) t;
}
