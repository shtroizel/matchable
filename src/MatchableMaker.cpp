/*
Copyright (c) 2019-2022, shtroizel
All rights reserved without prejudice.

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
    void Matchable::add_variant(std::string const & variant_name)
    {
        if (std::find(variants.begin(), variants.end(), variant_name) == variants.end())
        {
            MatchableVariant v;
            v.variant_name = variant_name;
            variants.push_back(v);
        }
    }


    void Matchable::del_variant(std::string const & variant_name)
    {
        auto variant_iter = std::find_if(
            variants.begin(),
            variants.end(),
            [&](auto const & s) { return s.variant_name == variant_name; }
        );

        if (variant_iter != variants.end())
            variants.erase(variant_iter);
    }


    void Matchable::variants_starting_with(std::string const & prefix, std::vector<std::string> & result)
    {
        result.clear();
        for (auto const & v : variants)
        {
            if (v.variant_name.size() < prefix.size())
                continue;

            if (prefix == v.variant_name.substr(0, prefix.size()))
                result.push_back(v.variant_name);
        }
    }


    bool Matchable::has_variant(std::string const & variant_name)
    {
        return std::find_if(
            variants.begin(),
            variants.end(),
            [&](auto const & s) { return s.variant_name == variant_name; }
        ) != variants.end();
    }


    bool Matchable::add_property(std::string const & property_type, std::string const & property_name)
    {
        for (auto const & [type, name] : property_types_and_names)
            if (name == property_name)
                return false;

        property_types_and_names.push_back(std::make_pair(property_type, property_name));
        return true;
    }


    set_property_status::Type Matchable::set_property(
        std::string const & variant_name,
        std::string const & property_name,
        std::string const & value
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_property_status::Type ret =
                verify_property_and_variant__and__get_variant_iter(property_name, variant_name, iter);
        if (ret != set_property_status::success::grab())
            return ret;

        auto property_iter = std::find_if(
            iter->properties.begin(),
            iter->properties.end(),
            [&](auto const & s) { return std::get<0>(s) == property_name; }
        );

        if (property_iter != iter->properties.end())
            std::get<1>(*property_iter) = value;
        else
            iter->properties.push_back(std::make_tuple(property_name, value, std::vector<std::string>{}));

        return ret;
    }


    set_propertyvect_status::Type Matchable::set_propertyvect(
        std::string const & variant_name,
        std::string const & property_name,
        std::vector<std::string> const & values
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_property_status::Type ret =
                verify_property_and_variant__and__get_variant_iter(property_name, variant_name, iter);
        if (ret != set_property_status::success::grab())
            return ret;

        auto property_iter = std::find_if(
            iter->properties.begin(),
            iter->properties.end(),
            [&](auto const & s) { return std::get<0>(s) == property_name; }
        );

        if (property_iter != iter->properties.end())
            std::get<2>(*property_iter) = values;
        else
            iter->properties.push_back(std::make_tuple(property_name, "", values));

        return ret;
    }


    void Matchable::get_property(
        std::string const & variant_name,
        std::string const & property_name,
        std::string & property_value
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_property_status::Type ret =
                verify_property_and_variant__and__get_variant_iter(property_name, variant_name, iter);
        if (ret != set_property_status::success::grab())
            return;

        auto property_iter = std::find_if(
            iter->properties.begin(),
            iter->properties.end(),
            [&](auto const & s) { return std::get<0>(s) == property_name; }
        );

        if (property_iter != iter->properties.end())
            property_value = std::get<1>(*property_iter);
    }



    void Matchable::get_propertyvect(
        std::string const & variant_name,
        std::string const & property_name,
        std::vector<std::string> & property_values
    )
    {
        std::vector<MatchableVariant>::iterator iter;
        set_property_status::Type ret =
                verify_property_and_variant__and__get_variant_iter(property_name, variant_name, iter);
        if (ret != set_property_status::success::grab())
        {
            std::cout << "verify failed with: " << ret << std::endl;
            return;
        }

        auto property_iter = std::find_if(
            iter->properties.begin(),
            iter->properties.end(),
            [&](auto const & s) { return std::get<0>(s) == property_name; }
        );

        if (property_iter != iter->properties.end())
            property_values = std::get<2>(*property_iter);
    }


    set_property_status::Type Matchable::verify_property_and_variant__and__get_variant_iter(
        std::string const & property,
        std::string const & variant,
        std::vector<MatchableVariant>::iterator & iter
    )
    {
        // verify property exists (has been added by add_property())
        {
            auto t_and_n_iter = std::find_if(
                property_types_and_names.begin(),
                property_types_and_names.end(),
                [&](auto const & t_and_n) { return t_and_n.second == property; }
            );
            if (t_and_n_iter == property_types_and_names.end())
                return set_property_status::property_lookup_failed::grab();
        }

        auto variant_iter = std::find(variants.begin(), variants.end(), variant);

        // verify add_variant() called for given variant
        if (variant_iter == variants.end())
            return set_property_status::variant_lookup_failed::grab();

        // only change iter on success
        iter = variant_iter;
        return set_property_status::success::grab();
    }



    MatchableMaker::MatchableMaker()
    {
    }


    MatchableMaker::~MatchableMaker()
    {
        for (auto & [n, m] : matchables)
            delete m;

        matchables.clear();
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


    save__status::Type MatchableMaker::save_as(
        std::string const & filename,
        save__content::Flags const & content,
        save__grow_mode::Type mode
    )
    {
        if (content.currently_set().size() == 0)
            return save__status::no_content::grab();

        if (mode.is_nil())
            return save__status::no_grow_mode::grab();

        FILE * f = fopen(filename.c_str(), "w");
        if (nullptr == f)
            return save__status::io_error::grab();

        save__status::Type ret{save__status::success::grab()};

        if (fputs("#pragma once\n\n\n#include <matchable/matchable.h>\n"
                  "#include <matchable/matchable_fwd.h>\n\n\n\n", f) == EOF)
            goto save_io_error;

        // if we need matchable 'generated_matchable' with variants for each matchable in the maker
        if (content.is_set(save__content::generated_matchable::grab()))
        {
            if (mode == save__grow_mode::wrap::grab())
            {
                if (fputs("MATCHABLE(generated_matchable", f) == EOF)
                    goto save_io_error;
            }
            else if (mode == save__grow_mode::always::grab())
            {
                if (fputs("GROW_MATCHABLE(generated_matchable", f) == EOF)
                    goto save_io_error;
            }
            else
            {
                ret = save__status::no_grow_mode::grab();
                goto save_cleanup;
            }
            int matchable_count{0};
            for (auto const & [name, m] : matchables)
            {
                ++matchable_count;
                if (matchable_count % 17 == 0)
                {
                    if (fputs(")\nGROW_MATCHABLE(", f) == EOF)
                        goto save_io_error;

                    if (fputs(name.c_str(), f) == EOF)
                        goto save_io_error;
                }
                if (fputs(", ", f) == EOF)
                    goto save_io_error;

                if (fputs(name.c_str(), f) == EOF)
                    goto save_io_error;
            }
            if (fputs(")\n", f) == EOF)
                goto save_io_error;
        }

        if (content.is_set(save__content::matchables::grab()))
        {
            // table recording whether each matchable has been written/saved (processed)
            std::map<std::string, bool> processed;
            for (auto const & [name, m] : matchables)
                processed.insert({name, false});

            // if just growing then the order does not matter,
            // so just go through them all and grow grow grow
            if (mode == save__grow_mode::always::grab())
            {
                for (auto const & [name, m] : matchables)
                {
                    std::string printed_matchable = print_matchable(*m, mode);
                    assert(printed_matchable.size());
                    processed[name] = true;
                    if (fputs(printed_matchable.c_str(), f) == EOF)
                        goto save_io_error;
                }
            }
            // not always growing means we provide definitions
            // initial definitions must be resolved so that dependency definitions occur first
            //   - this is an issue when matchables and their matchable properties are saved together
            else
            {
                // first forward declare all matchables resolving any dependency to self
                for (auto const & [name, m] : matchables)
                    if (fputs(print_matchable_fwd(*m).c_str(), f) == EOF)
                        goto save_io_error;

                // for each iteration of the following while(), processed_count must be incremented...
                // unchanged processed_count at end of iteration means we have cyclic dependencies!
                int processed_count{0};
                while (processed_count < (int) processed.size())
                {
                    int const prev_processed_count{processed_count};

                    for (auto const & [name, m] : matchables)
                    {
                        if (processed[name])
                            continue;

                        // ready means all dependencies to other matchables within this maker have already
                        // been printed.
                        bool ready{true};
                        for (auto const & [s_type, s_name] : m->property_types_and_names)
                        {
                            static std::string const matchable_type_ending{"::Type"};
                            if (s_type.size() > matchable_type_ending.size())
                            {
                                // get the matchable's 'namespace' name (if it is indeed a matchable at all)
                                std::string const s_type_without_ending{
                                    s_type.substr(0, s_type.size() - matchable_type_ending.size())
                                };

                                // if not self dependency (would be ok since we forward declared earlier) &&
                                // if this matchable dependency is also one of those to be saved &&
                                // if this matchable dependency has yet to be written then
                                //     we still have unprocessed dependencies
                                //     we must wait (stop looking at properties and skip to next matchable)
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
                        if (!ready) // dependencies exist that have yet to be written, skip and try later
                            continue;

                        // we are ready so write the matchable and record that we did so...
                        std::string const printed_matchable{print_matchable(*m, mode)};
                        if (fputs(printed_matchable.c_str(), f) == EOF)
                            goto save_io_error;

                        processed[name] = true;
                        processed_count++;
                    }

                    // if we went through all the matchables and none of them were ready
                    if (prev_processed_count == processed_count)
                    {
                        ret = save__status::cyclic_dependencies::grab();
                        std::cout << "WARNING! cyclic dependencies detected and omitted!" << std::endl;
                        for (auto const & [name, p] : processed)
                        {
                            if (!p)
                            {
                                std::cout << "    " << name << " : ";
                                for (auto const & [t, n] : matchables[name]->property_types_and_names)
                                    std::cout << " " << n;
                                std::cout << std::endl;
                            }
                        }
                        break;
                    }
                } // while matchables still need to be processed
            } // not always growing

            // initialize properties
            for (auto const & [name, m] : matchables)
                if (processed[name])
                    if (fputs(print_set_property(*m).c_str(), f) == EOF)
                        return save__status::io_error::grab();
        }

        goto save_cleanup;

save_io_error:
        ret = save__status::io_error::grab();

save_cleanup:
        fclose(f);
        return ret;
    }


    MATCHABLE(
        MatchableKeyword,
        esc_MATCHABLE_FWD,
        esc_MATCHABLE,
        esc_PROPERTYx1_MATCHABLE,
        esc_PROPERTYx2_MATCHABLE,
        esc_PROPERTYx3_MATCHABLE,
        esc_PROPERTYx4_MATCHABLE,
        esc_PROPERTYx5_MATCHABLE,
        esc_PROPERTYx6_MATCHABLE,
        esc_PROPERTYx7_MATCHABLE,
        esc_PROPERTYx8_MATCHABLE,
        esc_PROPERTYx9_MATCHABLE,
        esc_PROPERTYx10_MATCHABLE,
        esc_PROPERTYx11_MATCHABLE,
        esc_PROPERTYx12_MATCHABLE,
        esc_PROPERTYx13_MATCHABLE,
        esc_PROPERTYx14_MATCHABLE,
        esc_PROPERTYx15_MATCHABLE,
        esc_PROPERTYx16_MATCHABLE,
        esc_PROPERTYx17_MATCHABLE,
        esc_PROPERTYx18_MATCHABLE,
        esc_PROPERTYx19_MATCHABLE,
        esc_PROPERTYx20_MATCHABLE,
        esc_PROPERTYx21_MATCHABLE,
        esc_GROW_MATCHABLE,
        esc_MATCHABLE_VARIANT_PROPERTY_VALUE,
        esc_MATCHABLE_VARIANT_PROPERTY_VALUES
    );


    load__status::Type MatchableMaker::load(std::string const & filename)
    {
        FILE * f = fopen(filename.c_str(), "r");

        if (nullptr == f)
            return load__status::io_error::grab();

        load__status::Type ret{load__status::success::grab()};

        int ch;
        std::string str;
        MatchableKeyword::Type keyword;
        std::vector<std::string> args;
        int paren_balance{0};

        // loop over header file containing matchable keywords character by character
        while (true)
        {
            keyword = MatchableKeyword::nil;
            args.clear();

            // read next valid keyword
            while (keyword.is_nil())
            {
                str.clear();

                while (true)
                {
                    ch = fgetc(f);

                    if (ch == EOF)
                        goto load_end;

                    if (ch == '(' || ch == 10 || ch == 13)
                        break;

                    if (ch == ' ')
                        continue;

                    str += (char) ch;
                }

                if (ch == '(')
                    keyword = MatchableKeyword::from_string(str);
            }

            paren_balance = 0;

            // read keyword's arguments (populate "args")
            while (true)
            {
                str.clear();

                while (true)
                {
                    ch = fgetc(f);

                    if (ch == EOF)
                        goto load_end;

                    if (ch == '(')
                        --paren_balance;

                    if (ch == ',' || (ch == ')' && paren_balance == 0))
                        break;

                    if (ch == ')')
                        ++paren_balance;

                    if (ch == ' ')
                        continue;

                    str += (char) ch;
                }

                args.push_back(str);

                if (ch == ')')
                    break;
            }


            // perform loading...

            keyword.match({
                {MatchableKeyword::esc_MATCHABLE_FWD::grab(),
                    [&](){
                        if (args.size() != 1)
                        {
                            std::cout << "Error loading MATCHABLE_FWD(), expected 1 arg (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        grab(args[0]);
                    }},

                {MatchableKeyword::esc_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 1)
                        {
                            std::cout << "Error loading MATCHABLE(), expected >= 1 arg (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[0]);
                        for (int i = 1; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx1_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 3)
                        {
                            std::cout << "Error loading PROPERTYx1_MATCHABLE(), expected >= 3 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[2]);
                        m->add_property(args[0], args[1]);
                        for (int i = 3; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx2_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 5)
                        {
                            std::cout << "Error loading PROPERTYx2_MATCHABLE(), expected >= 5 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[4]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        for (int i = 5; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx3_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 7)
                        {
                            std::cout << "Error loading PROPERTYx3_MATCHABLE(), expected >= 7 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[6]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        for (int i = 7; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx4_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 9)
                        {
                            std::cout << "Error loading PROPERTYx4_MATCHABLE(), expected >= 9 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[8]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        for (int i = 9; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx5_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 11)
                        {
                            std::cout << "Error loading PROPERTYx5_MATCHABLE(), expected >= 11 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[10]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        for (int i = 11; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx6_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 13)
                        {
                            std::cout << "Error loading PROPERTYx6_MATCHABLE(), expected >= 13 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[12]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        for (int i = 13; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx7_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 15)
                        {
                            std::cout << "Error loading PROPERTYx7_MATCHABLE(), expected >= 15 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[14]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        for (int i = 15; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx8_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 17)
                        {
                            std::cout << "Error loading PROPERTYx8_MATCHABLE(), expected >= 17 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[16]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        for (int i = 17; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx9_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 19)
                        {
                            std::cout << "Error loading PROPERTYx9_MATCHABLE(), expected >= 19 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[18]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        for (int i = 19; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx10_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 21)
                        {
                            std::cout << "Error loading PROPERTYx10_MATCHABLE(), expected >= 21 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[20]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        for (int i = 21; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx10_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 21)
                        {
                            std::cout << "Error loading PROPERTYx10_MATCHABLE(), expected >= 21 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[20]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        for (int i = 21; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx11_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 23)
                        {
                            std::cout << "Error loading PROPERTYx11_MATCHABLE(), expected >= 23 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[22]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        for (int i = 23; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx12_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 25)
                        {
                            std::cout << "Error loading PROPERTYx12_MATCHABLE(), expected >= 25 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[24]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        for (int i = 25; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx13_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 27)
                        {
                            std::cout << "Error loading PROPERTYx13_MATCHABLE(), expected >= 27 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[26]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        for (int i = 27; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx14_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 29)
                        {
                            std::cout << "Error loading PROPERTYx14_MATCHABLE(), expected >= 29 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[28]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        for (int i = 29; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx15_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 31)
                        {
                            std::cout << "Error loading PROPERTYx15_MATCHABLE(), expected >= 31 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[30]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        for (int i = 31; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx16_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 33)
                        {
                            std::cout << "Error loading PROPERTYx16_MATCHABLE(), expected >= 33 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[32]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        for (int i = 33; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx17_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 35)
                        {
                            std::cout << "Error loading PROPERTYx17_MATCHABLE(), expected >= 35 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[34]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        m->add_property(args[32], args[33]);
                        for (int i = 35; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx18_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 37)
                        {
                            std::cout << "Error loading PROPERTYx18_MATCHABLE(), expected >= 37 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[36]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        m->add_property(args[32], args[33]);
                        m->add_property(args[34], args[35]);
                        for (int i = 37; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx19_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 39)
                        {
                            std::cout << "Error loading PROPERTYx19_MATCHABLE(), expected >= 39 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[38]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        m->add_property(args[32], args[33]);
                        m->add_property(args[34], args[35]);
                        m->add_property(args[36], args[37]);
                        for (int i = 39; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx20_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 41)
                        {
                            std::cout << "Error loading PROPERTYx20_MATCHABLE(), expected >= 41 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[40]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        m->add_property(args[32], args[33]);
                        m->add_property(args[34], args[35]);
                        m->add_property(args[36], args[37]);
                        m->add_property(args[38], args[39]);
                        for (int i = 41; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_PROPERTYx21_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 43)
                        {
                            std::cout << "Error loading PROPERTYx21_MATCHABLE(), expected >= 21 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[42]);
                        m->add_property(args[0], args[1]);
                        m->add_property(args[2], args[3]);
                        m->add_property(args[4], args[5]);
                        m->add_property(args[6], args[7]);
                        m->add_property(args[8], args[9]);
                        m->add_property(args[10], args[11]);
                        m->add_property(args[12], args[13]);
                        m->add_property(args[14], args[15]);
                        m->add_property(args[16], args[17]);
                        m->add_property(args[18], args[19]);
                        m->add_property(args[20], args[21]);
                        m->add_property(args[22], args[23]);
                        m->add_property(args[24], args[25]);
                        m->add_property(args[26], args[27]);
                        m->add_property(args[28], args[29]);
                        m->add_property(args[30], args[31]);
                        m->add_property(args[32], args[33]);
                        m->add_property(args[34], args[35]);
                        m->add_property(args[36], args[37]);
                        m->add_property(args[38], args[39]);
                        m->add_property(args[40], args[41]);
                        for (int i = 43; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_GROW_MATCHABLE::grab(),
                    [&](){
                        if (args.size() < 1)
                        {
                            std::cout << "Error loading GROW_MATCHABLE(), expected >= 1 arg (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[0]);
                        for (int i = 1; i < (int) args.size(); ++i)
                            m->add_variant(args[i]);
                    }},

                {MatchableKeyword::esc_MATCHABLE_VARIANT_PROPERTY_VALUE::grab(),
                    [&](){
                        if (args.size() != 4)
                        {
                            std::cout << "Error loading SET_PROPERTY(), expected 4 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[0]);
                        m->set_property(args[1], args[2], args[3]);
                    }},

                {MatchableKeyword::esc_MATCHABLE_VARIANT_PROPERTY_VALUES::grab(),
                    [&](){
                        if (args.size() < 3)
                        {
                            std::cout << "Error loading SET_PROPERTY_VECT(), expected >= 3 args (got "
                                      << args.size() << ")" << std::endl;
                            ret = load__status::syntax_error::grab();
                            return;
                        }
                        auto m = grab(args[0]);
                        std::vector<std::string> property_values;
                        for (int i = 3; i < (int) args.size(); ++i)
                            property_values.push_back(args[i]);
                        m->set_propertyvect(args[1], args[2], property_values);
                    }},
            });
        }

load_end:
        fclose(f);
        return ret;
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
        save__grow_mode::Type mode
    )
    {
        std::string ret;

        if (mode.is_nil())
            return ret;

        if (mode == save__grow_mode::wrap::grab())
        {
            if (m.property_types_and_names.size() > 0)
            {
                ret += "PROPERTYx";
                ret += std::to_string(m.property_types_and_names.size());
                ret += "_";
            }
            ret += "MATCHABLE(";
            for (auto const & [t, n] : m.property_types_and_names)
            {
                ret += t;
                ret += ", ";
                ret += n;
                ret += ", ";
            }
        }
        else if (mode == save__grow_mode::always::grab())
        {
            ret += "GROW_MATCHABLE(";
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
                ret += ")\nGROW_MATCHABLE(";
                ret += m.name;
            }

            ret += ", ";
            ret += v.variant_name;
        }
        ret += ")\n";

        return ret;
    }


    std::string MatchableMaker::print_set_property(Matchable const & m)
    {
        std::string ret;
        for (auto const & variant : m.variants)
        {
            for (auto const & [s_name, s_value, s_values] : variant.properties)
            {
                if (s_value.size() > 0)
                {
                    ret += "MATCHABLE_VARIANT_PROPERTY_VALUE(";
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
                    ret += "MATCHABLE_VARIANT_PROPERTY_VALUES(";
                    ret += m.name;
                    ret += ", ";
                    ret += variant.variant_name;
                    ret += ", ";
                    ret += s_name;
                    int element_count{0};
                    for (auto const & v : s_values)
                    {
                        ++element_count;
                        if (element_count % 17 == 0)
                            ret += ",\n                  ";
                        else
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
