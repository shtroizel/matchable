#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "matchable/MatchableMaker.h"
#include "test_ok.h"



static std::string slurp(std::filesystem::path const & path)
{
    std::ifstream in(path);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}


static bool contains(std::string const & haystack, std::string const & needle)
{
    return haystack.find(needle) != std::string::npos;
}



int main()
{
    test_ok ok;

    std::string const unique = "matchable_maker_" + std::to_string(std::random_device{}());
    std::filesystem::path const dir = std::filesystem::temp_directory_path() / unique;
    std::filesystem::create_directories(dir);

    auto const wrap = matchable::save__grow_mode::wrap::grab();
    auto const always = matchable::save__grow_mode::always::grab();
    matchable::save__content::Flags const both{
        matchable::save__content::generated_matchable::grab(),
        matchable::save__content::matchables::grab()
    };

    {
        matchable::MatchableMaker mm;
        mm.grab("A")->add_variant("x");
        std::string const path = (dir / "no_content.h").string();

        matchable::save__content::Flags none;
        TEST_EQ(ok, mm.save_as(path, none, wrap), matchable::save__status::no_content::grab());

        matchable::save__content::Flags only_nil;
        only_nil.set(matchable::save__content::nil);
        TEST_EQ(ok, mm.save_as(path, only_nil, wrap), matchable::save__status::no_content::grab());
        TEST_EQ(ok, std::filesystem::exists(path), false);
    }

    {
        matchable::MatchableMaker mm;
        mm.grab("A")->add_variant("x");
        std::filesystem::path const path = dir / "grow_mode.h";

        for (auto const & mode : matchable::save__grow_mode::variants())
        {
            TEST_EQ(ok, mm.save_as(path.string(), both, mode), matchable::save__status::success::grab());
            TEST_EQ(ok, contains(slurp(path), "MATCHABLE(A, x)"), true);
            TEST_EQ(ok, contains(slurp(path), "MATCHABLE(generated_matchable, A)"), true);
        }
        TEST_EQ(ok, mm.save_as(path.string(), both, matchable::save__grow_mode::nil),
                matchable::save__status::no_grow_mode::grab());

        mm.save_as(path.string(), both, wrap);
        TEST_EQ(ok, contains(slurp(path), "\nMATCHABLE(A, x)"), true);
        mm.save_as(path.string(), both, always);
        TEST_EQ(ok, contains(slurp(path), "\nGROW_MATCHABLE(A, x)"), true);
        TEST_EQ(ok, contains(slurp(path), "\nGROW_MATCHABLE(generated_matchable, A)"), true);
    }

    {
        matchable::MatchableMaker mm;
        mm.grab("Color")->add_variant("red");
        mm.grab("Car")->add_property("Color::Type", "paint");
        mm.grab("Car")->add_variant("sedan");
        mm.grab("Car")->set_property("sedan", "paint", "Color::red::grab()");
        std::filesystem::path const path = dir / "dependency.h";
        TEST_EQ(ok, mm.save_as(path.string(), {matchable::save__content::matchables::grab()}, wrap),
                matchable::save__status::success::grab());
        std::string const saved = slurp(path);
        size_t const color_at = saved.find("\nMATCHABLE(Color, red)");
        size_t const car_at = saved.find("\nPROPERTYx1_MATCHABLE(Color::Type, paint, Car, sedan)");
        TEST_EQ(ok, color_at != std::string::npos, true);
        TEST_EQ(ok, car_at != std::string::npos, true);
        TEST_EQ(ok, color_at < car_at, true);
        std::string const paint_value{
            "MATCHABLE_VARIANT_PROPERTY_VALUE(Car, sedan, paint, Color::red::grab())"
        };
        TEST_EQ(ok, contains(saved, paint_value), true);
        TEST_EQ(ok, (int) mm.matchables.size(), 2);
    }

    {
        matchable::MatchableMaker mm;
        mm.grab("Egg")->add_property("Hen::Type", "layer");
        mm.grab("Hen")->add_property("Egg::Type", "origin");
        std::filesystem::path const path = dir / "cycle.h";
        TEST_EQ(ok, mm.save_as(path.string(), {matchable::save__content::matchables::grab()}, wrap),
                matchable::save__status::cyclic_dependencies::grab());
        TEST_EQ(ok, (int) mm.matchables.size(), 2);
    }

    {
        matchable::MatchableMaker mm;
        for (int i = 0; i < 18; ++i)
            mm.grab((i < 10 ? "m0" : "m") + std::to_string(i));
        std::filesystem::path const path = dir / "generated.h";
        matchable::save__content::Flags const generated{
            matchable::save__content::generated_matchable::grab()
        };
        for (auto const & mode : matchable::save__grow_mode::variants())
        {
            mm.save_as(path.string(), generated, mode);
            std::string const saved = slurp(path);
            TEST_EQ(ok, contains(saved, "(generated_matchable, m00,"), true);
            TEST_EQ(ok, contains(saved, ", m15)\nGROW_MATCHABLE(generated_matchable, m16, m17)\n"), true);
            TEST_EQ(ok, contains(saved, "GROW_MATCHABLE(m16"), false);
        }
    }

    {
        // a sort key goes once, before the line declaring its type, and extra includes follow the
        // standard ones; both survive a load, so saving what was loaded writes the same file
        matchable::MatchableMaker mm;
        mm.includes.push_back("matchable_maker_key.h");
        mm.includes.push_back("other/key.h");
        auto word = mm.grab("Word");
        word->sortkey = "fold_case";
        for (int i = 0; i < 20; ++i)
            word->add_variant("w" + std::to_string(i));
        auto shape = mm.grab("Shape");
        shape->sortkey = "keys::shape_key";
        shape->add_property("int", "sides");
        shape->add_variant("square");
        mm.grab("Plain")->add_variant("p");
        std::filesystem::path const path = dir / "sortkey.h";
        matchable::save__content::Flags const content{matchable::save__content::matchables::grab()};
        TEST_EQ(ok, mm.save_as(path.string(), content, wrap), matchable::save__status::success::grab());
        std::string const saved = slurp(path);
        TEST_EQ(ok, contains(saved, "#include <matchable/matchable_fwd.h>\n"
                                    "#include <matchable_maker_key.h>\n"
                                    "#include <other/key.h>\n"), true);
        TEST_EQ(ok, contains(saved, "\nMATCHABLE_SORTKEY(Word, fold_case)\nMATCHABLE(Word, w0, "), true);
        TEST_EQ(ok, contains(saved, "\nMATCHABLE_SORTKEY(Shape, keys::shape_key)\n"
                                    "PROPERTYx1_MATCHABLE(int, sides, Shape, square)\n"), true);
        TEST_EQ(ok, contains(saved, "\nGROW_MATCHABLE(Word, w16, "), true);
        TEST_EQ(ok, saved.find("MATCHABLE_SORTKEY(Word") == saved.rfind("MATCHABLE_SORTKEY(Word"), true);
        TEST_EQ(ok, saved.find("MATCHABLE_FWD(Word)") < saved.find("MATCHABLE_SORTKEY(Word"), true);
        TEST_EQ(ok, contains(saved, "MATCHABLE_SORTKEY(Plain"), false);

        matchable::MatchableMaker loaded;
        TEST_EQ(ok, loaded.load(path.string()), matchable::load__status::success::grab());
        TEST_EQ(ok, loaded.includes == mm.includes, true);
        TEST_EQ(ok, loaded.grab("Word")->sortkey, std::string{"fold_case"});
        TEST_EQ(ok, loaded.grab("Shape")->sortkey, std::string{"keys::shape_key"});
        TEST_EQ(ok, loaded.grab("Plain")->sortkey, std::string{});
        std::filesystem::path const again = dir / "sortkey_again.h";
        TEST_EQ(ok, loaded.save_as(again.string(), content, wrap),
                matchable::save__status::success::grab());
        TEST_EQ(ok, slurp(again), saved);

        // always mode only grows types declared elsewhere, which carry their own sort key
        std::filesystem::path const grown = dir / "sortkey_grown.h";
        TEST_EQ(ok, mm.save_as(grown.string(), content, always), matchable::save__status::success::grab());
        std::string const grown_text = slurp(grown);
        TEST_EQ(ok, contains(grown_text, "#include <matchable_maker_key.h>\n"), true);
        TEST_EQ(ok, contains(grown_text, "\nGROW_MATCHABLE(Word, w0, "), true);
        TEST_EQ(ok, contains(grown_text, "MATCHABLE_SORTKEY"), false);
        TEST_EQ(ok, contains(grown_text, "MATCHABLE_FWD"), false);
    }

    {
        std::vector<std::string> numbers;
        for (int i = 0; i < 20; ++i)
            numbers.push_back(std::to_string(i));
        std::vector<std::string> const words{"\"one, two\"", "\"(three)\""};

        matchable::MatchableMaker mm;
        auto m = mm.grab("Shape");
        m->add_property("unsigned int", "sides");
        m->add_property("std::string", "label");
        m->add_property("int", "counts");
        m->add_property("std::string", "names");
        m->add_property("int", "limit");
        m->add_variant("square");
        m->set_property("square", "sides", "4");
        m->set_property("square", "limit", "std::max(1, 2)");
        m->set_property("square", "label", "\"a big, (bold) square\"");
        m->set_propertyvect("square", "counts", numbers);
        m->set_propertyvect("square", "names", words);
        std::filesystem::path const path = dir / "round_trip.h";
        TEST_EQ(ok, mm.save_as(path.string(), {matchable::save__content::matchables::grab()}, wrap),
                matchable::save__status::success::grab());

        matchable::MatchableMaker loaded;
        TEST_EQ(ok, loaded.load(path.string()), matchable::load__status::success::grab());
        auto l = loaded.grab("Shape");
        TEST_EQ(ok, l->property_types_and_names == m->property_types_and_names, true);
        TEST_EQ(ok, l->has_variant("square"), true);
        TEST_EQ(ok, (int) l->variants.size(), 1);

        std::string value;
        l->get_property("square", "sides", value);
        TEST_EQ(ok, value, std::string{"4"});
        l->get_property("square", "label", value);
        TEST_EQ(ok, value, std::string{"\"a big, (bold) square\""});
        l->get_property("square", "limit", value);
        TEST_EQ(ok, value, std::string{"std::max(1, 2)"});

        std::vector<std::string> values;
        l->get_propertyvect("square", "counts", values);
        TEST_EQ(ok, values == numbers, true);
        l->get_propertyvect("square", "names", values);
        TEST_EQ(ok, values == words, true);
    }

    {
        // variants that other files declare are declared extern before the matchables, 16 to a line, so
        // property values can refer to them; they survive a load
        matchable::MatchableMaker mm;
        for (int i = 0; i < 18; ++i)
            mm.extern_variants["Planet"].insert("p" + std::to_string(10 + i));
        mm.extern_variants["Moon"].insert("luna");
        auto alien = mm.grab("Alien");
        alien->add_property("::matchable::VariantRef", "home");
        alien->add_property("::matchable::VariantRef", "visited");
        alien->add_variant("zog");
        alien->set_property("zog", "home", "::matchable::variant_ref<Planet::p10>()");
        std::vector<std::string> const visited{
            "::matchable::variant_ref<Moon::luna>()", "::matchable::variant_ref<Planet::p27>()"
        };
        alien->set_propertyvect("zog", "visited", visited);
        std::filesystem::path const path = dir / "extern_variants.h";
        matchable::save__content::Flags const content{matchable::save__content::matchables::grab()};
        TEST_EQ(ok, mm.save_as(path.string(), content, wrap), matchable::save__status::success::grab());
        std::string const saved = slurp(path);
        TEST_EQ(ok, contains(saved, "\nMATCHABLE_EXTERN_VARIANTS(Moon, luna)\n"
                                    "MATCHABLE_EXTERN_VARIANTS(Planet, p10, p11, "), true);
        TEST_EQ(ok, contains(saved, ", p25)\nMATCHABLE_EXTERN_VARIANTS(Planet, p26, p27)\n"), true);
        TEST_EQ(ok, saved.find("(Planet, p26") < saved.find("MATCHABLE_FWD(Alien)"), true);

        matchable::MatchableMaker loaded;
        TEST_EQ(ok, loaded.load(path.string()), matchable::load__status::success::grab());
        TEST_EQ(ok, loaded.extern_variants == mm.extern_variants, true);
        TEST_EQ(ok, (int) loaded.matchables.size(), 1);
        std::vector<std::string> values;
        loaded.grab("Alien")->get_propertyvect("zog", "visited", values);
        TEST_EQ(ok, values == visited, true);
        std::filesystem::path const again = dir / "extern_variants_again.h";
        TEST_EQ(ok, loaded.save_as(again.string(), content, wrap),
                matchable::save__status::success::grab());
        TEST_EQ(ok, slurp(again), saved);

        // a file that only grows refers to them the same way
        std::filesystem::path const grown = dir / "extern_variants_grown.h";
        TEST_EQ(ok, mm.save_as(grown.string(), content, always), matchable::save__status::success::grab());
        TEST_EQ(ok, contains(slurp(grown), "\nMATCHABLE_EXTERN_VARIANTS(Moon, luna)\n"), true);
    }

    if (std::filesystem::exists("/dev/full") && std::filesystem::exists("/proc/self/fd"))
    {
        auto const open_files = []() {
            return std::distance(std::filesystem::directory_iterator("/proc/self/fd"),
                                 std::filesystem::directory_iterator{});
        };
        auto const before = open_files();

        matchable::MatchableMaker small;
        small.grab("A")->add_variant("x");
        TEST_EQ(ok, small.save_as("/dev/full", {matchable::save__content::matchables::grab()}, wrap),
                matchable::save__status::io_error::grab());

        matchable::MatchableMaker big;
        auto m = big.grab("A");
        m->add_property("int", "n");
        m->add_variant("x");
        m->set_propertyvect("x", "n", std::vector<std::string>(4000, "1"));
        TEST_EQ(ok, big.save_as("/dev/full", {matchable::save__content::matchables::grab()}, wrap),
                matchable::save__status::io_error::grab());

        TEST_EQ(ok, open_files(), before);
    }

    std::filesystem::remove_all(dir);
    return ok();
}
