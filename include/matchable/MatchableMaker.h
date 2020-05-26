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
    MATCHABLE(save_as__status, success, no_content, no_spread_mode, io_error, cyclic_dependencies);
    MATCHABLE(save_as__content, generated_matchable, matchables);
    MATCHABLE(save_as__spread_mode, wrap, always);
    MATCHABLE(set_property_status, success, variant_lookup_failed, property_lookup_failed);
    namespace set_propertyvect_status = set_property_status;

    struct MatchableVariant
    {
        std::string variant_name;
        //                     property name, property value, property vector value
        std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> properties;
        bool operator==(std::string const & other) const { return variant_name == other; }
    };

    class Matchable
    {
        friend class MatchableMaker;
    public:
        void add_variant(std::string const & variant_name);
        void del_variant(std::string const & variant_name);
        void variants_starting_with(std::string const & prefix, std::vector<std::string> & result);
        bool has_variant(std::string const & variant_name);
        bool add_property(std::string const & property_type, std::string const & property_name);
        set_property_status::Type set_property(
            std::string const & variant,
            std::string const & property_name,
            std::string const & property_value
        );
        set_propertyvect_status::Type set_propertyvect(
            std::string const & variant,
            std::string const & property_name,
            std::vector<std::string> const & property_values
        );
        void get_property(
            std::string const & variant_name,
            std::string const & property_name,
            std::string & property_value
        );
        void get_propertyvect(
            std::string const & variant_name,
            std::string const & property_name,
            std::vector<std::string> & property_values
        );
    private:
        set_property_status::Type verify_property_and_variant__and__get_variant_iter(
            std::string const & property,
            std::string const & variant,
            std::vector<MatchableVariant>::iterator & iter
        );
        std::string name;
        std::vector<MatchableVariant> variants;
        std::vector<std::pair<std::string, std::string>> property_types_and_names;
    };

    class MatchableMaker
    {
    public:
        ~MatchableMaker();
        Matchable * grab(std::string const & m);
        save_as__status::Type save_as(
            std::string const & filename,
            save_as__content::Flags const & content,
            save_as__spread_mode::Type mode
        );
        std::map<std::string, Matchable *> matchables;
    private:
        std::string print_matchable_fwd(Matchable const & m);
        std::string print_matchable(Matchable const & m, save_as__spread_mode::Type mode);
        std::string print_set_property(Matchable const & m);
    };
}
