// refers to variants a shared library owns by name, knowing only their declarations: the linker resolves
// the references to that library's objects, which answer from its registry, which also holds banana
#include <string>

#include "refs.h"
#include "test_ok.h"



MATCHABLE_EXTERN_VARIANTS(Fruit, pear, apple, banana)

static ::matchable::VariantRef const refs[] = {
    ::matchable::variant_ref<Fruit::apple>(),
    ::matchable::variant_ref<Fruit::pear>(),
    ::matchable::variant_ref<Fruit::banana>(),
};



int main()
{
    test_ok ok;

    TEST_EQ(ok, refs[0] == apple_in_library(), true);
    TEST_EQ(ok, refs[0]->as_string(), std::string("apple"));
    TEST_EQ(ok, refs[0]->as_by_string_index(), 0);
    TEST_EQ(ok, refs[1]->as_string(), std::string("pear"));
    TEST_EQ(ok, refs[1]->as_by_string_index(), 2);
    TEST_EQ(ok, refs[2]->as_string(), std::string("banana"));
    TEST_EQ(ok, refs[2]->as_by_string_index(), 1);

    return ok();
}
