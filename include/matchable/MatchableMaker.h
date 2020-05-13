#pragma once


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


#include <map>
#include <string>
#include <matchable/matchable.h>


namespace matchable
{
    MATCHABLE(save_as__status, success, no_content, no_grow_mode, io_error, cyclic_dependencies);
    MATCHABLE(save_as__content, generated_matchable, matchables);
    MATCHABLE(save_as__grow_mode, wrap, always);
    MATCHABLE(set_spread_status, success, variant_lookup_failed, spread_lookup_failed);
    namespace set_spreadvect_status = set_spread_status;

    struct MatchableVariant
    {
        std::string variant_name;

        //                     spread name, spread value, spread vector value
        std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> spreads;

        bool operator==(std::string const & other) const { return variant_name == other; }
    };


    class Matchable
    {
        friend class MatchableMaker;
    public:
        void add_variant(std::string const & name);
        bool add_spread(std::string const & spread_type, std::string const & spread_name);
        set_spread_status::Type set_spread(
            std::string const & variant,
            std::string const & spread_name,
            std::string const & spread_value
        );
        set_spreadvect_status::Type set_spreadvect(
            std::string const & variant,
            std::string const & spread_name,
            std::vector<std::string> const & spread_values
        );

    private:
        set_spread_status::Type verify_spread_and_variant__and__get_variant_iter(
            std::string const & spread,
            std::string const & variant,
            std::vector<MatchableVariant>::iterator & iter
        );

        std::string name;
        std::vector<MatchableVariant> variants;
        std::vector<std::pair<std::string, std::string>> spread_types_and_names;
    };


    class MatchableMaker
    {
    public:
        ~MatchableMaker();
        Matchable * grab(std::string const & m);
        save_as__status::Type save_as(
            std::string const & filename,
            save_as__content::Flags const & content,
            save_as__grow_mode::Type mode
        );

    private:
        std::string print_matchable_fwd(Matchable const & m);
        std::string print_matchable(Matchable const & m, save_as__grow_mode::Type mode);
        std::string print_set_spread(Matchable const & m);

        std::map<std::string, Matchable *> matchables;
    };
}
