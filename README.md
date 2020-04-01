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
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

#include "matchable.h"



MATCHABLE(Actor, Rock, Paper, Scissors, Lizard, Spock)

MATCHABLE(
    AttackPastTense,
    covered,
    crushed,
    cut,
    decapitated,
    disproved,
    ate,
    poisoned,
    smashed,
    vaporized
)
SPREADx1_MATCHABLE(
    AttackPastTense,
    Attack,
    covers,
    crushes,
    cuts,
    decapitates,
    disproves,
    eats,
    poisons,
    smashes,
    vaporizes
)
VARIANT_SPREADVARIANT(Attack, covers, AttackPastTense, covered)
VARIANT_SPREADVARIANT(Attack, crushes, AttackPastTense, crushed)
VARIANT_SPREADVARIANT(Attack, cuts, AttackPastTense, cut)
VARIANT_SPREADVARIANT(Attack, decapitates, AttackPastTense, decapitated)
VARIANT_SPREADVARIANT(Attack, disproves, AttackPastTense, disproved)
VARIANT_SPREADVARIANT(Attack, eats, AttackPastTense, ate)
VARIANT_SPREADVARIANT(Attack, poisons, AttackPastTense, poisoned)
VARIANT_SPREADVARIANT(Attack, smashes, AttackPastTense, smashed)
VARIANT_SPREADVARIANT(Attack, vaporizes, AttackPastTense, vaporized)

static MatchBox<Actor::Type, Attack::Type> const rock_attack({
    { Actor::Lizard::grab(), Attack::crushes::grab() },
    { Actor::Scissors::grab(), Attack::crushes::grab() },
});
static MatchBox<Actor::Type, Attack::Type> const paper_attack({
    { Actor::Rock::grab(), Attack::covers::grab() },
    { Actor::Spock::grab(), Attack::disproves::grab() },
});
static MatchBox<Actor::Type, Attack::Type> const scissors_attack({
    { Actor::Paper::grab(), Attack::cuts::grab() },
    { Actor::Lizard::grab(), Attack::decapitates::grab() },
});
static MatchBox<Actor::Type, Attack::Type> const lizard_attack({
    { Actor::Spock::grab(), Attack::poisons::grab() },
    { Actor::Paper::grab(), Attack::eats::grab() },
});
static MatchBox<Actor::Type, Attack::Type> const spock_attack({
    { Actor::Scissors::grab(), Attack::smashes::grab() },
    { Actor::Rock::grab(), Attack::vaporizes::grab() },
});
static MatchBox<Actor::Type, MatchBox<Actor::Type, Attack::Type>> const attack({
    { Actor::Rock::grab(), rock_attack },
    { Actor::Paper::grab(), paper_attack },
    { Actor::Scissors::grab(), scissors_attack },
    { Actor::Lizard::grab(), lizard_attack },
    { Actor::Spock::grab(), spock_attack },
});



void print_rules()
{
    std::cout << "********* Rules *********";
    for (auto attacker : Actor::variants())
    {
        std::cout << "\n" << attacker << ":" << std::endl;
        for (auto attacked : Actor::variants())
        {
            Attack::Type att = attack.at(attacker).at(attacked);
            if (!att.is_nil())
                std::cout << "    " << att << " " << attacked << std::endl;
        }
    }
    std::cout << "*************************\n" << std::endl;
}


void choose_actors(Actor::Type & actor_0, Actor::Type & actor_1)
{
    static std::mt19937 engine{std::random_device()()};
    static std::uniform_int_distribution<int> actor_distribution{0, (int) Actor::variants().size() - 1};

    actor_0 = Actor::from_index(actor_distribution(engine));
    actor_1 = Actor::from_index(actor_distribution(engine));
}


MATCHABLE(compare_actors_Verbosity, Print, Silent)
Actor::Type compare_actors(Actor::Type actor_0, Actor::Type actor_1, compare_actors_Verbosity::Type print)
{
    Attack::Type attack_0 = attack.at(actor_0).at(actor_1);
    Attack::Type attack_1 = attack.at(actor_1).at(actor_0);

    Actor::Type winner;
    if (!attack_0.is_nil())
        winner = actor_0;
    else if (!attack_1.is_nil())
        winner = actor_1;

    if (print == compare_actors_Verbosity::Print::grab())
    {
        std::cout << "comparing " << actor_0 << " with " << actor_1 << ":\n    ";
        if (!attack_0.is_nil())
            std::cout << actor_0 << " " << attack_0.as_AttackPastTense() << " " << actor_1 << std::endl;
        else if (!attack_1.is_nil())
            std::cout << actor_1 << " " << attack_1.as_AttackPastTense() << " " << actor_0 << std::endl;
        else
            std::cout << "tie!" << std::endl;
        std::cout << std::endl;
    }

    return winner;
}


int main()
{
    Actor::Type actor_0;
    Actor::Type actor_1;

    print_rules();

    for (int i = 0; i < 108; ++i)
    {
        choose_actors(actor_0, actor_1);
        compare_actors(actor_0, actor_1, compare_actors_Verbosity::Print::grab());
    }

    int const TRIALS{46656};
    int const TRIALS_PER_DOT{27};
    std::cout << "108 trials run, now just start printing one '.' per " << TRIALS_PER_DOT << " trials"
              << std::flush;
    MatchBox<Actor::Type, int> win_counts{0};
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TRIALS; ++i)
    {
        if (i % TRIALS_PER_DOT == 0)
            std::cout << "." << std::flush;
        choose_actors(actor_0, actor_1);
        Actor::Type winner = compare_actors(actor_0, actor_1, compare_actors_Verbosity::Silent::grab());
        win_counts.mut_at(winner) += 1;
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\ncompleted " << TRIALS << " trials using a single thread in " << duration.count()
              << " milliseconds" << std::endl;

    std::string const TITLE{"Summary Win Counts"};
    int const WIDTH = TITLE.size();
    std::cout << "\n" << TITLE << ":" << std::endl;
    for (auto actor : Actor::variants())
        std::cout << std::setw(WIDTH) << actor << ": " << win_counts.at(actor) << std::endl;
    std::cout << std::setw(WIDTH) << "TIE" << ": " << win_counts.at(Actor::nil) << std::endl;

    return 0;
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

Further namespace nesting beyond 5 is supported by using the Unmatchable template directly (see unmatchable.cpp)
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
