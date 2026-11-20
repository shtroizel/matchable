// growth units loaded and unloaded at run time: each unit's variants appear while it is loaded and are gone
// after, variants the program grew itself stay, and storage a unit built first outlives the unit
#include <dlfcn.h>
#include <unistd.h>

#include <climits>
#include <iostream>
#include <string>

#include "grown.h"
#include "test_ok.h"



GROW_MATCHABLE(Grown, kilo)
MATCHABLE_VARIANT_PROPERTY_VALUE(Grown, kilo, id, 11)



namespace
{
    // the units sit next to this program in the build tree and in ../modules once installed
    std::string unit_path(int version)
    {
        char exe[PATH_MAX];
        ssize_t const len = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
        std::string dir = len > 0 ? std::string(exe, len) : std::string("./x");
        dir.erase(dir.rfind('/') + 1);
        std::string const name = "libgrow_unload_unit_" + std::to_string(version) + ".so";
        if (access((dir + name).c_str(), R_OK) == 0)
            return dir + name;
        return dir + "../modules/" + name;
    }

    void * load(test_ok & ok, int version)
    {
        void * unit = dlopen(unit_path(version).c_str(), RTLD_NOW | RTLD_LOCAL);
        if (nullptr == unit)
            ok.test_fail(std::string("dlopen: ") + dlerror());
        return unit;
    }

    bool unload(void * unit, int version)
    {
        if (nullptr != unit)
            dlclose(unit);
        return nullptr == dlopen(unit_path(version).c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
    }

    std::string names()
    {
        std::string s;
        for (auto const & v : Grown::variants_by_string())
            s += v.as_string() + " ";
        return s;
    }

    // every variant's by-string index is its position, and the order is by string
    bool consistent()
    {
        auto const & vs = Grown::variants_by_string();
        for (int i = 0; i < (int) vs.size(); ++i)
        {
            if (vs[i].as_by_string_index() != i)
                return false;
            if (i > 0 && !vs[i - 1].lt_by_string(vs[i]))
                return false;
        }
        for (auto const & v : Grown::variants_by_string())
            if (Grown::from_string(v.as_string()) != v)
                return false;
        return true;
    }

    void base_intact(test_ok & ok)
    {
        TEST_EQ(ok, Grown::from_string("base_a"), Grown::base_a::grab());
        TEST_EQ(ok, Grown::from_string("base_b"), Grown::base_b::grab());
        TEST_EQ(ok, Grown::from_string("kilo"), Grown::kilo::grab());
        TEST_EQ(ok, Grown::kilo::grab().as_id(), 11);
#ifndef MATCHABLE_OMIT_BY_INDEX
        for (auto const & v : {Grown::base_a::grab(), Grown::base_b::grab(), Grown::kilo::grab()})
            TEST_EQ(ok, Grown::from_index(v.as_index()), v);
#endif
    }
}



int main()
{
    test_ok ok;

    TEST_EQ(ok, names(), std::string("base_a base_b kilo "));

    // load: the unit's variants join the program's, found by string with their values
    void * unit = load(ok, 1);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo mike zulu "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, Grown::from_string("mike").as_id(), 13);
    TEST_EQ(ok, Grown::from_string("zulu").as_id(), 26);
    TEST_EQ(ok, Grown::base_a::grab().as_extra(), 7);
    base_intact(ok);

    // unload: only the unit's variants go; kilo, which the unit regrew, stays the program's
    TEST_EQ(ok, unload(unit, 1), true);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, Grown::from_string("mike").is_nil(), true);
    TEST_EQ(ok, Grown::from_string("zulu").is_nil(), true);
    base_intact(ok);

    // storage the unit built first (extra) still works after the unit is gone
    TEST_EQ(ok, Grown::base_a::grab().as_extra(), 7);
    Grown::base_b::grab().set_extra(8);
    TEST_EQ(ok, Grown::base_b::grab().as_extra(), 8);

    // swap: remove then add another version
    unit = load(ok, 2);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo yankee zulu "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, Grown::from_string("zulu").as_id(), 52);
    TEST_EQ(ok, Grown::from_string("yankee").as_id(), 25);
    base_intact(ok);
    TEST_EQ(ok, unload(unit, 2), true);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo "));

    // load again
    unit = load(ok, 1);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo mike zulu "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, Grown::from_string("zulu").as_id(), 26);
    base_intact(ok);

    // a second unit growing a name already registered does not register it again: lookups keep the first
    void * unit_2 = load(ok, 2);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo mike yankee zulu "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, Grown::from_string("zulu").as_id(), 26);
    TEST_EQ(ok, unload(unit, 1), true);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo yankee "));
    TEST_EQ(ok, consistent(), true);
    TEST_EQ(ok, unload(unit_2, 2), true);
    TEST_EQ(ok, names(), std::string("base_a base_b kilo "));
    TEST_EQ(ok, consistent(), true);
    base_intact(ok);

    return ok();
}
