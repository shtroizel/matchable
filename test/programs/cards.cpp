#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, Black, Red)
PROPERTYx1_MATCHABLE(Color::Type, color, Suit, Clubs, Diamonds, Hearts, Spades)

// There are two syntaxes for this. Here we set the black suits at link time using the
// MATCHABLE_VARIANT_PROPERTY_VALUE() macro. To demonstrate the other syntax we use the run-time
// interface within main() to set the red suits.
MATCHABLE_VARIANT_PROPERTY_VALUE(Suit, Clubs, color, Color::Black::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Suit, Spades, color, Color::Black::grab())

MATCHABLE(Rank, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace)



struct Card
{
    Suit::Type suit;
    Rank::Type rank;
    bool operator==(Card const & other) { return suit == other.suit && rank == other.rank; }
};



std::array<Card, 5> deal()
{
    static std::mt19937 engine{std::random_device()()};
    static std::uniform_int_distribution<int> suit_distribution{0, (int) Suit::variants().size() - 1};
    static std::uniform_int_distribution<int> rank_distribution{0, (int) Rank::variants().size() - 1};

    std::array<Card, 5> cards;
    std::vector<Card> already_used;
    for (auto & card : cards)
    {
        do
        {
            card.suit = Suit::from_index(suit_distribution(engine));
            card.rank = Rank::from_index(rank_distribution(engine));
        }
        while (std::find(already_used.begin(), already_used.end(), card) != already_used.end());
        already_used.push_back(card);
    }

    return cards;
}


int main()
{
    test_ok ok;

    // accomplishes the same as MATCHABLE_VARIANT_PROPERTY_VALUE() above, but at run-time...
    // which of course can be done again and again if different values are needed based on
    // some run-time state
    Suit::Diamonds::grab().set_color(Color::Red::grab());
    Suit::Hearts::grab().set_color(Color::Red::grab());

    auto cards = deal();
    int black_count{0};

    for (auto card : cards)
    {
        std::cout << card.rank << " of " << card.suit << std::endl;

        if (card.suit.as_color() == Color::Black::grab())
            ++black_count;

        card.suit.match({
            { Suit::Clubs::grab(),    [&](){ TEST_EQ(ok, card.suit.as_color(), Color::Black::grab()); } },
            { Suit::Diamonds::grab(), [&](){ TEST_EQ(ok, card.suit.as_color(), Color::Red::grab()); } },
            { Suit::Hearts::grab(),   [&](){ TEST_EQ(ok, card.suit.as_color(), Color::Red::grab()); } },
            { Suit::Spades::grab(),   [&](){ TEST_EQ(ok, card.suit.as_color(), Color::Black::grab()); } },
            { Suit::nil,              [&](){ TEST_FAIL(ok); } }
        });
    }

    std::cout << black_count << " card";
    if (black_count == 1)
        std::cout << " is ";
    else
        std::cout << "s are ";
    std::cout << "black." << std::endl;

    return ok();
}
