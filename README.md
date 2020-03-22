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
## Complete Example
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
SPREAD_MATCHABLE(
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
SPREADVARIANT_VARIANTS(AttackPastTense, covered, Attack, covers)
SPREADVARIANT_VARIANTS(AttackPastTense, crushed, Attack, crushes)
SPREADVARIANT_VARIANTS(AttackPastTense, cut, Attack, cuts)
SPREADVARIANT_VARIANTS(AttackPastTense, decapitated, Attack, decapitates)
SPREADVARIANT_VARIANTS(AttackPastTense, disproved, Attack, disproves)
SPREADVARIANT_VARIANTS(AttackPastTense, ate, Attack, eats)
SPREADVARIANT_VARIANTS(AttackPastTense, poisoned, Attack, poisons)
SPREADVARIANT_VARIANTS(AttackPastTense, smashed, Attack, smashes)
SPREADVARIANT_VARIANTS(AttackPastTense, vaporized, Attack, vaporizes)

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
