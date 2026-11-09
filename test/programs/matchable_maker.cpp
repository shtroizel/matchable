#include <filesystem>
#include <iostream>
#include <random>
#include <string>

#include "matchable/MatchableMaker.h"
#include "test_ok.h"



int main()
{
    test_ok ok;

    std::string const unique = "matchable_maker_" + std::to_string(std::random_device{}());
    std::filesystem::path const dir = std::filesystem::temp_directory_path() / unique;
    std::filesystem::create_directories(dir);

    {
        matchable::MatchableMaker mm;
        mm.grab("A")->add_variant("x");
        std::string const path = (dir / "no_content.h").string();
        auto const wrap = matchable::save__grow_mode::wrap::grab();

        matchable::save__content::Flags none;
        TEST_EQ(ok, mm.save_as(path, none, wrap), matchable::save__status::no_content::grab());

        matchable::save__content::Flags only_nil;
        only_nil.set(matchable::save__content::nil);
        TEST_EQ(ok, mm.save_as(path, only_nil, wrap), matchable::save__status::no_content::grab());
        TEST_EQ(ok, std::filesystem::exists(path), false);
    }

    std::filesystem::remove_all(dir);
    return ok();
}
