#include <cstdio>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, Magenta, Cyan, Yellow)
PROPERTYx1_MATCHABLE(Color::Type, color, GlobalSetting, FeatureX)
MATCHABLE_VARIANT_PROPERTY_VALUE(GlobalSetting, FeatureX, color, Color::Yellow::grab())



int main()
{
    test_ok ok;

    // single property (non-nil setting)
    {
        bool calledA = false;
        bool calledB = false;
        bool calledC = false;
        auto fx = GlobalSetting::FeatureX::grab();
        fx.add_color_observer([&](){ printf("single property observed by A\n"); calledA = true; });
        fx.add_color_observer([&](){ printf("single property observed by B\n"); calledB = true; });
        fx.add_color_observer([&](){ printf("single property observed by C\n"); calledC = true; });
        fx.set_color(Color::Magenta::grab());
        TEST_EQ(ok, calledA, true);
        TEST_EQ(ok, calledB, true);
        TEST_EQ(ok, calledC, true);
    }

    // single property (nil setting)
    {
        bool calledA = false;
        bool calledB = false;
        bool calledC = false;
        GlobalSetting::Type nil_setting;
        nil_setting.add_color_observer([&](){ printf("single prop for nil: A\n"); calledA = true; });
        nil_setting.add_color_observer([&](){ printf("single prop for nil: B\n"); calledB = true; });
        nil_setting.add_color_observer([&](){ printf("single prop for nil: C\n"); calledC = true; });
        nil_setting.set_color(Color::Magenta::grab());
        TEST_EQ(ok, calledA, true);
        TEST_EQ(ok, calledB, true);
        TEST_EQ(ok, calledC, true);
    }

    // vector property (non-nil setting)
    {
        bool calledA = false;
        bool calledB = false;
        bool calledC = false;
        auto fx = GlobalSetting::FeatureX::grab();
        fx.add_color_vect_observer([&](){ printf("vector property observed by A\n"); calledA = true; });
        fx.add_color_vect_observer([&](){ printf("vector property observed by B\n"); calledB = true; });
        fx.add_color_vect_observer([&](){ printf("vector property observed by C\n"); calledC = true; });
        fx.set_color_vect({Color::Magenta::grab(), Color::Cyan::grab()});
        TEST_EQ(ok, calledA, true);
        TEST_EQ(ok, calledB, true);
        TEST_EQ(ok, calledC, true);
    }

    // vector property (nil setting)
    {
        bool calledA = false;
        bool calledB = false;
        bool calledC = false;
        GlobalSetting::Type nil_setting;
        nil_setting.add_color_vect_observer([&](){ printf("vector prop for nil: A\n"); calledA = true; });
        nil_setting.add_color_vect_observer([&](){ printf("vector prop for nil: B\n"); calledB = true; });
        nil_setting.add_color_vect_observer([&](){ printf("vector prop for nil: C\n"); calledC = true; });
        nil_setting.set_color_vect({Color::Magenta::grab(), Color::Cyan::grab()});
        TEST_EQ(ok, calledA, true);
        TEST_EQ(ok, calledB, true);
        TEST_EQ(ok, calledC, true);
    }

    return ok();
}
