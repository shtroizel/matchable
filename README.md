# MATCHABLE

seemlessly flow<br/>
between strings and identifiers<br/>
itemized in groups<br/>
that can spread forming relationships<br/>
as a compile-time tool<br/>
for inconspicuous efficiency<br/>

## Getting Started
MATCHABLE is implemented entirely by src/matchable.h
## Matchable API
Matchables are created using the macro API (see Macro API below). Here we describe what can be
done with those types that are already created, as a complete example:
```
#include <iostream>

#include "matchable.h"



MATCHABLE(TimeUnit, Seconds, Minutes, Hours, Days, Weeks)



int main()
{
    // default construction
    {
        TimeUnit::Type const default_constructed;
    }

    // static functions returning TimeUnit::Type
    TimeUnit::Seconds::grab();
    TimeUnit::Minutes::grab();
    TimeUnit::Hours::grab();
    TimeUnit::Days::grab();
    TimeUnit::Weeks::grab();

    // assignment
    TimeUnit::Type const time_unit = TimeUnit::Minutes::grab();

    // copy construction / assignment
    {
        TimeUnit::Type copied{time_unit};
        TimeUnit::Type assigned = time_unit;
    }

    // move construction / assignment
    {
        TimeUnit::Type tu = TimeUnit::Minutes::grab();
        TimeUnit::Type move_constructed{std::move(tu)};
        TimeUnit::Type move_assigned = std::move(move_constructed);
    }

    {
        // string compare! (same as lt_alphabetic())
        bool lt = TimeUnit::Seconds::grab() < time_unit; // lt is false

        // explicit string compare
        lt = TimeUnit::Seconds::grab().lt_alphabetic(time_unit); // lt is false

        // enum order compare
        lt = TimeUnit::Seconds::grab().lt_enum_order(time_unit); // lt is true
    }

    {
        // equality compare
        bool eq = TimeUnit::Seconds::grab() == time_unit; // eq is false
        eq = TimeUnit::Seconds::grab() != time_unit; // eq is true
    }

    {
        // as_string()
        std::string s = time_unit.as_string(); // s is "Minutes"
        s = TimeUnit::nil.as_string(); // s is "nil"
        s = TimeUnit::Type().as_string(); // s is "nil"
    }

    // from_string()
    {
        TimeUnit::Type tu = TimeUnit::from_string("107"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("nil"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("Weeks"); // tu is TimeUnit::Weeks::grab()
    }

    // as_enum()
    switch (time_unit.as_enum())
    {
        case TimeUnit::Enum::Seconds:
        case TimeUnit::Enum::Hours:
        case TimeUnit::Enum::Days:
        case TimeUnit::Enum::Weeks:
        case TimeUnit::Enum::nil: return -1;
        case TimeUnit::Enum::Minutes:;
        // no default, compile warning as error better than runtime default
    }

    {
        // as_index()
        int index = TimeUnit::Seconds::grab().as_index(); // index is 0
        index = TimeUnit::Days::grab().as_index(); // index is 3
    }

    {
        // from_index()
        TimeUnit::Type tu = TimeUnit::from_index(4); // tu is TimeUnit::Weeks::grab()
        tu = TimeUnit::from_index(107); // tu is TimeUnit::nil
    }

    // is_nil();
    time_unit.is_nil(); // false

    {
        // flags
        TimeUnit::Flags flags;
        bool b = flags.is_set(TimeUnit::Seconds::grab()); // b is false (all are unset by default)
        flags.set(TimeUnit::Hours::grab());
        b = flags.is_set(TimeUnit::Hours::grab()); // b is true

        // copy flags
        TimeUnit::Flags more_flags{flags};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is true

        // clear flags
        more_flags = TimeUnit::Flags{};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is false
    }

    // match()
    {
        TimeUnit::Type const tu = [](){ return TimeUnit::Minutes::grab(); }().match({
            { TimeUnit::Seconds::grab(), [](){ exit(-1); } },
            { TimeUnit::Minutes::grab(), [](){ std::cout << "match!" << std::endl; } },
            { TimeUnit::Hours::grab(), [](){ exit(-1); } },
            { TimeUnit::Days::grab(), [](){ exit(-1); } },
            { TimeUnit::Weeks::grab(), [](){ exit(-1); } },
            { TimeUnit::nil, [](){ exit(-1); } },
        }); // tu is TimeUnit::Minutes::grab()
    }

    // traversal, variants(), operator<<()
    TimeUnit::variants().size(); // 5
    std::cout << "Traversing TimeUnit variants..." << std::endl;
    for (auto variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    {
        // remove Days and Weeks
        UNMATCHABLE(TimeUnit, Days, Weeks);
        std::cout << "Traversing TimeUnit variants with Days and Weeks removed..." << std::endl;
        for (auto variant : TimeUnit::variants())
            std::cout << "    " << variant << std::endl;
    }

    std::cout << "Traversing TimeUnit variants with Days and Weeks restored..." << std::endl;
    for (auto variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    // return success
    [](int magic){ if (magic == 107) return TimeUnit::Weeks::grab(); return TimeUnit::nil; }(107).match({
        { TimeUnit::Weeks::grab(), [](){ exit(0); } },
    });

    return -1;
}

```
## Macro-API<a name="macro_api"></a>
macro names<br/>
mirror their parameters<br/>
respectively<br/>
<br/>
where a MATCHABLE is a type followed by its variants
<br/>
<br/>
for documention here note that backticks are used to notate parameter substitution
### Macros for creating new types
#### Create a new type and its variants with:
```
MATCHABLE(type, variant...)
```
Where: variant... is: in [0..108] comma separated variants<br/>
Example: test/programs/matchable.cpp
#### Create a new type and its variants having a matchable property with:
```
SPREAD_MATCHABLE(spread, type, variant...)
```
Where: spread is: a type defined by MATCHABLE() or SPREAD\*\_MATCHABLE()<br/>
Injects:
```
void set_`spread`(`spread`::Type)
`spread`::Type as_`spread`() const
```
as member functions of \`type\`::Type<br/>
Example: test/programs/spread_matchable.cpp
#### Create a new type and its variants having 2 matchable properties with:
```
SPREADx2_MATCHABLE(spread0, spread1, type, variant...)
```
Where: spreads are: independent types defined by MATCHABLE() or SPREAD\*\_MATCHABLE()<br/>
Injects:
```
void set_`spread0`(`spread`::Type)
`spread0`::Type as_`spread0`() const
void set_`spread1`(`spread1`::Type)
`spread1`::Type as_`spread1`() const
```
as member functions of \`type\`::Type<br/>
Example: test/programs/spreadx2matchable.cpp
#### Create a new type and its variants having a "vector of matchable" property with:
```
SPREADVECTOF_MATCHABLE(spread, type, variant...)
```
Where: spread is: a type defined by MATCHABLE() or SPREAD\*\_MATCHABLE()<br/>
Injects:
```
void set_`spread`_vect(std::vector<`spread`::Type>)
std::vector<`spread`::Type> as_`spread`_vect() const
```
as member functions of \`type\`::Type<br/>
Example: test/programs/spreadvectof_matchable.cpp
### Macros manipulating existing types
#### Call set_\`spread_type\`(\`spread_type\`::\`spread_variant\`::grab()) with each \`variant...\` of \`type\` at link-time with:
```
SPREADVARIANT_VARIANTS(spread_type, spread_variant, type, variant...)
```
Example: test/programs/spread_matchable.cpp
#### Call \`type\`::\`variant\`::grab().set_\`spread_type\`_vect() with a vector formed by the given \`spread_variants...\` of \`spread_type\` at link-time with:
```
VARIANT_SPREADVARIANTVECT(type, variant, spread_type, spread_variants...)
```
Example: test/programs/spreadvectof_matchable.cpp
#### Remove variants for the current scope with:
```
UNMATCHABLE(type, variant...)
```
when the scope exits the removed variants are restored<br/>
Example: test/programs/unmatchable.cpp
### Prerequisites

clang >= 7.0.1-8<br/>
or<br/>
eg++ >= 8.3.0

### Running Tests / Examples

Example workflow from project root:
```
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER="/usr/bin/clang++" -DCMAKE_INSTALL_PREFIX=../install ..
make install
cd ..
```

Assuming workflow above with install directory under the project root (modify paths accordingly for your workflow).
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

### Example Source Code
Example code can be found under:
```
test/programs
```

## Versioning

For the versions available, see the [tags on this repository](https://github.com/shtroizel/matchable/tags).

## Authors

* **Eric Hyer**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

## License

This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details
