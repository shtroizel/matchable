#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



int word_count();
int word_index(std::string const & s, bool * found);
int zebra_by_string_index();

// constructed before Word registers its variants, so it is destroyed after them: lookups made while
// static objects are destroyed must still find Word's names, and an index read before registration is -1
struct LookupAtExit
{
    LookupAtExit() : zebra_before_registration{zebra_by_string_index()} { word_count(); }
    ~LookupAtExit()
    {
        bool found = false;
        if (word_index("zebra", &found) != 18 || !found || zebra_by_string_index() != 18)
            std::_Exit(1);
    }
    int const zebra_before_registration;
} lookup_at_exit;



MATCHABLE(
    Word,
    apple,
    Apple,
    apples,
    apple_spc_pie,
    apple_mns_tree,
    banana,
    esc_17,
    esc_2,
    zebra,
    Zebra,
    a_very_long_variant_name_well_past_fifteen_characters,
    a_very_long_variant_name_well_past_fifteen_characterz
)
GROW_MATCHABLE(Word, aardvark, mango, esc_100, b, ba, bab, apple_tld_pie)

MATCHABLE(Empty)


int word_count() { return (int) Word::variants_by_string().size(); }
int word_index(std::string const & s, bool * found) { return Word::variants_by_string_index_of(s, found); }
int zebra_by_string_index() { return Word::zebra::grab().as_by_string_index(); }


// the by-string order, written out here instead of taken from the library: unsigned bytes, then shorter
// first
bool expected_lt(std::string const & l, std::string const & r)
{
    for (size_t i = 0; i < l.size() && i < r.size(); ++i)
        if (l[i] != r[i])
            return (unsigned char) l[i] < (unsigned char) r[i];
    return l.size() < r.size();
}



// variants_by_string_index_of() as it was before lookups searched a table of names, with the test's own
// comparison
template<typename T>
int reference_index_of(std::string const & s, bool * found)
{
    auto const & v = T::variants_by_string();
    auto it = std::lower_bound(v.begin(), v.end(), s, [](T const & a, std::string const & b)
                                                      { return expected_lt(a.as_string(), b); });
    if (it == v.end())
    {
        *found = false;
        return (int) v.size();
    }
    *found = s == it->as_string();
    return it->as_by_string_index();
}


std::vector<std::string> probes()
{
    std::vector<std::string> p{"", "a", "A", "z", "~", " ", "0", "9", std::string(1, '\0'),
                               std::string("apple\0x", 7), std::string(1, (char) 200)};
    for (auto const & w : Word::variants_by_string())
    {
        std::string const s = w.as_string();
        p.push_back(s);
        for (char c : {'\0', ' ', '0', 'A', 'a', 'z', '~', (char) 200})
            p.push_back(s + c);
        p.push_back(s.substr(0, s.size() - 1));
        p.push_back(s.substr(1));
    }
    std::mt19937 rng{20261117};
    std::string const chars = "aAbBpPzZ0179 -_~.";
    for (int i = 0; i < 5000; ++i)
    {
        std::string s;
        for (int n = (int) (rng() % 8); n > 0; --n)
            s += chars[rng() % chars.size()];
        p.push_back(s);
    }
    return p;
}


void check_word(test_ok & ok, std::vector<std::string> const & probes, std::string const & where)
{
    for (auto const & s : probes)
    {
        bool expected_found = true;
        int const expected = reference_index_of<Word::Type>(s, &expected_found);
        Word::Type const & expected_variant = expected_found ? Word::from_by_string_index_as_ref(expected)
                                                             : Word::nil;

        bool found = !expected_found;
        TEST_EQ(ok, Word::variants_by_string_index_of(s, &found), expected);
        TEST_EQ(ok, found, expected_found);
        TEST_EQ(ok, Word::variants_by_string_index_of(s, nullptr), expected);
        TEST_EQ(ok, Word::from_string_as_ref(s), expected_variant);
        TEST_EQ(ok, Word::from_string(s), expected_variant);

        if (s.find('\0') == std::string::npos)
        {
            found = !expected_found;
            TEST_EQ(ok, Word::variants_by_string_index_of(s.c_str(), &found), expected);
            TEST_EQ(ok, found, expected_found);
            TEST_EQ(ok, Word::from_string_as_ref(s.c_str()), expected_variant);
            TEST_EQ(ok, Word::from_string(s.c_str()), expected_variant);
        }

        found = !expected_found;
        TEST_EQ(ok, Word::variants_by_string_index_of(s.data(), s.size(), &found), expected);
        TEST_EQ(ok, found, expected_found);
        TEST_EQ(ok, Word::from_string_as_ref(s.data(), s.size()), expected_variant);
    }
    if (!ok.ok)
        std::cout << "failed while checking " << where << std::endl;
}


int main()
{
    test_ok ok;
    auto const p = probes();

    std::vector<std::string> const expected_order{
        "100", "17", "2", "Apple", "Zebra", "a_very_long_variant_name_well_past_fifteen_characters",
        "a_very_long_variant_name_well_past_fifteen_characterz", "aardvark", "apple", "apple pie",
        "apple-tree", "apples", "apple~pie", "b", "ba", "bab", "banana", "mango", "zebra"
    };
    std::vector<std::string> order;
    for (auto const & w : Word::variants_by_string())
        order.push_back(w.as_string());
    TEST_EQ(ok, order == expected_order, true);
    TEST_EQ(ok, lookup_at_exit.zebra_before_registration, -1);
    for (int i = 0; i < (int) Word::variants_by_string().size(); ++i)
        TEST_EQ(ok, Word::variants_by_string()[i].as_by_string_index(), i);

    // as_index() is the registration order, in every build: each index from 0 to size - 1 once
    std::vector<int> indexes;
    for (auto const & w : Word::variants())
        indexes.push_back(w.as_index());
    std::sort(indexes.begin(), indexes.end());
    for (int i = 0; i < (int) indexes.size(); ++i)
        TEST_EQ(ok, indexes[i], i);
    TEST_EQ(ok, Word::nil.as_index(), -1);

    for (auto const & w : Word::variants_by_string())
    {
        bool found = false;
        TEST_EQ(ok, Word::variants_by_string_index_of(w.as_string(), &found), w.as_by_string_index());
        TEST_EQ(ok, found, true);
        TEST_EQ(ok, Word::from_string(w.as_string()), w);
    }
    check_word(ok, p, "all variants");

    {
        char const * null_name = nullptr;
        bool found = true;
        TEST_EQ(ok, Word::variants_by_string_index_of(null_name, &found), 0);
        TEST_EQ(ok, found, false);
        TEST_EQ(ok, Word::from_string_as_ref(null_name), Word::nil);
        TEST_EQ(ok, Word::from_string(nullptr), Word::nil);
    }

    for (auto const & s : p)
    {
        bool found = true;
        TEST_EQ(ok, Empty::variants_by_string_index_of(s, &found), 0);
        TEST_EQ(ok, found, false);
        TEST_EQ(ok, Empty::from_string(s), Empty::nil);
    }

    return ok();
}
