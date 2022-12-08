#pragma once

#include "matchable/matchable.h"


MATCHABLE(Color, Red, Green, Blue)
namespace single_ns { MATCHABLE(TimeUnit, Seconds, Minutes, Hours) }
namespace outer_ns { namespace inner_ns { MATCHABLE(number, seventeen, hundredseven) } }
