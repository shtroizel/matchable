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

#include "matchable/matchable.h"
#include "test_ok.h"



PROPERTYx1_MATCHABLE(
    int, p1,
    matchable_1, variant
);
PROPERTYx2_MATCHABLE(
    int, p1,
    int, p2,
    matchable_2, variant
);
PROPERTYx3_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    matchable_3, variant
);
PROPERTYx4_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    matchable_4, variant
);
PROPERTYx5_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    matchable_5, variant
);
PROPERTYx6_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    matchable_6, variant
);
PROPERTYx7_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    matchable_7, variant
);
PROPERTYx8_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    matchable_8, variant
);
PROPERTYx9_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    matchable_9, variant
);
PROPERTYx10_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    matchable_10, variant
);
PROPERTYx11_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    matchable_11, variant
);
PROPERTYx12_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    matchable_12, variant
);
PROPERTYx13_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    int, p13,
    matchable_13, variant
);
PROPERTYx14_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    int, p13,
    int, p14,
    matchable_14, variant
);
PROPERTYx15_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    int, p13,
    int, p14,
    int, p15,
    matchable_15, variant
);
PROPERTYx16_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    int, p13,
    int, p14,
    int, p15,
    int, p16,
    matchable_16, variant
);
PROPERTYx17_MATCHABLE(
    int, p1,
    int, p2,
    int, p3,
    int, p4,
    int, p5,
    int, p6,
    int, p7,
    int, p8,
    int, p9,
    int, p10,
    int, p11,
    int, p12,
    int, p13,
    int, p14,
    int, p15,
    int, p16,
    int, p17,
    matchable_17, variant
);


int main()
{
    test_ok ok;

    matchable_1::variant::grab().set_p1(1);
    TEST_EQ(ok, matchable_1::variant::grab().as_p1(), 1);

    matchable_2::variant::grab().set_p1(1);
    matchable_2::variant::grab().set_p2(2);
    TEST_EQ(ok, matchable_2::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_2::variant::grab().as_p2(), 2);

    matchable_3::variant::grab().set_p1(1);
    matchable_3::variant::grab().set_p2(2);
    matchable_3::variant::grab().set_p3(3);
    TEST_EQ(ok, matchable_3::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_3::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_3::variant::grab().as_p3(), 3);

    matchable_4::variant::grab().set_p1(1);
    matchable_4::variant::grab().set_p2(2);
    matchable_4::variant::grab().set_p3(3);
    matchable_4::variant::grab().set_p4(4);
    TEST_EQ(ok, matchable_4::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_4::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_4::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_4::variant::grab().as_p4(), 4);

    matchable_5::variant::grab().set_p1(1);
    matchable_5::variant::grab().set_p2(2);
    matchable_5::variant::grab().set_p3(3);
    matchable_5::variant::grab().set_p4(4);
    matchable_5::variant::grab().set_p5(5);
    TEST_EQ(ok, matchable_5::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_5::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_5::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_5::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_5::variant::grab().as_p5(), 5);

    matchable_6::variant::grab().set_p1(1);
    matchable_6::variant::grab().set_p2(2);
    matchable_6::variant::grab().set_p3(3);
    matchable_6::variant::grab().set_p4(4);
    matchable_6::variant::grab().set_p5(5);
    matchable_6::variant::grab().set_p6(6);
    TEST_EQ(ok, matchable_6::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_6::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_6::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_6::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_6::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_6::variant::grab().as_p6(), 6);

    matchable_7::variant::grab().set_p1(1);
    matchable_7::variant::grab().set_p2(2);
    matchable_7::variant::grab().set_p3(3);
    matchable_7::variant::grab().set_p4(4);
    matchable_7::variant::grab().set_p5(5);
    matchable_7::variant::grab().set_p6(6);
    matchable_7::variant::grab().set_p7(7);
    TEST_EQ(ok, matchable_7::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_7::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_7::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_7::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_7::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_7::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_7::variant::grab().as_p7(), 7);

    matchable_8::variant::grab().set_p1(1);
    matchable_8::variant::grab().set_p2(2);
    matchable_8::variant::grab().set_p3(3);
    matchable_8::variant::grab().set_p4(4);
    matchable_8::variant::grab().set_p5(5);
    matchable_8::variant::grab().set_p6(6);
    matchable_8::variant::grab().set_p7(7);
    matchable_8::variant::grab().set_p8(8);
    TEST_EQ(ok, matchable_8::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_8::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_8::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_8::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_8::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_8::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_8::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_8::variant::grab().as_p8(), 8);

    matchable_9::variant::grab().set_p1(1);
    matchable_9::variant::grab().set_p2(2);
    matchable_9::variant::grab().set_p3(3);
    matchable_9::variant::grab().set_p4(4);
    matchable_9::variant::grab().set_p5(5);
    matchable_9::variant::grab().set_p6(6);
    matchable_9::variant::grab().set_p7(7);
    matchable_9::variant::grab().set_p8(8);
    matchable_9::variant::grab().set_p9(9);
    TEST_EQ(ok, matchable_9::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_9::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_9::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_9::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_9::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_9::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_9::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_9::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_9::variant::grab().as_p9(), 9);

    matchable_10::variant::grab().set_p1(1);
    matchable_10::variant::grab().set_p2(2);
    matchable_10::variant::grab().set_p3(3);
    matchable_10::variant::grab().set_p4(4);
    matchable_10::variant::grab().set_p5(5);
    matchable_10::variant::grab().set_p6(6);
    matchable_10::variant::grab().set_p7(7);
    matchable_10::variant::grab().set_p8(8);
    matchable_10::variant::grab().set_p9(9);
    matchable_10::variant::grab().set_p10(10);
    TEST_EQ(ok, matchable_10::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_10::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_10::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_10::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_10::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_10::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_10::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_10::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_10::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_10::variant::grab().as_p10(), 10);

    matchable_11::variant::grab().set_p1(1);
    matchable_11::variant::grab().set_p2(2);
    matchable_11::variant::grab().set_p3(3);
    matchable_11::variant::grab().set_p4(4);
    matchable_11::variant::grab().set_p5(5);
    matchable_11::variant::grab().set_p6(6);
    matchable_11::variant::grab().set_p7(7);
    matchable_11::variant::grab().set_p8(8);
    matchable_11::variant::grab().set_p9(9);
    matchable_11::variant::grab().set_p10(10);
    matchable_11::variant::grab().set_p11(11);
    TEST_EQ(ok, matchable_11::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_11::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_11::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_11::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_11::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_11::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_11::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_11::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_11::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_11::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_11::variant::grab().as_p11(), 11);

    matchable_12::variant::grab().set_p1(1);
    matchable_12::variant::grab().set_p2(2);
    matchable_12::variant::grab().set_p3(3);
    matchable_12::variant::grab().set_p4(4);
    matchable_12::variant::grab().set_p5(5);
    matchable_12::variant::grab().set_p6(6);
    matchable_12::variant::grab().set_p7(7);
    matchable_12::variant::grab().set_p8(8);
    matchable_12::variant::grab().set_p9(9);
    matchable_12::variant::grab().set_p10(10);
    matchable_12::variant::grab().set_p11(11);
    matchable_12::variant::grab().set_p12(12);
    TEST_EQ(ok, matchable_12::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_12::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_12::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_12::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_12::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_12::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_12::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_12::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_12::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_12::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_12::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_12::variant::grab().as_p12(), 12);

    matchable_13::variant::grab().set_p1(1);
    matchable_13::variant::grab().set_p2(2);
    matchable_13::variant::grab().set_p3(3);
    matchable_13::variant::grab().set_p4(4);
    matchable_13::variant::grab().set_p5(5);
    matchable_13::variant::grab().set_p6(6);
    matchable_13::variant::grab().set_p7(7);
    matchable_13::variant::grab().set_p8(8);
    matchable_13::variant::grab().set_p9(9);
    matchable_13::variant::grab().set_p10(10);
    matchable_13::variant::grab().set_p11(11);
    matchable_13::variant::grab().set_p12(12);
    matchable_13::variant::grab().set_p13(13);
    TEST_EQ(ok, matchable_13::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_13::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_13::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_13::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_13::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_13::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_13::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_13::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_13::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_13::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_13::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_13::variant::grab().as_p12(), 12);
    TEST_EQ(ok, matchable_13::variant::grab().as_p13(), 13);

    matchable_14::variant::grab().set_p1(1);
    matchable_14::variant::grab().set_p2(2);
    matchable_14::variant::grab().set_p3(3);
    matchable_14::variant::grab().set_p4(4);
    matchable_14::variant::grab().set_p5(5);
    matchable_14::variant::grab().set_p6(6);
    matchable_14::variant::grab().set_p7(7);
    matchable_14::variant::grab().set_p8(8);
    matchable_14::variant::grab().set_p9(9);
    matchable_14::variant::grab().set_p10(10);
    matchable_14::variant::grab().set_p11(11);
    matchable_14::variant::grab().set_p12(12);
    matchable_14::variant::grab().set_p13(13);
    matchable_14::variant::grab().set_p14(14);
    TEST_EQ(ok, matchable_14::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_14::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_14::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_14::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_14::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_14::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_14::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_14::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_14::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_14::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_14::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_14::variant::grab().as_p12(), 12);
    TEST_EQ(ok, matchable_14::variant::grab().as_p13(), 13);
    TEST_EQ(ok, matchable_14::variant::grab().as_p14(), 14);

    matchable_15::variant::grab().set_p1(1);
    matchable_15::variant::grab().set_p2(2);
    matchable_15::variant::grab().set_p3(3);
    matchable_15::variant::grab().set_p4(4);
    matchable_15::variant::grab().set_p5(5);
    matchable_15::variant::grab().set_p6(6);
    matchable_15::variant::grab().set_p7(7);
    matchable_15::variant::grab().set_p8(8);
    matchable_15::variant::grab().set_p9(9);
    matchable_15::variant::grab().set_p10(10);
    matchable_15::variant::grab().set_p11(11);
    matchable_15::variant::grab().set_p12(12);
    matchable_15::variant::grab().set_p13(13);
    matchable_15::variant::grab().set_p14(14);
    matchable_15::variant::grab().set_p15(15);
    TEST_EQ(ok, matchable_15::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_15::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_15::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_15::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_15::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_15::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_15::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_15::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_15::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_15::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_15::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_15::variant::grab().as_p12(), 12);
    TEST_EQ(ok, matchable_15::variant::grab().as_p13(), 13);
    TEST_EQ(ok, matchable_15::variant::grab().as_p14(), 14);
    TEST_EQ(ok, matchable_15::variant::grab().as_p15(), 15);

    matchable_16::variant::grab().set_p1(1);
    matchable_16::variant::grab().set_p2(2);
    matchable_16::variant::grab().set_p3(3);
    matchable_16::variant::grab().set_p4(4);
    matchable_16::variant::grab().set_p5(5);
    matchable_16::variant::grab().set_p6(6);
    matchable_16::variant::grab().set_p7(7);
    matchable_16::variant::grab().set_p8(8);
    matchable_16::variant::grab().set_p9(9);
    matchable_16::variant::grab().set_p10(10);
    matchable_16::variant::grab().set_p11(11);
    matchable_16::variant::grab().set_p12(12);
    matchable_16::variant::grab().set_p13(13);
    matchable_16::variant::grab().set_p14(14);
    matchable_16::variant::grab().set_p15(15);
    matchable_16::variant::grab().set_p16(16);
    TEST_EQ(ok, matchable_16::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_16::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_16::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_16::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_16::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_16::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_16::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_16::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_16::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_16::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_16::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_16::variant::grab().as_p12(), 12);
    TEST_EQ(ok, matchable_16::variant::grab().as_p13(), 13);
    TEST_EQ(ok, matchable_16::variant::grab().as_p14(), 14);
    TEST_EQ(ok, matchable_16::variant::grab().as_p15(), 15);
    TEST_EQ(ok, matchable_16::variant::grab().as_p16(), 16);

    matchable_17::variant::grab().set_p1(1);
    matchable_17::variant::grab().set_p2(2);
    matchable_17::variant::grab().set_p3(3);
    matchable_17::variant::grab().set_p4(4);
    matchable_17::variant::grab().set_p5(5);
    matchable_17::variant::grab().set_p6(6);
    matchable_17::variant::grab().set_p7(7);
    matchable_17::variant::grab().set_p8(8);
    matchable_17::variant::grab().set_p9(9);
    matchable_17::variant::grab().set_p10(10);
    matchable_17::variant::grab().set_p11(11);
    matchable_17::variant::grab().set_p12(12);
    matchable_17::variant::grab().set_p13(13);
    matchable_17::variant::grab().set_p14(14);
    matchable_17::variant::grab().set_p15(15);
    matchable_17::variant::grab().set_p16(16);
    matchable_17::variant::grab().set_p17(17);
    TEST_EQ(ok, matchable_17::variant::grab().as_p1(), 1);
    TEST_EQ(ok, matchable_17::variant::grab().as_p2(), 2);
    TEST_EQ(ok, matchable_17::variant::grab().as_p3(), 3);
    TEST_EQ(ok, matchable_17::variant::grab().as_p4(), 4);
    TEST_EQ(ok, matchable_17::variant::grab().as_p5(), 5);
    TEST_EQ(ok, matchable_17::variant::grab().as_p6(), 6);
    TEST_EQ(ok, matchable_17::variant::grab().as_p7(), 7);
    TEST_EQ(ok, matchable_17::variant::grab().as_p8(), 8);
    TEST_EQ(ok, matchable_17::variant::grab().as_p9(), 9);
    TEST_EQ(ok, matchable_17::variant::grab().as_p10(), 10);
    TEST_EQ(ok, matchable_17::variant::grab().as_p11(), 11);
    TEST_EQ(ok, matchable_17::variant::grab().as_p12(), 12);
    TEST_EQ(ok, matchable_17::variant::grab().as_p13(), 13);
    TEST_EQ(ok, matchable_17::variant::grab().as_p14(), 14);
    TEST_EQ(ok, matchable_17::variant::grab().as_p15(), 15);
    TEST_EQ(ok, matchable_17::variant::grab().as_p16(), 16);
    TEST_EQ(ok, matchable_17::variant::grab().as_p17(), 17);

    return ok();
}
