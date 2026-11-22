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

    // from_string_as_ref() and from_by_string_index_as_ref() do the same lookups as from_string()
    // and from_by_string_index() but return a reference to the stored variant instead of a copy,
    // so they don't allocate
    // the reference stays valid until variants are added or removed
    DayOfWeek::Type const & monday = DayOfWeek::from_string_as_ref("Monday");
    std::cout << "\nfrom_string_as_ref(\"Monday\") --> " << monday << std::endl;

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

## Choosing the by-string order
By default, by-string order (variants_by_string(), as_by_string_index(), lt_by_string()) compares names as
unsigned bytes, the order of std::string.<br/>

#### MATCHABLE_SORTKEY(type, key)
Orders **type** by key(name), then by name, both as unsigned bytes. **key** is a function that takes and
returns a std::string, for example one that folds case so "Apple" and "apple" sort next to each other.
Write it before **type** is declared, in the same header, so every translation unit sees the same order;
written after, it stops the build. A type with a sort key stores each key once, and a lookup computes the
key of the string it looks up once.<br/>

Example: test/programs/sortkey.cpp

## Going beyond 108 variants
Although a matchable is initially defined with up to 108 variants, it may grow to achieve variant counts higher than 108.
#### GROW_MATCHABLE(type, variant...)
Add up to 108 new variants to **type**.<br/>
This may be repeated as needed...<br/>
Example: test/programs/max_variants.cpp

#### Growing a matchable from several translation units
**type** may be grown from different translation units, each adding its own variants (for example a base
header declares the type and each module grows it). Every variant registers exactly once, however many
translation units include it, and the program sees all of them.<br/>
* by-string order (as_by_string_index(), variants_by_string()) compares names as unsigned bytes, the order
  of std::string, so it is the same in every build and on every platform
* by-index order (as_index(), variants_by_index()) follows declaration order for variants declared in the
  same header. Between variants grown in different translation units it follows static initialization
  order, which depends on link order, so it can differ between builds. Don't persist or exchange those
  indexes; use as_string() instead.

Example: test/programs/multi_tu/

#### Unloading grown variants
Code loaded at run time (with dlopen, or by a JIT) may grow a type and be unloaded later. Compile that code
with MATCHABLE_UNLOADABLE defined: each of its translation units records the variants it grows and
unregisters them when its static objects are destroyed, which happens when the code is unloaded or the
program exits.<br/>
* the program that loads the code exports its symbols (for example `-rdynamic`, or CMake's ENABLE_EXPORTS)
  so the loaded code shares its registry. With GCC, compile the loaded code with `-fno-gnu-unique` so
  dlclose unloads it
* a name that is already registered is not registered again, and lookups keep finding the first. To
  replace loaded code with another version, unload the old version first
* drop copies of a variant before unloading the code that grew it
* as_index() values are not reused, and from_index() still finds every remaining variant
* build code that stays loaded for the whole run without MATCHABLE_UNLOADABLE, so lookups made while static
  objects are destroyed still find its variants

Example: test/programs/grow_unload/

## Referring to variants of any matchable
Every variant derives from matchable::Variant, which gives as_string(), as_type_name(), as_by_string_index()
and as_index() without knowing the variant's type. A matchable::VariantRef refers to one, or is nil, and
works as a property type, so variants of one matchable can refer to variants of others.<br/>
* `matchable::variant_ref<type::variant>()` is a variant's reference. It's a constant, so tables of
  references need no code to fill them, and it needs only a declaration of the variant
  (`namespace type { class variant; }`), not its matchable
* `->` and `*` reach the variant, get() gives a `matchable::Variant const *` (nullptr when nil), and
  **type**::from_variant(ref) gives the typed variant back, or nil when the reference is nil or to another
  type
* code split into libraries with hidden visibility defines each variant's instance in the library that
  owns it (`template struct __attribute__((visibility("default"))) matchable::Instance<type::variant>;`)
  and declares it everywhere else (`extern template struct matchable::Instance<type::variant>;`). The
  linker resolves references to the owner's instance, which answers from the owner's registry. The owner
  defines it before its own first use, or GCC ignores the visibility

Examples: test/programs/variant_ref.cpp, test/programs/variant_ref_lib/

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
To benchmark compile time performance with and without forward declarations run:
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
Build and run tests that need only C++14, using $CXX (default g++) with -std=gnu++1y (-a omits by index):
```
test_scripts/cxx14.sh
```
CI runs the full suite with gcc and clang, with and without sanitizers, plus the C++14 tests with GCC 4.8.

# Measuring Performance
Build and lookup times depend on the machine and its load, so they compare best within one run on one
machine: each script can build a second copy with another matchable.h and print ratios. Each script lists
its options with -h.

#### scripts/test_buildtimes.py
Compiler cpu time for builds using full definitions vs forward declarations (default), for growing types
(-s), or for a sweep of variants per type (--sweep), with each compile timed on its own<br/>
-b \<include dir\> also builds every case with another matchable.h and prints the ratio
```
scripts/test_buildtimes.py -b ../other-matchable/include
scripts/test_buildtimes.py --sweep=10,50,100,250,500,1000 -j 6
```

#### scripts/bench_lookup.py
ns per variants_by_string_index_of(), from one variant looked up over and over to random variants of 1000
types, plus misses<br/>
-p pins the timed program to one cpu, -b compares with another matchable.h
```
scripts/bench_lookup.py -p 2 -b ../other-matchable/include
scripts/bench_lookup.py --sweep=10,25,50,100,250,500,1000 -p 2
```

#### scripts/plot_sweep.py
Plots --sweep results against variants per type and fits a curve to each case<br/>
Needs numpy and matplotlib (for example the python3-numpy and python3-matplotlib packages)
```
scripts/plot_sweep.py build_sweep_buildtimes.csv build_bench_lookup/sweep.csv
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
* monero: 4BETuKtvLjkT7VBW85HLJ1XPqhr1TqQj11UmEMnphgG4DzNtSgJmK8b3ZmTudnbbjf7oHfNpCPxDG3BWnmVSCrtrNXMnhyv<br/>
