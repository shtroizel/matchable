/*
Copyright (c) 2020, Eric Hyer
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



#include <cstdio>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, Magenta, Cyan, Yellow)
PROPERTYx1_MATCHABLE(Color::Type, color, GlobalSetting, FeatureX)
SET_PROPERTY(GlobalSetting, FeatureX, color, Color::Yellow::grab())



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
