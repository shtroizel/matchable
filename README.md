# MATCHABLE

seemlessly flow<br/>
between strings and identifiers<br/>
itemized in groups<br/>
that can spread forming relationships<br/>
as a compile-time tool<br/>
for inconspicuous efficiency<br/>

# Getting Started
MATCHABLE is implemented by:<br/>
* src/matchable.h<br/>
* src/matchable_fwd.h
# Complete Example
The following program can be found at: test/programs/matchable_usage.cpp
```cpp
#include <random>
#include <iostream>

#include "matchable.h"



MATCHABLE(status, success, invalid_input, nothing_to_do, insufficient_memory, timed_out);


status::Type foo()
{
    static int magic;
    magic++;
    if (magic % 107 == 0) // get variant by grabbing (constant complexity)
        return status::success::grab();

    if (magic % 42 == 0) // get variant from string (log2(n) complexity)
        return status::from_string("invalid_input");

    static std::mt19937 engine{std::random_device()()};

    // get index from variant
    // get std::vector of all variants
    static std::uniform_int_distribution<int> status_distribution{
        status::nothing_to_do::grab().as_index(),
        static_cast<int>(status::variants().size()) - 1
    };

    // get variant from index (constant complexity)
    status::Type ret{status::from_index(status_distribution(engine))};
    assert(!ret.is_nil()); // index out of range would mean bug creating status_distribution

    return ret;
}


int main()
{
    status::Type foo_status;
    assert(foo_status.is_nil());

    while (true)
    {
        std::cout << "." << std::flush;

        foo_status = foo();
        MATCH_WITH_FLOW_CONTROL foo_status.match({
            {status::nil,
                [](auto &){
                    std::cout << "nil!" << std::endl;
                }},
            {status::success::grab(),
                [](matchable::FlowControl & fc){ // explicit type instead of auto for FlowControl
                    std::cout << "107" << std::endl;
                    fc.brk();
                }},
            {status::nothing_to_do::grab(),       [](auto & fc){ fc.cont(); }},
            {status::invalid_input::grab(),       [](auto & fc){ fc.cont(); }},
            {status::insufficient_memory::grab(), [](auto & fc){ fc.cont(); }},
            {status::timed_out::grab(),           [](auto & fc){ fc.cont(); }},
        }); EVAL_FLOW_CONTROL

        // only happens if foo_status == status::nil, which is never
        std::cout << "match: " << status::nil.as_string() << "!" << std::endl;
    }

    // operator<<() is provided, so as_string() can be omitted...
    std::cout << foo_status << std::endl;

    return foo_status.as_index();
}

```
# Macro-API<a name="macro_api"></a>
## Macros for creating new types
#### MATCHABLE(type, variant...)
Params:
* **type** The new "matchable" to be created<br/>
* **variant...** 0..108 comma separated variants of the new matchable type

#### SPREADx1_MATCHABLE(spread, type, variant...)
Params:
* **spread** Existing matchable type (must be at least forward declared)<br/>
* **type** The new "matchable" to be created<br/>
* **variant...** 0..108 comma separated variants of the new matchable type<br/>
Same as MATCHABLE(), but with the added ability to have a setter and accessor created for some matchable
type "spread".<br/>

Injects:
```
void set_`spread`(`spread`::Type)
`spread`::Type as_`spread`() const
```
as member functions of the new type.<br/>
Example: test/programs/spread_matchable.cpp

#### SPREADx2_MATCHABLE(spread0, spread1, type, variant...)
Similar to SPREADx1_MATCHABLE(), but with ability to specify 2 spread types<br/>
Example: test/programs/spread2x_matchable.cpp

#### SPREADx3_MATCHABLE(spread0, spread1, spread2, type, variant...)
Similar to SPREADx1_MATCHABLE(), but with ability to specify 3 spread types<br/>
Example: test/programs/relationships.cpp

#### SPREADx4_MATCHABLE(spread0, spread1, spread2, spread3, type, variant...)
Similar to SPREADx1_MATCHABLE(), but with ability to specify 4 spread types<br/>
Example: test/programs/relationships.cpp

#### SPREADVECTOF_MATCHABLE(spread, type, variant...)
Similar to SPREADx1_MATCHABLE(), but the injected functions work on vectors of the given **spread**.

Injects:
```
void set_`spread`_vect(std::vector<`spread`::Type>)
std::vector<`spread`::Type> as_`spread`_vect() const
```
as member functions of the new type.<br/>
Example: test/programs/spreadvectof_matchable.cpp

## Going beyond 108 variants
Although a matchable is initially defined with up to 108 variants, it may grow as needed to achieve variant
counts higher than 108.
#### MATCHABLE_GROW(type, variant...)
Add up to 108 new variants to **type**.<br/>
This may be repeated as needed...<br/>
Example: test/programs/max_variants.cpp

## Macros for setting spreads at link-time

#### VARIANT_SPREADVARIANT(type, variant, spread_type, spread_variant)
Params:
* **type** A matchable type with spread, **spread_type**.
* **variant** A variant of **type** to have its **spread_type** set to **spread_variant**
* **spread_type** A spread type available to **type**.
* **spread_variant** A variant of **spread_type** as the new value to be set

Call **type**::**variant**::grab().set_**spread_type**(**spread_type**::**spread_variant**::grab()) at link-time.<br/>
Example: test/programs/spread_matchable.cpp

#### VARIANT_SPREADVARIANTVECT(type, variant, spread_type, spread_variant...)
Params:
* **type** A matchable type with vector of **spread_type**
* **variant** A variant of **type** to have its vector of **spread_type** set
* **spread_type** A matchable type available to **type** as a vector.
* **spread_variant..** up to 108 variants of **spread_type** forming the vector (the new value to be set)

Call **type**::**variant**::grab().set_**spread_type**_vect() with a vector formed by the given **spread_variants...** of **spread_type** at link-time.<br/>
Example: test/programs/spreadvectof_matchable.cpp<br/>

## Run-time macros

#### UNMATCHABLE(type, variant...)
Removes variants for the current scope. When the scope exits the removed variants are restored.<br/>
For matchables defined within namespaces the following macros are available:<br/>
* NAMESPACEx1_UNMATCHABLE()
* NAMESPACEx2_UNMATCHABLE()
* NAMESPACEx3_UNMATCHABLE()
* NAMESPACEx4_UNMATCHABLE()
* NAMESPACEx5_UNMATCHABLE()

Further namespace nesting beyond 5 is supported by using the Unmatchable template directly (see example)<br/>
Example: test/programs/unmatchable.cpp<br/>

#### MATCHABLE_INSTANCE_IN(type, instance, variant...) -> bool
returns true if instance is one of the given variants or false otherwise<br/>
Example:
```
bool is_weekday(DayOfWeek::Type day)
{
    return MATCHABLE_INSTANCE_IN(DayOfWeek, day, Monday, Tuesday, Wednesday, Thursday, Friday);
}
```
Example: test/programs/relationships.cpp<br/>

## Forward Declaring Matchables
To benchmark compile time performace with and without forward declarations run:
```
scripts/test_buildtimes.py
```

#### MATCHABLE_FWD(type)
Forward declare a matchable<br/>
Example:
```
#include "matchable_fwd.h"
MATCHABLE_FWD(DayOfWeek)
```

# Prerequisites

clang >= 7.0.1<br/>
or<br/>
g++ >= 8.3.0

# Building, Installing and Running Tests
```
scripts/setup.py
```
setup.py without arguments will from the project root do:
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make install
cd ..
install/test/bin/run_all.sh again_quietly
```
to specify custom build or install directories or to force use of clang see:
```
scripts/setup.py -h
```
* use of setup.py is of course optional and serves as a reference or example workflow
* setup.py will remove the build and install directories before starting!
# Running Tests / Examples
Assuming workflow above with install directory under the project root (modify paths accordingly for your
workflow).
<br/>
<br/>

List all tests:
```
install/test/bin/list.sh
```
Run Single Test (for example "max_variants"):
```
install/test/bin/run_test.sh max_variants
```
Run Quietly:
```
install/test/bin/run_test.sh max_variants quietly
```
Run All Tests:
```
install/test/bin/run_all.sh
```
Run All Tests Quietly
```
install/test/bin/run_all.sh quietly
```

# Example Source Code
Example code can be found under:
```
test/programs
```

# Versioning

For the versions available, see the [tags on this repository](https://github.com/shtroizel/matchable/tags).

# Authors

* **Eric Hyer**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

# License

This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details
