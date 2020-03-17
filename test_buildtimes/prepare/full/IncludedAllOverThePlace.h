#pragma once
#include "TimeUnit.h"
class IncludedAllOverThePlace
{
public:
    explicit IncludedAllOverThePlace(TimeUnit::Type initial_time_unit);
    virtual ~IncludedAllOverThePlace();
private:
    TimeUnit::Type tu;
};
