#include <iostream>

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

    return ok();
}
