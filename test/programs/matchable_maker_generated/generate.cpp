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

    // refers to variants of a matchable that the saved header leaves to other code
    mm.extern_variants["Planet"] = {"mars", "venus"};
    auto alien = mm.grab("Alien");
    alien->add_property("::matchable::VariantRef", "home");
    alien->add_property("::matchable::VariantRef", "visited");
    alien->add_variant("zog");
    alien->set_property("zog", "home", "::matchable::variant_ref<Planet::mars>()");
    alien->set_propertyvect("zog", "visited", {"::matchable::variant_ref<Planet::venus>()",
                                               "::matchable::variant_ref<Planet::mars>()"});

    matchable::save__content::Flags const content{matchable::save__content::matchables::grab()};
    auto const status = mm.save_as(argv[1], content, matchable::save__grow_mode::wrap::grab());
    return status == matchable::save__status::success::grab() ? 0 : 1;
}
