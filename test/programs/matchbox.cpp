/*
Copyright (c) 2019-2020, Eric Hyer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

#include "matchable/matchable.h"



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
PROPERTYx1_MATCHABLE(
    AttackPastTense::Type,
    past_tense,
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
SET_PROPERTY(Attack, covers, past_tense, AttackPastTense::covered::grab())
SET_PROPERTY(Attack, crushes, past_tense, AttackPastTense::crushed::grab())
SET_PROPERTY(Attack, cuts, past_tense, AttackPastTense::cut::grab())
SET_PROPERTY(Attack, decapitates, past_tense, AttackPastTense::decapitated::grab())
SET_PROPERTY(Attack, disproves, past_tense, AttackPastTense::disproved::grab())
SET_PROPERTY(Attack, eats, past_tense, AttackPastTense::ate::grab())
SET_PROPERTY(Attack, poisons, past_tense, AttackPastTense::poisoned::grab())
SET_PROPERTY(Attack, smashes, past_tense, AttackPastTense::smashed::grab())
SET_PROPERTY(Attack, vaporizes, past_tense, AttackPastTense::vaporized::grab())

static matchable::MatchBox<Actor::Type, Attack::Type> const rock_attack({
    { Actor::Lizard::grab(), Attack::crushes::grab() },
    { Actor::Scissors::grab(), Attack::crushes::grab() },
});
static matchable::MatchBox<Actor::Type, Attack::Type> const paper_attack({
    { Actor::Rock::grab(), Attack::covers::grab() },
    { Actor::Spock::grab(), Attack::disproves::grab() },
});
static matchable::MatchBox<Actor::Type, Attack::Type> const scissors_attack({
    { Actor::Paper::grab(), Attack::cuts::grab() },
    { Actor::Lizard::grab(), Attack::decapitates::grab() },
});
static matchable::MatchBox<Actor::Type, Attack::Type> const lizard_attack({
    { Actor::Spock::grab(), Attack::poisons::grab() },
    { Actor::Paper::grab(), Attack::eats::grab() },
});
static matchable::MatchBox<Actor::Type, Attack::Type> const spock_attack({
    { Actor::Scissors::grab(), Attack::smashes::grab() },
    { Actor::Rock::grab(), Attack::vaporizes::grab() },
});
static matchable::MatchBox<Actor::Type, matchable::MatchBox<Actor::Type, Attack::Type>> const attack({
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

    actor_0 = Actor::from_by_string_index(actor_distribution(engine));
    actor_1 = Actor::from_by_string_index(actor_distribution(engine));
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
            std::cout << actor_0 << " " << attack_0.as_past_tense() << " " << actor_1 << std::endl;
        else if (!attack_1.is_nil())
            std::cout << actor_1 << " " << attack_1.as_past_tense() << " " << actor_0 << std::endl;
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
    matchable::MatchBox<Actor::Type, int> win_counts{0};
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
