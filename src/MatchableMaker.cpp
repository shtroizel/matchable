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



#include <matchable/MatchableMaker.h>

#include <algorithm>
#include <cstdio>
#include <map>
#include <iostream>

#include <matchable/matchable.h>



namespace matchable
{
    void Matchable::add_variant(std::string const & name)
    {
        if (std::find(variants.begin(), variants.end(), name) == variants.end())
        {
            MatchableVariant v;
            v.variant_name = name;
            variants.push_back(v);
        }
    }


    bool Matchable::add_spread(std::string const & spread_type, std::string const & spread_name)
    {
        for (auto const & [type, name] : spread_types_and_names)
            if (name == spread_name)
                return false;

        spread_types_and_names.push_back(std::make_pair(spread_type, spread_name));
        return true;
    }


    set_spread_status::Type Matchable::set_spread(
        std::string const & variant,
        std::string const & name,
        std::string const & value
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_spread_status::Type ret = verify_spread_and_variant__and__get_variant_iter(name, variant, iter);
        if (ret != set_spread_status::success::grab())
            return ret;

        auto spread_iter = std::find_if(
            iter->spreads.begin(),
            iter->spreads.end(),
            [&](auto const & s) { return std::get<0>(s) == name; }
        );

        if (spread_iter != iter->spreads.end())
            std::get<1>(*spread_iter) = value;
        else
            iter->spreads.push_back(std::make_tuple(name, value, std::vector<std::string>{}));

        return ret;
    }


    set_spreadvect_status::Type Matchable::set_spreadvect(
        std::string const & variant,
        std::string const & name,
        std::vector<std::string> const & values
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_spread_status::Type ret = verify_spread_and_variant__and__get_variant_iter(name, variant, iter);
        if (ret != set_spread_status::success::grab())
            return ret;

        auto spread_iter = std::find_if(
            iter->spreads.begin(),
            iter->spreads.end(),
            [&](auto const & s) { return std::get<0>(s) == name; }
        );

        if (spread_iter != iter->spreads.end())
            std::get<2>(*spread_iter) = values;
        else
            iter->spreads.push_back(std::make_tuple(name, "", values));

        return ret;
    }


    set_spread_status::Type Matchable::verify_spread_and_variant__and__get_variant_iter(
        std::string const & spread,
        std::string const & variant,
        std::vector<MatchableVariant>::iterator & iter
    )
    {
        // verify spread exists (has been added by add_spread())
        {
            auto t_and_n_iter = std::find_if(
                spread_types_and_names.begin(),
                spread_types_and_names.end(),
                [&](auto const & t_and_n) { return t_and_n.second == spread; }
            );
            if (t_and_n_iter == spread_types_and_names.end())
                return set_spread_status::spread_lookup_failed::grab();
        }

        auto variant_iter = std::find(variants.begin(), variants.end(), variant);

        // verify add_variant() called for given variant
        if (variant_iter == variants.end())
            return set_spread_status::variant_lookup_failed::grab();

        // only change iter on success
        iter = variant_iter;
        return set_spread_status::success::grab();
    }



























    Matchable * MatchableMaker::grab(std::string const & m)
    {
        auto lb = matchables.lower_bound(m);
        if (lb != matchables.end() && !(matchables.key_comp()(m, lb->first)))
            return lb->second;

        Matchable * new_m = new Matchable;
        new_m->name = m;
        matchables.insert(lb, std::make_pair(m, new_m));
        return new_m;
    }


    void MatchableMaker::save_as(
        std::string const & filename,
        save_as__content::Flags const & content,
        save_as__grow_mode::Type mode
    )
    {
        if (content.currently_set().size() == 0)
            return;

        if (mode.is_nil())
            return;

        FILE * f = fopen(filename.c_str(), "w");
        fputs("#include <matchable/matchable.h>\n#include <matchable/matchable_fwd.h>\n\n\n\n", f);

        // generated_matchable
        if (content.is_set(save_as__content::generated_matchable::grab()))
        {
            mode.match({
                {save_as__grow_mode::wrap::grab(),
                    [&](){
                        fputs("MATCHABLE_FWD(generated_matchable)\n", f);
                        fputs("SPREADx1_MATCHABLE(generated_matchable::Type, dependency, "
                              "generated_matchable", f);
                    }},
                {save_as__grow_mode::always::grab(),
                    [&](){
                        fputs("MATCHABLE_GROW(generated_matchable", f);
                    }},
            });
            int matchable_count{0};
            for (auto const & [name, m] : matchables)
            {
                ++matchable_count;
                if (matchable_count % 17 == 0)
                {
                    fputs(")\nMATCHABLE_GROW(", f);
                    fputs(name.c_str(), f);
                }
                fputs(", ", f);
                fputs(name.c_str(), f);
            }
            fputs(")\n", f);
        }

        if (content.is_set(save_as__content::matchables::grab()))
        {
            for (auto const & [name, m] : matchables)
                fputs(print_matchable_fwd(*m).c_str(), f);

            // if just growing then the order does not matter,
            // so just go through them all and grow grow grow
            if (mode == save_as__grow_mode::always::grab())
            {
                for (auto const & [name, m] : matchables)
                {
                    std::string printed_matchable = print_matchable(*m, mode);
                    assert(printed_matchable.size());
                    fputs(printed_matchable.c_str(), f);
                }
            }
            // resolve so that dependency definitions occur first
            //   - this is an issue when matchables spread to matchables
            else
            {
                std::map<std::string, bool> processed;
                for (auto const & [name, m] : matchables)
                    processed.insert({name, false});

                int processed_count{0};

                while (processed_count < (int) processed.size())
                {
                    int const prev_processed_count{processed_count};

                    for (auto const & [name, m] : matchables)
                    {
                        // ready means all dependencies to other matchables within this maker have already
                        // been printed.
                        bool ready{true};
                        for (auto const & [s_type, s_name] : m->spread_types_and_names)
                        {
                            static std::string const matchable_type_ending{"::Type"};
                            if (s_type.rfind(matchable_type_ending) != s_type.npos)
                            {
                                std::string const s_type_without_ending{
                                    s_type.substr(0, s_type.size() - matchable_type_ending.size())
                                };

                                if (
                                    s_type_without_ending != name &&
                                    processed.find(s_type_without_ending) != processed.end() &&
                                    !processed[s_name]
                                )
                                {
                                    ready = false;
                                    break;
                                }
                            }
                        }
                        if (!ready)
                            continue;

                        std::string const printed_matchable{print_matchable(*m, mode)};
                        fputs(printed_matchable.c_str(), f);
                        processed[name] = true;
                        processed_count++;
                    }

                    if (prev_processed_count == processed_count)
                    {
                        std::cout << "WARNING! cyclic dependencies detected!" << std::endl;
                        for (auto const & [name, p] : processed)
                        {
                            if (!p)
                            {
                                std::cout << "    " << name << " : ";
                                for (auto const & [t, n] : matchables[name]->spread_types_and_names)
                                    std::cout << " " << n;
                                std::cout << std::endl;

                                std::string printed_matchable = print_matchable(*matchables[name], mode);
                                fputs(printed_matchable.c_str(), f);
                                processed[name] = true;
                            }
                        }
                        break;
                    }
                }
            }

            // initialize spreads
            for (auto const & [Matchable, m] : matchables)
                fputs(print_set_spread(*m).c_str(), f);
        }

        fclose(f);
    }


    std::string MatchableMaker::print_matchable_fwd(Matchable const & m)
    {
        std::string ret{"MATCHABLE_FWD("};
        ret += m.name;
        ret += ")\n";
        return ret;
    }


    std::string MatchableMaker::print_matchable(
        matchable::Matchable const & m,
        save_as__grow_mode::Type mode
    )
    {
        std::string ret;

        if (mode.is_nil())
            return ret;

        if (mode == save_as__grow_mode::wrap::grab())
        {
            if (m.spread_types_and_names.size() > 0)
            {
                ret += "SPREADx";
                ret += std::to_string(m.spread_types_and_names.size());
                ret += "_";
            }
            ret += "MATCHABLE(";
            for (auto const & [t, n] : m.spread_types_and_names)
            {
                ret += t;
                ret += ", ";
                ret += n;
                ret += ", ";
            }
        }
        else if (mode == save_as__grow_mode::always::grab())
        {
            ret += "MATCHABLE_GROW(";
        }
        else
        {
            // unhandled mode
            assert(107);
        }
        ret += m.name;
        int variant_count{0};
        for (auto const & v : m.variants)
        {
            ++variant_count;
            if (variant_count % 17 == 0)
            {
                ret += ")\nMATCHABLE_GROW(";
                ret += m.name;
            }

            ret += ", ";
            ret += v.variant_name;
        }
        ret += ")\n";

        return ret;
    }


    std::string MatchableMaker::print_set_spread(Matchable const & m)
    {
        std::string ret;
        for (auto const & variant : m.variants)
        {
            for (auto const & [s_name, s_value, s_values] : variant.spreads)
            {
                if (s_value.size() > 0)
                {
                    ret += "SET_SPREAD(";
                    ret += m.name;
                    ret += ", ";
                    ret += variant.variant_name;
                    ret += ", ";
                    ret += s_name;
                    ret += ", ";
                    ret += s_value;
                    ret += ")\n";
                }
                if (s_values.size() > 0)
                {
                    ret += "SET_SPREAD_VECT(";
                    ret += m.name;
                    ret += ", ";
                    ret += variant.variant_name;
                    ret += ", ";
                    ret += s_name;
                    for (auto const & v : s_values)
                    {
                        ret += ", ";
                        ret += v;
                    }
                    ret += ")\n";
                }
            }
        }

        return ret;
    }
}
