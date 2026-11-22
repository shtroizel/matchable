#pragma once

#include <string>



// the sort key matchable_maker_generated's header names: the name with ASCII letters lowercased
inline std::string fold_case(std::string const & name)
{
    std::string folded{name};
    for (char & c : folded)
        if (c >= 'A' && c <= 'Z')
            c = (char) (c - 'A' + 'a');
    return folded;
}
