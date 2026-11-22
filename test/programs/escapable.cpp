#include <cstdio>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(
    special,
    esc_alignas,
    esc_alignof,
    esc_and,
    esc_and_eq,
    esc_asm,
    esc_atomic_cancel,
    esc_atomic_commit,
    esc_atomic_noexcept,
    esc_auto,
    esc_bitand,
    esc_bitor,
    esc_bool,
    esc_break,
    esc_case,
    esc_catch,
    esc_char,
    esc_char8_t,
    esc_char16_t,
    esc_char32_t,
    esc_class,
    esc_compl,
    esc_concept,
    esc_const,
    esc_consteval,
    esc_constexpr,
    esc_constinit,
    esc_const_cast,
    esc_continue,
    esc_co_await,
    esc_co_return,
    esc_co_yield,
    esc_decltype,
    esc_default,
    esc_delete,
    esc_do,
    esc_double,
    esc_dynamic_cast,
    esc_else,
    esc_enum,
    esc_explicit,
    esc_export,
    esc_extern,
    esc_false,
    esc_float,
    esc_for,
    esc_friend,
    esc_goto,
    esc_if,
    esc_inline,
    esc_int,
    esc_long,
    esc_mutable,
    esc_namespace,
    esc_new,
    esc_noexcept,
    esc_not,
    esc_not_eq,
    esc_nullptr,
    esc_operator,
    esc_or,
    esc_or_eq,
    esc_private,
    esc_protected,
    esc_public,
    esc_reflexpr,
    esc_register,
    esc_reinterpret_cast,
    esc_requires,
    esc_return,
    esc_short,
    esc_signed,
    esc_sizeof,
    esc_static,
    esc_static_assert,
    esc_static_cast,
    esc_struct,
    esc_switch,
    esc_synchronized,
    esc_template,
    esc_this,
    esc_thread_local,
    esc_throw,
    esc_true,
    esc_try,
    esc_typedef,
    esc_typeid,
    esc_typename,
    esc_union,
    esc_unsigned,
    esc_using,
    esc_virtual,
    esc_void,
    esc_volatile,
    esc_wchar_t,
    esc_while,
    esc_xor,
    esc_xor_eq,
    esc_assert,
    esc_clone,
    esc_create,
    esc_grab,
    esc_std
)
MATCHABLE(utf8, caf_xc3__xa9_, _xc3__x9c_ber, esc__xc3__xa9_t_xc3__xa9_)



// escape_all() and unescape_all() round trip symbols and bytes 128 and up next to each other and to text
// that spells no code
void check_round_trips(test_ok & ok)
{
    std::vector<std::string> const pieces{
        "a", "Z", "0", "q", " ", "&", "(", ")", "~", "-", "\xc3", "\xa9", "\xff", "\x80"
    };
    std::mt19937 rng{20261122};
    for (int i = 0; i < 20000; ++i)
    {
        std::string s;
        for (int n = (int) (rng() % 6) + 1; n > 0; --n)
            s += pieces[rng() % pieces.size()];
        TEST_EQ(ok, matchable::escapable::unescape_all(matchable::escapable::escape_all(s)), s);
    }
}



int main()
{
    test_ok ok;

    std::cout << "special by index:" << std::endl;
    for (auto s : special::variants())
    {
        std::cout << "    " << s << std::endl;
        TEST_EQ(ok, s.as_identifier_string().substr(0, 4), std::string("esc_"));
        TEST_EQ(ok, s.as_identifier_string().substr(4, s.as_identifier_string().size()), s.as_string());
        TEST_EQ(ok, matchable::escapable::unescape_all(s.as_identifier_string()), s.as_string());
    }
    std::cout << std::endl << "special by string:" << std::endl;
    for (auto s : special::variants_by_string())
        std::cout << "    " << s << std::endl;

    std::cout << "\nescape codes:" << std::endl;
    for (auto const & [code, symbol] : matchable::escapable::code_symbol_pairs())
        std::cout << "    " << code << " matches '" << symbol << "'" << std::endl;

    auto s = special::from_string("xor_eq");
    TEST_EQ(ok, s, special::esc_xor_eq::grab());
    TEST_EQ(ok, s, special::from_identifier_string("esc_xor_eq"));

    std::string esc{"_"};
    TEST_EQ(ok, esc, matchable::escapable::escape(esc));

    esc = "1";
    TEST_EQ(ok, esc, matchable::escapable::escape(esc));

    esc = "s";
    TEST_EQ(ok, esc, matchable::escapable::escape(esc));

    for (int byte = 128; byte < 256; ++byte)
    {
        char code[8];
        std::snprintf(code, sizeof(code), "_x%02x_", byte);
        std::string const raw(1, (char) byte);
        TEST_EQ(ok, matchable::escapable::escape(raw), std::string(code));
        TEST_EQ(ok, matchable::escapable::unescape(code), raw);
        TEST_EQ(ok, matchable::escapable::unescape_all(code), raw);
    }
    check_round_trips(ok);

    // codes next to each other each keep both of their own underscores
    TEST_EQ(ok, matchable::escapable::unescape_all("_parl__und__parl_"), std::string("(&("));
    TEST_EQ(ok, matchable::escapable::unescape_all("_parl_und_parl_"), std::string("_parl&parl_"));

    TEST_EQ(ok, utf8::caf_xc3__xa9_::grab().as_string(), std::string("caf\xc3\xa9"));
    TEST_EQ(ok, utf8::_xc3__x9c_ber::grab().as_string(), std::string("\xc3\x9c" "ber"));
    TEST_EQ(ok, utf8::esc__xc3__xa9_t_xc3__xa9_::grab().as_string(), std::string("\xc3\xa9t\xc3\xa9"));
    TEST_EQ(ok, utf8::from_string("caf\xc3\xa9"), utf8::caf_xc3__xa9_::grab());
    TEST_EQ(ok, utf8::variants_by_string()[0], utf8::caf_xc3__xa9_::grab());
    TEST_EQ(ok, utf8::variants_by_string()[1], utf8::_xc3__x9c_ber::grab());
    TEST_EQ(ok, matchable::escapable::escape_all("caf\xc3\xa9"), std::string("caf_xc3__xa9_"));

    return ok();
}
