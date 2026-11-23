#include <algorithm>
#include <string>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



int fold_calls{0};


std::string fold(std::string const & name)
{
    ++fold_calls;
    std::string folded{name};
    for (char & c : folded)
        if (c >= 'A' && c <= 'Z')
            c = (char) (c - 'A' + 'a');
    return folded;
}


MATCHABLE_SORTKEY(Word, fold)
MATCHABLE(Word, apple, Apple, Banana, banana, APPLE, b, esc_1, apple_spc_pie)
GROW_MATCHABLE(Word, aPPle, BANANA, zebra)

MATCHABLE(Plain, apple, Apple, Banana, banana)



// the order written out here: the folded name as unsigned bytes, then the name
bool expected_lt(std::string const & l, std::string const & r)
{
    std::string const fl = fold(l);
    std::string const fr = fold(r);
    return fl != fr ? fl < fr : l < r;
}


std::vector<std::string> names_by_string()
{
    std::vector<std::string> names;
    for (auto const & w : Word::variants_by_string())
        names.push_back(w.as_string());
    return names;
}



int main()
{
    test_ok ok;

    std::vector<std::string> const expected{
        "1", "APPLE", "Apple", "aPPle", "apple", "apple pie", "b", "BANANA", "Banana", "banana", "zebra"
    };
    TEST_EQ(ok, names_by_string() == expected, true);
    for (int i = 0; i < (int) Word::variants_by_string().size(); ++i)
    {
        Word::Type const & w = Word::variants_by_string()[i];
        TEST_EQ(ok, w.as_by_string_index(), i);
        TEST_EQ(ok, Word::from_string(w.as_string()), w);
        bool found = false;
        TEST_EQ(ok, Word::variants_by_string_index_of(w.as_string(), &found), i);
        TEST_EQ(ok, found, true);
        if (i > 0)
        {
            TEST_EQ(ok, Word::variants_by_string()[i - 1].lt_by_string(w), true);
            TEST_EQ(ok, w.lt_by_string(Word::variants_by_string()[i - 1]), false);
            TEST_EQ(ok, Word::variants_by_string()[i - 1].lt_by_string(w.as_string()), true);
        }
    }

    // a name that isn't a variant: not found, and its index is where it would go
    for (std::string const s : {"", "0", "APPLe", "Apple pie", "ba", "BANANAS", "c", "zz", "~", "\xc3\xa9"})
    {
        bool found = true;
        int const index = Word::variants_by_string_index_of(s, &found);
        TEST_EQ(ok, found, false);
        TEST_EQ(ok, Word::from_string(s), Word::nil);
        auto const at = std::lower_bound(expected.begin(), expected.end(), s, expected_lt);
        TEST_EQ(ok, index, (int) (at - expected.begin()));
    }

    // two variants compare by their stored order, and a variant against a string computes one key
    {
        Word::Type const apple = Word::apple::grab();
        Word::Type const banana = Word::Banana::grab();
        int before = fold_calls;
        TEST_EQ(ok, apple.lt_by_string(banana), true);
        TEST_EQ(ok, banana.lt_by_string(apple), false);
        TEST_EQ(ok, apple.lt_by_string(apple), false);
        TEST_EQ(ok, fold_calls - before, 0);
        before = fold_calls;
        TEST_EQ(ok, apple.lt_by_string(std::string{"Apple pie"}), true);
        TEST_EQ(ok, apple.lt_by_string(std::string{"APPLE"}), false);
        TEST_EQ(ok, apple.lt_by_string(std::string{"apple"}), false);
        TEST_EQ(ok, apple.lt_by_string(std::string{"applf"}), true);
        TEST_EQ(ok, banana.lt_by_string(std::string{"banana"}), true);
        TEST_EQ(ok, banana.lt_by_string(std::string{"b"}), false);
        TEST_EQ(ok, fold_calls - before, 6);

        // nil compares as the name "nil"
        Word::Type const nil;
        TEST_EQ(ok, nil.lt_by_string(apple), false);
        TEST_EQ(ok, apple.lt_by_string(nil), true);
        TEST_EQ(ok, nil.lt_by_string(std::string{"zebra"}), true);

        TEST_EQ(ok, Plain::apple::grab().lt_by_string(Plain::Banana::grab()), false);
        TEST_EQ(ok, Plain::Banana::grab().lt_by_string(Plain::apple::grab()), true);
        TEST_EQ(ok, Plain::apple::grab().lt_by_string(std::string{"b"}), true);
        TEST_EQ(ok, Plain::apple::grab().lt_by_string(std::string{"Zebra"}), false);
        TEST_EQ(ok, Plain::Type{}.lt_by_string(Plain::apple::grab()), false);
    }

    // a type without a sort key keeps plain byte order
    std::vector<std::string> plain;
    for (auto const & p : Plain::variants_by_string())
        plain.push_back(p.as_string());
    TEST_EQ(ok, plain == (std::vector<std::string>{"Apple", "Banana", "apple", "banana"}), true);

    // removing variants removes their keys too, so the rest keep their order and lookups
    Word::IWord::unregister_variants({Word::aPPle::grab().as_index(), Word::BANANA::grab().as_index()});
    std::vector<std::string> const kept{
        "1", "APPLE", "Apple", "apple", "apple pie", "b", "Banana", "banana", "zebra"
    };
    TEST_EQ(ok, names_by_string() == kept, true);
    for (int i = 0; i < (int) kept.size(); ++i)
    {
        bool found = false;
        TEST_EQ(ok, Word::variants_by_string_index_of(kept[i], &found), i);
        TEST_EQ(ok, found, true);
        TEST_EQ(ok, Word::from_string(kept[i]).as_by_string_index(), i);
    }
    bool found = true;
    TEST_EQ(ok, Word::variants_by_string_index_of("aPPle", &found), 3);
    TEST_EQ(ok, found, false);

    return ok();
}
