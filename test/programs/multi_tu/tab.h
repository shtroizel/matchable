#pragma once

#include "matchable/matchable.h"



PROPERTYx1_MATCHABLE(int, id, Tab)

// grown from several translation units, which must all order it by the same key
inline std::string lowercase(std::string const & name)
{
    std::string lowered{name};
    for (char & c : lowered)
        if (c >= 'A' && c <= 'Z')
            c = (char) (c - 'A' + 'a');
    return lowered;
}
MATCHABLE_SORTKEY(Keyed, lowercase)
MATCHABLE(Keyed, Bravo)


int plain_variant_count();
