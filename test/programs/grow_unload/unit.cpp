// an unloadable unit, built as a loadable module with MATCHABLE_UNLOADABLE. UNIT_VERSION 1 adds mike and
// zulu, regrows the program's kilo and is the first to set extra; UNIT_VERSION 2 adds zulu with another id,
// and yankee
#include "grown.h"



#if UNIT_VERSION == 1
GROW_MATCHABLE(Grown, mike, zulu, kilo)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, mike, id, 13)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, zulu, id, 26)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, base_a, extra, 7)
#else
GROW_MATCHABLE(Grown, yankee, zulu)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, yankee, id, 25)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, zulu, id, 52)
#endif
