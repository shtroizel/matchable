// compiles the header generate.cpp saved during the build and checks the order its sort key gives
#include <string>
#include <vector>

#include "matchable_maker_generated.h"
#include "test_ok.h"



template<typename T>
std::vector<std::string> names_by_string()
{
    std::vector<std::string> names;
    for (auto const & v : T::variants_by_string())
        names.push_back(v.as_string());
    return names;
}



int main()
{
    test_ok ok;

    std::vector<std::string> const keyed{
        "APPLE", "Apple", "apple", "b", "Banana", "banana", "Cherry", "cherry", "D", "d", "E", "e",
        "F", "f", "G", "g", "H", "h", "Zebra", "zebra"
    };
    TEST_EQ(ok, names_by_string<Word::Type>() == keyed, true);
    for (int i = 0; i < (int) keyed.size(); ++i)
        TEST_EQ(ok, Word::from_string(keyed[i]).as_by_string_index(), i);

    std::vector<std::string> const plain{"Apple", "Banana", "apple", "banana"};
    TEST_EQ(ok, names_by_string<Plain::Type>() == plain, true);

    return ok();
}
