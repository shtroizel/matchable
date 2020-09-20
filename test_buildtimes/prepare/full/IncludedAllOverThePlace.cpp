#include "IncludedAllOverThePlace.h"
#include "matchables.h"


Color::Type IncludedAllOverThePlace::f0(Color::Flags)
{
    return Color::nil;
}


single_ns::TimeUnit::Type IncludedAllOverThePlace::f1(single_ns::TimeUnit::Flags)
{
    return single_ns::TimeUnit::nil;
}


outer_ns::inner_ns::number::Type IncludedAllOverThePlace::f2(outer_ns::inner_ns::number::Flags flags)
{
    if (flags.currently_set().size() > 0)
        return flags.currently_set()[0];

    return outer_ns::inner_ns::number::nil;
}
