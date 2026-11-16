#include <cstdlib>
#include <iostream>
#include <new>
#include <string>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



static int allocations{0};

void * operator new(std::size_t size)
{
    ++allocations;
    void * p = std::malloc(size == 0 ? 1 : size);
    if (nullptr == p)
        throw std::bad_alloc();
    return p;
}

void operator delete(void * p) noexcept { std::free(p); }
void operator delete(void * p, std::size_t) noexcept { std::free(p); }



PROPERTYx2_MATCHABLE(int, score, int, tags, Fruit, cherry, apple, banana)
MATCHABLE_VARIANT_PROPERTY_VALUE(Fruit, apple, score, 1)
MATCHABLE_VARIANT_PROPERTY_VALUE(Fruit, banana, score, 2)
MATCHABLE_VARIANT_PROPERTY_VALUE(Fruit, cherry, score, 3)
MATCHABLE_VARIANT_PROPERTY_VALUES(Fruit, banana, tags, 4, 5)



int main()
{
    test_ok ok;

    {
        Fruit::Type const & apple = Fruit::from_by_string_index_as_ref(0);
        Fruit::Type const & banana = Fruit::from_by_string_index_as_ref(1);
        Fruit::Type const & cherry = Fruit::from_by_string_index_as_ref(2);
        TEST_EQ(ok, apple, Fruit::apple::grab());
        TEST_EQ(ok, banana, Fruit::banana::grab());
        TEST_EQ(ok, cherry, Fruit::cherry::grab());
        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(-1), Fruit::nil);
        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(3), Fruit::nil);
        TEST_EQ(ok, Fruit::from_string_as_ref("banana"), Fruit::banana::grab());
        TEST_EQ(ok, Fruit::from_string_as_ref("kiwi"), Fruit::nil);

        std::string const cherry_str{"cherry"};
        int const before{allocations};
        int sum{0};
        for (int i = 0; i < 3; ++i)
        {
            Fruit::Type const & f = Fruit::from_by_string_index_as_ref(i);
            sum += f.as_score() + (int) f.as_tags_vect().size();
        }
        sum += Fruit::from_string_as_ref(cherry_str).as_score();
        sum += Fruit::nil.as_score();
        int const allocated{allocations - before};
        TEST_EQ(ok, allocated, 0);
        TEST_EQ(ok, sum, 1 + 2 + 2 + 3 + 3);
    }

    {
        UNMATCHABLE(Fruit, banana);

        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(0), Fruit::apple::grab());
        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(1), Fruit::nil);
        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(2), Fruit::cherry::grab());
        TEST_EQ(ok, Fruit::from_string_as_ref("banana"), Fruit::nil);
        for (int i = -1; i < 4; ++i)
            TEST_EQ(ok, Fruit::from_by_string_index_as_ref(i), Fruit::from_by_string_index(i));
    }

    for (int i = -1; i < 4; ++i)
        TEST_EQ(ok, Fruit::from_by_string_index_as_ref(i), Fruit::from_by_string_index(i));

    return ok();
}
