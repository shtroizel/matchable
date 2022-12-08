#pragma once

#include <map>
#include <string>
#include <matchable/matchable.h>



namespace matchable
{
    MATCHABLE(save__status, success, no_content, no_grow_mode, io_error, cyclic_dependencies);
    MATCHABLE(save__content, generated_matchable, matchables);
    MATCHABLE(save__grow_mode, wrap, always);
    MATCHABLE(load__status, success, io_error, syntax_error);
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
        std::string name;
        std::vector<MatchableVariant> variants;
        std::vector<std::pair<std::string, std::string>> property_types_and_names;
    private:
        set_property_status::Type verify_property_and_variant__and__get_variant_iter(
            std::string const & property,
            std::string const & variant,
            std::vector<MatchableVariant>::iterator & iter
        );
    };

    class MatchableMaker
    {
    public:
        MatchableMaker();
        ~MatchableMaker();
        Matchable * grab(std::string const & m);
        save__status::Type save_as(
            std::string const & filename,
            save__content::Flags const & content,
            save__grow_mode::Type mode
        );
        load__status::Type load(std::string const & filename);
        std::map<std::string, Matchable *> matchables;
    private:
        std::string print_matchable_fwd(Matchable const & m);
        std::string print_matchable(Matchable const & m, save__grow_mode::Type mode);
        std::string print_set_property(Matchable const & m);
    };
}
