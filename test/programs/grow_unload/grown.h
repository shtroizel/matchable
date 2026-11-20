#pragma once

#include "matchable/matchable.h"



// declared in the program; growth units add variants and are unloaded again. extra is never touched by the
// program before a unit sets it, so a unit is the first to build its storage
PROPERTYx2_MATCHABLE(int, id, int, extra, Grown, base_a, base_b)
