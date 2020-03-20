# MATCHABLE

seemlessly flow<br/>
between strings and identifiers<br/>
itemized in groups<br/>
that can spread forming relationships<br/>
as a compile-time tool<br/>
for inconspicuous efficiency<br/>

## Getting Started
MATCHABLE is implemented by:<br/>
* src/matchable.h<br/>
* src/matchable_fwd.h
## Matchable API
Matchables are created using the macro API (see Macro API below). Here we describe what can be
done with those types that are already created, as a complete example:
```
#include <iostream>

#include "matchable.h"



MATCHABLE(TimeUnit, Seconds, Minutes, Hours, Days, Weeks)



int main()
{
    {
        // default construction
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

    {
        // copy construction
        TimeUnit::Type copied{time_unit};
    }

    {
        // move construction / assignment
        TimeUnit::Type tu = TimeUnit::Minutes::grab();
        TimeUnit::Type move_constructed{std::move(tu)};
        TimeUnit::Type move_assigned = std::move(move_constructed);
    }

    {
        // same as lt_by_index())
        bool lt = TimeUnit::Seconds::grab() < time_unit; // lt is true
        (void) lt;

        // string compare
        lt = TimeUnit::Seconds::grab().lt_by_string(time_unit); // lt is false

        // enum order compare
        lt = TimeUnit::Seconds::grab().lt_by_index(time_unit); // lt is true
    }

    {
        // equality compare
        bool eq = TimeUnit::Seconds::grab() == time_unit; // eq is false
        (void) eq;
        eq = TimeUnit::Seconds::grab() != time_unit; // eq is true
    }

    {
        // as_string()
        std::string s = time_unit.as_string(); // s is "Minutes"
        s = TimeUnit::nil.as_string(); // s is "nil"
        s = TimeUnit::Type().as_string(); // s is "nil"
    }

    {
        // from_string()
        TimeUnit::Type tu = TimeUnit::from_string("107"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("nil"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("Weeks"); // tu is TimeUnit::Weeks::grab()
    }

    {
        // as_index()
        int index = TimeUnit::Seconds::grab().as_index(); // index is 0
        (void) index;
        index = TimeUnit::Days::grab().as_index(); // index is 3
    }

    {
        // from_index()
        TimeUnit::Type tu = TimeUnit::from_index(4); // tu is TimeUnit::Weeks::grab()
        tu = TimeUnit::from_index(107); // tu is TimeUnit::nil
    }

    // is_nil()
    if (time_unit.is_nil())
        exit(EXIT_FAILURE);

    {
        // flags
        TimeUnit::Flags flags;
        bool b = flags.is_set(TimeUnit::Seconds::grab()); // b is false (all are unset by default)
        (void) b;
        flags.set(TimeUnit::Hours::grab());
        b = flags.is_set(TimeUnit::Hours::grab()); // b is true

        // copy flags
        TimeUnit::Flags more_flags{flags};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is true

        // clear flags
        more_flags = TimeUnit::Flags{};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is false
    }

    {
        // match()
        time_unit.match({
            { TimeUnit::Seconds::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Minutes::grab(), [](){ std::cout << "match!" << std::endl; } },
            { TimeUnit::Hours::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Days::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Weeks::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::nil, [](){ exit(EXIT_FAILURE); } },
        }); // tu is TimeUnit::Minutes::grab()
    }

    // traversal, variants(), operator<<()
    TimeUnit::variants().size(); // 5
    std::cout << "Printing some TimeUnit variants..." << std::endl;
    for (auto variant : TimeUnit::variants())
        if (MATCHABLE_INSTANCE_IN(TimeUnit, variant, Hours, Days, Weeks))
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
        { TimeUnit::Weeks::grab(), [](){ exit(EXIT_SUCCESS); } },
    });

    return EXIT_FAILURE;
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
VARIANT_SPREADVARIANTVECT(type, variant, spread_type, spread_variant...)
```
Example: test/programs/spreadvectof_matchable.cpp
#### Remove variants for the current scope with:
```
UNMATCHABLE(type, variant...)
```
when the scope exits the removed variants are restored<br/>
Example: test/programs/unmatchable.cpp
### Macros for working with matchable instances
#### Test if a matchable instance matches one of some given variants
```
MATCHABLE_INSTANCE_IN(type, instance, variant...) -> bool
```
returns true if instance is one of the given variants or false otherwise
### Forward declare a MATCHABLE type - TimeUnit for example:
```
#include "matchable_fwd.h"
MATCHABLE_FWD(TimeUnit)
```
To understand why MATCHABLE_FWD exists run:
```
scripts/test_buildtimes.py
```
### Prerequisites

clang >= 7.0.1<br/>
or<br/>
g++ >= 8.3.0

### Building, Installing and Running Tests In One Step
```
scripts/setup.sh
```
setup.sh without arguments will from the project root do:
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
scripts/setup.sh --help
```
* use of setup.sh is of course optional and serves as a reference or example workflow
* setup.sh will remove the build and install directories before starting!
### Running Tests / Examples
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
