// built with hidden visibility and MATCHABLE_INSTANCE_VISIBILITY=default, which exports the instances of
// the variants it declares and grows, the way a library of leaves would
#include "fruit.h"



// only the library has banana, so its registry differs from the program's
GROW_MATCHABLE(Fruit, banana)

__attribute__((visibility("default"))) ::matchable::VariantRef apple_in_library()
{
    return ::matchable::variant_ref<Fruit::apple>();
}
