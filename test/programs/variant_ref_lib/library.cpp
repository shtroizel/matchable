// built with hidden visibility, exporting only the variants it owns, the way a library of leaves would
#include "fruit.h"



// only the library has banana, so its registry differs from the program's
GROW_MATCHABLE(Fruit, banana)

template struct __attribute__((visibility("default"))) ::matchable::Instance<Fruit::pear>;
template struct __attribute__((visibility("default"))) ::matchable::Instance<Fruit::apple>;

__attribute__((visibility("default"))) ::matchable::VariantRef apple_in_library()
{
    return ::matchable::variant_ref<Fruit::apple>();
}
