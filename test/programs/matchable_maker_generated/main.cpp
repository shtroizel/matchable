// compiles the header generate.cpp saved during the build and checks the order its sort key gives,
// and that its references reach the planets, whose instances another source file defines
#include <string>
#include <vector>

#include "matchable_maker_generated.h"
#include "planet.h"
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

    TEST_EQ(ok, Planet::from_variant(Alien::zog::grab().as_home()), Planet::mars::grab());
    std::vector<::matchable::VariantRef> const & visited = Alien::zog::grab().as_visited_vect();
    TEST_EQ(ok, (int) visited.size(), 2);
    if (visited.size() == 2)
    {
        TEST_EQ(ok, visited[0]->as_string(), std::string("venus"));
        TEST_EQ(ok, visited[1] == Alien::zog::grab().as_home(), true);
    }

    return ok();
}
