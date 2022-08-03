# MATCHABLE
Matchable provides string-convertable and flaggable enumerated types<br/>
These types can have properties, and these properties can have observers<br/>
Examples can be found under test/programs<br/>

## Files Needed For Getting Started
Matchable is primarily a header-only library. To get started quickly you could just copy the following
two files into your project:

* include/matchable/matchable.h<br/>
* include/matchable/matchable_fwd.h<br/>

If using cmake then you could instead install matchable, set matchable_DIR and use find_package()
within your project.<br/>


# Hello Matchable
The following program can be found at: test/programs/matchable_usage.cpp<br/>
```cpp
#include <iostream>
#include "matchable/matchable.h"


MATCHABLE(
    // type
    DayOfWeek,

    // variants
    Monday,
    Tuesday,
    Wednessday,
    Thursday,
    Friday,
    Saturday,
    Sunday
)


MATCHABLE(
    Status,

    // escape codes can be used for characters that are invalid for identifiers
    success_bng_,              // Status::success_bng_::grab().as_string() == "success!"
    invalid_spc_input,         // Status::invalid_spc_input::grab().as_string() == "invalid input"
    timed_spc_out,             // Status::from_string("timed out") == Status::timed_spc_out::grab()
    nothing_spc_to_spc_do,
    insufficient_spc_memory,
    index_spc_out_spc_of_spc_bounds,

    // optionally, variants can be prefixed with esc_ as needed
    esc_17                     // Status::esc_17::grab().as_string() == "17"

    // see bottom of matchable/matchable.h for all the escape codes
    // also reference test/programs/escapable.cpp for more on this
);


Status::Type foo(std::string day_string)
{
    // create an instance of the DayOfWeek matchable
    DayOfWeek::Type day_of_week;

    // all matchable types have a hidden "nil" variant
    // at this point day_of_week == DayOfWeek::nil
    assert(day_of_week == DayOfWeek::nil);

    // nil is a valid and usable variant but its syntax is a bit different
    // notice how we don't "grab()" nil like we do for the other variants
    // also, to test for nil we could alternatively do:
    assert(day_of_week.is_nil());

    // lets try to assign the variant from the day_string parameter
    // note the complexity for from_string() is O(log N)
    // if day_string is equal to the output of "as_string()" for any defined variant,
    // then day_of_week will be set to that variant
    // if no variant is found then nil will be assigned instead
    day_of_week = DayOfWeek::from_string(day_string);
    if (day_of_week.is_nil())
        return Status::invalid_spc_input::grab();

    // to show how flags work, we'll create a flags instance and flag our favorite days
    DayOfWeek::Flags favorite_days;
    for (int i = 0; i < 3; ++i)
        favorite_days.set(DayOfWeek::from_index(i)); // note the complexity for from_index is O(1)

    // oops, actually we don't like Wednessday after all
    favorite_days.unset(DayOfWeek::Wednessday::grab());

    // to get all flagged variants we can use currently_set()
    for (auto day : favorite_days.currently_set())
        if (day == day_of_week)
            std::cout << day << " is one of my favorite days! ";

    // we can also use is_set() to check for flagged variants
    if (favorite_days.is_set(DayOfWeek::Friday::grab()))
        return Status::esc_17::grab();

    // we could also use MATCHABLE_INSTANCE_IN()
    if (!MATCHABLE_INSTANCE_IN(DayOfWeek, day_of_week, Monday, Tuesday))
        return Status::nothing_spc_to_spc_do::grab();

    return Status::success_bng_::grab();
}


int main()
{
    std::cout << "variant iteration by index:" << std::endl;
    for (auto day_of_week : DayOfWeek::variants_by_index())
        std::cout << "    " << day_of_week << std::endl;

    std::cout << "\nvariant iteration by string (alphabetic):" << std::endl;
    for (auto day_of_week : DayOfWeek::variants_by_string())
        std::cout << "    " << day_of_week << std::endl;

    std::cout << "\noutput of foo(\"bad input\") --> " << foo("bad input") << std::endl;
    std::cout << "output of foo(\"Saturday\") --> " << foo("Saturday") << std::endl;
    std::cout << "output of foo(\"Tuesday\") --> " << foo("Tuesday") << std::endl;

    return 0;
}

```
Output for the program above:<br/>
```
$  ./matchable_usage
variant iteration by index:
    Monday
    Tuesday
    Wednessday
    Thursday
    Friday
    Saturday
    Sunday

variant iteration by string (alphabetic):
    Friday
    Monday
    Saturday
    Sunday
    Thursday
    Tuesday
    Wednessday

output of foo("bad input") --> invalid input
output of foo("Saturday") --> nothing to do
output of foo("Tuesday") --> Tuesday is one of my favorite days! success!
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


Examples:<br/>
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
PROPERTYx8_MATCHABLE()<br/>
PROPERTYx9_MATCHABLE()<br/>
PROPERTYx10_MATCHABLE()<br/>
PROPERTYx11_MATCHABLE()<br/>
PROPERTYx12_MATCHABLE()<br/>
PROPERTYx13_MATCHABLE()<br/>
PROPERTYx14_MATCHABLE()<br/>
PROPERTYx15_MATCHABLE()<br/>
PROPERTYx16_MATCHABLE()<br/>
PROPERTYx17_MATCHABLE()<br/>
PROPERTYx18_MATCHABLE()<br/>
PROPERTYx19_MATCHABLE()<br/>
PROPERTYx20_MATCHABLE()<br/>
PROPERTYx21_MATCHABLE()<br/>

Example: test/programs/relationships.cpp

## Going beyond 108 variants
Although a matchable is initially defined with up to 108 variants, it may spread to achieve variant counts higher than 108.
#### SPREAD_MATCHABLE(type, variant...)
Add up to 108 new variants to **type**.<br/>
This may be repeated as needed...<br/>
Example: test/programs/max_variants.cpp

## Macros for setting properties at link-time

#### MATCHABLE_VARIANT_PROPERTY_VALUE(type, variant, property_name, property_value)
Params:
* **type** A matchable type
* **variant** A variant of **type**
* **property_name** A property available to **type**.
* **property_value** The new value to be set

Calls **type**::**variant**::grab().set_**property_name**(**property_value**) at link-time.<br/>
Examples:<br/>
test/programs/cards.cpp<br/>
test/programs/relationships.cpp<br/>

#### MATCHABLE_VARIANT_PROPERTY_VALUES(type, variant, property_name, property_values...)
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
#include "matchable/matchable_fwd.h"
MATCHABLE_FWD(DayOfWeek)
```

# Building, Installing and Running Tests
```
scripts/build_and_install.py
```
build_and_install.py without arguments will from the project root do:
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make install
cd ..
```

refer to help for custom build/install directories and other options:
```
scripts/build_and_install.py -h
```
* use of build_and_install.py is of course optional and serves as a reference or example workflow
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
```

# Supported Systems
Artix Linux, amd64<br/>
Gentoo Linux, sparc64<br/>
<br/>
Matchable should work on any unix-like system but only Artix and Gentoo are supported.
OpenBSD, sparc64 is also known to work but you may need to use the "-c" option when building.

# Authors

* **shtroizel**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

# License
This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details

# Donating To This Project
donations are greatly appreciated, thank you!<br/>
* monero: 89VkK94RMHSTjSsR28wGR2PLyMt4i6jndYmQb3xWSVoKBMaLAVMYXkSL7Eoe3ES9rDKFar5XNQMw7ZmUF9PNJ4WwFaNgebx<br/>
