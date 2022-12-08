#include <iostream>

#include "matchable/matchable.h"
#include "matchable/matchable_fwd.h"
#include "test_ok.h"



// forward declare is enough for property of type self
MATCHABLE_FWD(ppl)

PROPERTYx2_MATCHABLE(ppl::Type, friends, int, favorite_number, ppl, Alice, Bob, Charlie)
MATCHABLE_VARIANT_PROPERTY_VALUES(ppl, Alice, friends, ppl::Bob::grab(), ppl::Charlie::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(ppl, Alice, favorite_number, 478)
MATCHABLE_VARIANT_PROPERTY_VALUE(ppl, Bob, favorite_number, 275)
MATCHABLE_VARIANT_PROPERTY_VALUE(ppl, Charlie, favorite_number, 696)



int main()
{
    test_ok ok;

    int sum{0};

    std::cout << "friends: " << std::endl;
    for (auto f : ppl::Alice::grab().as_friends_vect())
    {
        sum += f.as_favorite_number();
        std::cout << "    " << f.as_favorite_number() << " is Alice's friend " << f
                  << "'s favorite number" << std::endl;
    }

    TEST_EQ(ok, sum, 971);

    return ok();
}
