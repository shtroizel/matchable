#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(
    Number
    , zero
    , one
    , two
    , three
    , four
    , five
    , six
    , seven
    , eight
    , nine
    , ten
    , eleven
    , twelve
    , thirteen
    , fourteen
    , fifteen
    , sixteen
    , seventeen
    , eighteen
    , nineteen
    , twenty
    , twenty_mns_one
    , twenty_mns_two
    , twenty_mns_three
    , twenty_mns_four
    , twenty_mns_five
    , twenty_mns_six
    , twenty_mns_seven
    , twenty_mns_eight
    , twenty_mns_nine
    , thirty
    , thirty_mns_one
    , thirty_mns_two
    , thirty_mns_three
    , thirty_mns_four
    , thirty_mns_five
    , thirty_mns_six
    , thirty_mns_seven
    , thirty_mns_eight
    , thirty_mns_nine
    , fourty
    , fourty_mns_one
    , fourty_mns_two
    , fourty_mns_three
    , fourty_mns_four
    , fourty_mns_five
    , fourty_mns_six
    , fourty_mns_seven
    , fourty_mns_eight
    , fourty_mns_nine
    , fifty
    , fifty_mns_one
    , fifty_mns_two
    , fifty_mns_three
    , fifty_mns_four
    , fifty_mns_five
    , fifty_mns_six
    , fifty_mns_seven
    , fifty_mns_eight
    , fifty_mns_nine
    , sixty
    , sixty_mns_one
    , sixty_mns_two
    , sixty_mns_three
    , sixty_mns_four
    , sixty_mns_five
    , sixty_mns_six
    , sixty_mns_seven
    , sixty_mns_eight
    , sixty_mns_nine
    , seventy
    , seventy_mns_one
    , seventy_mns_two
    , seventy_mns_three
    , seventy_mns_four
    , seventy_mns_five
    , seventy_mns_six
    , seventy_mns_seven
    , seventy_mns_eight
    , seventy_mns_nine
    , eighty
    , eighty_mns_one
    , eighty_mns_two
    , eighty_mns_three
    , eighty_mns_four
    , eighty_mns_five
    , eighty_mns_six
    , eighty_mns_seven
    , eighty_mns_eight
    , eighty_mns_nine
    , ninety
    , ninety_mns_one
    , ninety_mns_two
    , ninety_mns_three
    , ninety_mns_four
    , ninety_mns_five
    , ninety_mns_six
    , ninety_mns_seven
    , ninety_mns_eight
    , ninety_mns_nine
    , one_spc_hundred
    , one_spc_hundred_spc_and_spc_one
    , one_spc_hundred_spc_and_spc_two
    , one_spc_hundred_spc_and_spc_three
    , one_spc_hundred_spc_and_spc_four
    , one_spc_hundred_spc_and_spc_five
    , one_spc_hundred_spc_and_spc_six
    , one_spc_hundred_spc_and_spc_seven
);


// grow beyond 108 variants...
GROW_MATCHABLE(
    Number,
    one_spc_hundred_spc_and_spc_eight,
    one_spc_hundred_spc_and_spc_nine,
    one_spc_hundred_spc_and_spc_ten
)



int main()
{
    test_ok ok;

    TEST_EQ(ok, Number::variants().size(), static_cast<size_t>(111));

    Number::Type n;
    for (auto number : Number::variants())
    {
        n = Number::from_string(number.as_string());
        TEST_EQ(ok, n, number);
        n = Number::from_index(number.as_index());
        TEST_EQ(ok, n, number);

        std::cout << "index " << n.as_index() << ": " << n << std::endl;
    }

    return ok();
}
