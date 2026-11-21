#include <iostream>
#include <type_traits>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green)
PROPERTYx2_MATCHABLE(
    ::matchable::VariantRef, favorite,
    ::matchable::VariantRef, opposite,
    Fruit, pear, apple
)

MATCHABLE_VARIANT_PROPERTY_VALUE(Fruit, apple, favorite, ::matchable::variant_ref<Color::red>())
MATCHABLE_VARIANT_PROPERTY_VALUES(Fruit, apple, opposite, ::matchable::variant_ref<Fruit::pear>(),
                                  ::matchable::variant_ref<Color::green>())

// references are constants, so tables of them need no code to fill
static ::matchable::VariantRef const table[] = {
    ::matchable::variant_ref<Fruit::pear>(),
    ::matchable::variant_ref<Color::green>(),
};



int main()
{
    test_ok ok;

    static_assert(std::is_base_of<::matchable::Variant, Fruit::IFruit>::value, "Fruit is a Variant");
    static_assert(std::is_base_of<::matchable::Variant, Color::IColor>::value, "Color is a Variant");
    static_assert(!std::is_assignable<Fruit::Type &, Color::Type>::value, "typed matchables don't mix");

    {
        ::matchable::VariantRef r = ::matchable::variant_ref<Fruit::apple>();
        TEST_EQ(ok, r.is_nil(), false);
        TEST_EQ(ok, r->as_string(), std::string("apple"));
        TEST_EQ(ok, r->as_type_name(), std::string("Fruit"));
        TEST_EQ(ok, r->as_by_string_index(), Fruit::apple::grab().as_by_string_index());
        TEST_EQ(ok, r->as_index(), Fruit::apple::grab().as_index());
        TEST_EQ(ok, r->as_by_string_index(), 0);
    }

    {
        ::matchable::VariantRef r = ::matchable::variant_ref<Color::green>();
        TEST_EQ(ok, r->as_string(), std::string("green"));
        TEST_EQ(ok, r->as_type_name(), std::string("Color"));
        TEST_EQ(ok, r->as_by_string_index(), 0);
    }

    // back to a typed variant, or nil when the reference is to another matchable or nil
    TEST_EQ(ok, Fruit::from_variant(::matchable::variant_ref<Fruit::pear>()), Fruit::pear::grab());
    TEST_EQ(ok, Fruit::from_variant(::matchable::variant_ref<Color::red>()), Fruit::nil);
    TEST_EQ(ok, Fruit::from_variant(nullptr), Fruit::nil);
    TEST_EQ(ok, Color::from_variant(table[1]), Color::green::grab());

    // as property values
    TEST_EQ(ok, Fruit::apple::grab().as_favorite()->as_string(), std::string("red"));
    TEST_EQ(ok, Fruit::pear::grab().as_favorite().is_nil(), true);
    TEST_EQ(ok, Fruit::pear::grab().as_favorite() == nullptr, true);
    {
        std::vector<::matchable::VariantRef> const & o = Fruit::apple::grab().as_opposite_vect();
        TEST_EQ(ok, (int) o.size(), 2);
        if (o.size() == 2)
        {
            TEST_EQ(ok, o[0] == table[0], true);
            TEST_EQ(ok, Color::from_variant(o[1]), Color::green::grab());
        }
    }
    Fruit::pear::grab().set_favorite(::matchable::variant_ref<Color::green>());
    TEST_EQ(ok, Fruit::pear::grab().as_favorite()->as_string(), std::string("green"));

    return ok();
}
