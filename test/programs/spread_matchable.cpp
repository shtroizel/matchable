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



#include <iostream>

#include "matchable.h"
#include "test_ok.h"



MATCHABLE(Floor, Carpet, Concrete, Linoleum, Parquet, Tile)
SPREAD_MATCHABLE(Floor, Room, Bathroom, Bedroom, Garage, Hallway, Kitchen, Lounge)



int main()
{
    test_ok ok;

    Room::Bathroom::grab().spread(Floor::Tile::grab());
    Room::Bedroom::grab().spread(Floor::Carpet::grab());
    Room::Garage::grab().spread(Floor::Concrete::grab());
    Room::Hallway::grab().spread(Floor::Linoleum::grab());
    Room::Kitchen::grab().spread(Floor::Tile::grab());
    Room::Lounge::grab().spread(Floor::Parquet::grab());

    for (auto room : Room::variants())
        std::cout << "Room::" << room << "::grab().as_Floor() --> " << room.as_Floor() << std::endl;

    TEST_EQ(ok, Room::Bathroom::grab().as_Floor(), Floor::Tile::grab());
    TEST_EQ(ok, Room::Bedroom::grab().as_Floor(), Floor::Carpet::grab());
    TEST_EQ(ok, Room::Garage::grab().as_Floor(), Floor::Concrete::grab());
    TEST_EQ(ok, Room::Hallway::grab().as_Floor(), Floor::Linoleum::grab());
    TEST_EQ(ok, Room::Kitchen::grab().as_Floor(), Floor::Tile::grab());
    TEST_EQ(ok, Room::Lounge::grab().as_Floor(), Floor::Parquet::grab());

    return ok();
}
