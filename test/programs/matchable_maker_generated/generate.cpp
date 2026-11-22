// generate <header>: saves the matchables that main.cpp checks, during the build
#include <string>
#include <vector>

#include "matchable/MatchableMaker.h"



int main(int argc, char ** argv)
{
    if (argc != 2)
        return 2;

    matchable::MatchableMaker mm;
    mm.includes.push_back("matchable_maker_key.h");

    // more than 16 variants, so the type is grown after the line that declares it
    std::vector<std::string> const words{
        "apple", "Apple", "APPLE", "Banana", "banana", "b", "Cherry", "cherry", "d", "D", "e", "E",
        "f", "F", "g", "G", "h", "H", "zebra", "Zebra"
    };
    auto word = mm.grab("Word");
    word->sortkey = "fold_case";
    for (auto const & w : words)
        word->add_variant(w);

    auto plain = mm.grab("Plain");
    for (auto const & w : {"apple", "Apple", "Banana", "banana"})
        plain->add_variant(w);

    matchable::save__content::Flags const content{matchable::save__content::matchables::grab()};
    auto const status = mm.save_as(argv[1], content, matchable::save__grow_mode::wrap::grab());
    return status == matchable::save__status::success::grab() ? 0 : 1;
}
