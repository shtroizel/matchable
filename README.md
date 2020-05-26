# Matchable Header Library
* include/matchable/matchable.h<br/>
* include/matchable/matchable_fwd.h
# Complete Example
The following program can be found at: test/programs/matchable_usage.cpp
```cpp
#include <random>
#include <iostream>

#include "matchable/matchable.h"



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
                    std::cout << status::nil.as_string() << "!" << std::endl;
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
        // operator<<() is provided, so as_string() can be omitted...
        std::cout << "match: " << status::nil << "!" << std::endl;
    }

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

#### PROPERTYx1_MATCHABLE(property_type, property_name, type, variant...)
Same as MATCHABLE(), but injects a property called **property_name** of type **property_type**<br/>
Params:
* **property_type** type of injected property<br/>
* **property_name** name of injected property<br/>
* **type** name of the new "matchable" to be created<br/>
* **variant...** 0..108 comma separated variants of the new matchable<br/>
<br/>
The property is available both as a single value and as a vector with:<br/>

**property_type** const & as_**property_name**()<br/>
std::vector<**property_type**> const & as_**property_name**_vect()<br/>

void set_**property_name**(**property_type** const &)<br/>
void set_**property_name**_vect(std::vector<**property_type**> const &)<br/>


Examples:<br>
test/programs/cards.cpp<br/>
test/programs/matchbox.cpp<br/>
test/programs/sorting.cpp<br/>

#### PROPERTYx*_MATCHABLE()
Similar to PROPERTYx1_MATCHABLE(), the following macros exist for injecting multiple properties when creating matchables.<br/>

PROPERTYx2_MATCHABLE()<br/>
PROPERTYx3_MATCHABLE()<br/>
PROPERTYx4_MATCHABLE()<br/>
PROPERTYx5_MATCHABLE()<br/>
PROPERTYx6_MATCHABLE()<br/>
PROPERTYx7_MATCHABLE()<br/>

Example: test/programs/relationships.cpp

## Going beyond 108 variants
Although a matchable is initially defined with up to 108 variants, it may spread to achieve variant counts higher than 108.
#### SPREAD_MATCHABLE(type, variant...)
Add up to 108 new variants to **type**.<br/>
This may be repeated as needed...<br/>
Example: test/programs/max_variants.cpp

## Macros for setting properties at link-time

#### SET_PROPERTY(type, variant, property_name, property_value)
Params:
* **type** A matchable type
* **variant** A variant of **type**
* **property_name** A property available to **type**.
* **property_value** The new value to be set

Calls **type**::**variant**::grab().set_**property_name**(**property_value**) at link-time.<br/>
Examples:<br/>
test/programs/cards.cpp<br/>
test/programs/relationships.cpp<br/>

#### SET_PROPERTY_VECT(type, variant, property_name, property_values...)
Params:
* **type** A matchable type
* **variant** A variant of **type**
* **property_name** A property available to **type**
* **property_values...** The new values to be set

Calls **type**::**variant**::grab().set_**property_name**_vect() with a vector formed by the given **property_values...**<br/>
Example: test/programs/relationships.cpp<br/>

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
install/share/matchable/test/bin/run_all.sh again_quietly
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
install/share/matchable/test/bin/list.sh
```
Run Single Test (for example "max_variants"):
```
install/share/matchable/test/bin/run_test.sh max_variants
```
Run Quietly:
```
install/share/matchable/test/bin/run_test.sh max_variants quietly
```
Run All Tests:
```
install/share/matchable/test/bin/run_all.sh
```
Run All Tests Quietly
```
install/share/matchable/test/bin/run_all.sh quietly
```

# Example Source Code
Example code can be found under:
```
test/programs
```

# Creating Matchables at Runtime
libmatchable provides "MatchableMaker" for creating header files with matchable definitions programmatically

## libmatchable
both static and shared libraries can be found under install/lib/matchable

## MatchableMaker
Usage with examples comming soon...

# Versioning
For the versions available, see the [tags on this repository](https://github.com/shtroizel/matchable/tags).

# Authors

* **Eric Hyer**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

# License

This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details
