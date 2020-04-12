#pragma once



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



#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>



namespace matchable
{
    /**
     * Associative array of Matchable variants to T.
     *
     * Example: // MatchBoxUsage.cpp
     *          #include <iostream>
     *          #include "matchable.h"
     *
     *          MATCHABLE(ReturnCode, Zero, MinusOne, MinusTwo)
     *          MATCHABLE(ReturnCodeAsValue, Success, InvalidInput, CalculationFailed)
     *
     *          MatchBox<ReturnCode::Type, ReturnCodeAsValue::Type> code_value_matcher{
     *              { ReturnCode::Zero::grab(), ReturnCodeAsValue::Success::grab() },
     *              { ReturnCode::MinusOne::grab(), ReturnCodeAsValue::InvalidInput::grab() },
     *              { ReturnCode::MinusTwo::grab(), ReturnCodeAsValue::CalculationFailed::grab() }
     *          };
     *
     *          int main()
     *          {
     *              for (auto const & return_code : ReturnCode::variants())
     *                  std::cout << "code: " << return_code << ", as value: "
     *                                        << code_value_matcher.at(return_code) << std::endl;
     *              return 0;
     *          }
     *
     */
    template<typename M, typename T>
    class MatchBox
    {
    public:
        using match_t = M;
        using target_t = T;

        MatchBox() : MatchBox(T{}) {}
        explicit MatchBox(T const & fill);
        MatchBox(std::initializer_list<std::pair<M, T>> init_list);
        T const & at(M const &) const;
        T & mut_at(M const &);
        void set(M const &, T const &);
        void unset(M const &);
        bool is_set(M const &) const;
        std::vector<M> currently_set() const;

    private:
        T nil_target;
        bool nil_target_init_flag;
        T default_t;
        std::vector<T> targets;
        std::vector<bool> init_flags;
    };


    template<typename M, typename T>
    MatchBox<M, T>::MatchBox(T const & fill)
        : nil_target{}
        , nil_target_init_flag{false}
        , default_t{fill}
    {
        targets.reserve(M::variants().size());
        init_flags.reserve(M::variants().size());

        for (size_t i = 0; i < M::variants().size(); ++i)
        {
            targets.push_back(default_t);
            init_flags.push_back(false);
        }
    }


    template<typename M, typename T>
    MatchBox<M, T>::MatchBox(std::initializer_list<std::pair<M, T>> init_list)
        : MatchBox()
    {
        for (auto const & elem : init_list)
            set(elem.first, elem.second);
    }


    template<typename M, typename T>
    T const & MatchBox<M, T>::at(M const & match) const
    {
        return match.is_nil() ? nil_target : targets[static_cast<size_t>(match.as_index())];
    }


    template<typename M, typename T>
    T & MatchBox<M, T>::mut_at(M const & match)
    {
        return match.is_nil() ? nil_target : targets[static_cast<size_t>(match.as_index())];
    }


    template<typename M, typename T>
    void MatchBox<M, T>::set(M const & match, T const & target)
    {
        if (match.is_nil())
        {
            nil_target = target;
            nil_target_init_flag = true;
        }
        else
        {
            size_t index = static_cast<size_t>(match.as_index());
            targets[index] = target;
            init_flags[index] = true;
        }
    }


    template<typename M, typename T>
    void MatchBox<M, T>::unset(M const & match)
    {
        if (match.is_nil())
        {
            nil_target = default_t;
            nil_target_init_flag = false;
        }
        else
        {
            size_t index = static_cast<size_t>(match.as_index());
            targets[index] = default_t;
            init_flags[index] = false;
        }
    }


    template<typename M, typename T>
    bool MatchBox<M, T>::is_set(const M & match) const
    {
        if (match.is_nil())
            return nil_target_init_flag;

        return init_flags[static_cast<size_t>(match.as_index())];
    }


    template<typename M, typename T>
    std::vector<M> MatchBox<M, T>::currently_set() const
    {
        std::vector<M> ret;
        for (auto const & m : M::variants())
            if (is_set(m))
                ret.push_back(m);
        return ret;
    }


    /**
     * MatchBox stores a bool for each element anyway for tracking explicit setting, so MatchBox<M, bool>
     * would yield 2 bools per element. To avoid the extra bool use this MatchBox<M, void> specialization
     * instead.
     */
    template<typename M>
    class MatchBox<M, void>
    {
    public:
        using match_t = M;

        MatchBox();
        void set(M const &);
        void unset(M const &);
        bool is_set(M const &) const;
        std::vector<M> currently_set() const;
        bool operator==(MatchBox const &) const;
        bool operator!=(MatchBox const &) const;
        friend std::ostream & operator<<(std::ostream & o, MatchBox<M, void> const & m)
        {
            for (auto v : M::variants())
                o << v << ": " << m.is_set(v) << ", ";
            return o << M() << ": " << m.is_set(M());
        }

    private:
        bool nil_init_flag;
        std::vector<bool> init_flags;
    };

    template<typename M>
    MatchBox<M, void>::MatchBox() : nil_init_flag{false}
    {
        init_flags.reserve(M::variants().size());
        for (size_t i = 0; i < M::variants().size(); ++i)
            init_flags.push_back(false);
    }


    template<typename M>
    void MatchBox<M, void>::set(M const & match)
    {
        if (match.is_nil())
            nil_init_flag = true;
        else
            init_flags[static_cast<size_t>(match.as_index())] = true;
    }


    template<typename M>
    void MatchBox<M, void>::unset(M const & match)
    {
        if (match.is_nil())
            nil_init_flag = false;
        else
            init_flags[static_cast<size_t>(match.as_index())] = false;
    }


    template<typename M>
    bool MatchBox<M, void>::is_set(M const & match) const
    {
        if (match.is_nil())
            return nil_init_flag;

        return init_flags[static_cast<size_t>(match.as_index())];
    }


    template<typename M>
    std::vector<M> MatchBox<M, void>::currently_set() const
    {
        std::vector<M> ret;
        for (auto const & m : M::variants())
            if (is_set(m))
                ret.push_back(m);
        return ret;
    }


    template<typename M>
    bool MatchBox<M, void>::operator==(MatchBox<M, void> const & other) const
    {
        return init_flags == other.init_flags;
    }


    template<typename M>
    bool MatchBox<M, void>::operator!=(MatchBox<M, void> const & other) const
    {
        return !operator==(other);
    }


    class FlowControl
    {
    public:
        void cont() { c = true; }
        void brk() { b = true; }
        bool cont_requested() const { return c; }
        bool brk_requested() const { return b; }
    private:
        bool c{false};
        bool b{false};
    };


    template<typename M>
    class Unmatchable
    {
    public:
        Unmatchable(std::initializer_list<M> um)
        {
            prev_variants = M::variants();
            auto i = std::remove_if(
                M::interface_type::private_variants().begin(),
                M::interface_type::private_variants().end(),
                [&](M m){ return std::find(um.begin(), um.end(), m) != um.end(); }
            );
            M::interface_type::private_variants().erase(i, M::interface_type::private_variants().end());
        }
        ~Unmatchable()
        {
            M::interface_type::private_variants() = prev_variants;
        }
    private:
        std::vector<M> prev_variants;
    };

} // namespace matchable



// next come the macros...
//
// note that compiling backslashes is slow!
// code is formatted to minimize backslashes while maintaining some semblance of readability


#define _matchable_declare_begin(_t)                                                                       \
    namespace _t                                                                                           \
    {                                                                                                      \
        using Type = Matchable<class I##_t>;                                                               \
        class I##_t                                                                                        \
        {                                                                                                  \
            friend class Matchable<I##_t>;                                                                 \
            using interface_type = I##_t;                                                                  \
            friend class ::matchable::Unmatchable<Matchable<I##_t>>;                                       \
        public:                                                                                            \
            I##_t() = default;                                                                             \
            virtual ~I##_t() = default;                                                                    \
            virtual std::string const & as_string() const = 0;                                             \
            virtual int as_index() const = 0;                                                              \
            static std::vector<Type> const & variants() { return private_variants(); }                     \
            static bool register_variant(Type const & variant, int * i);                                   \
        protected:                                                                                         \
        private:                                                                                           \
            virtual std::shared_ptr<I##_t> clone() const = 0;                                              \
            static bool & nil_flag() { static bool nf{false}; return nf; }                                 \
            static std::vector<Type> & private_variants() { static std::vector<Type> v; return v; }


#define _matchable_declare_end(_t)                                                                         \
        };                                                                                                 \
    }


#define _matchable_create_type_begin(_t)                                                                   \
    namespace _t                                                                                           \
    {                                                                                                      \
        template<typename T>                                                                               \
        class Matchable                                                                                    \
        {                                                                                                  \
        public:                                                                                            \
            using interface_type = T;                                                                      \
            using MatchParam = ::matchable::MatchBox<Matchable, std::function<void()>>;                    \
            using MatchParamWithFlowControl =                                                              \
                    ::matchable::MatchBox<Matchable, std::function<void(matchable::FlowControl &)>>;       \
            Matchable() = default;                                                                         \
            ~Matchable() = default;                                                                        \
            Matchable(std::shared_ptr<T> m) : t{m} {}                                                      \
            Matchable(Matchable const & o) : t{nullptr == o.t ? nullptr : o.t->clone()} {}                 \
            Matchable(Matchable &&) = default;                                                             \
            Matchable & operator=(Matchable const & other)                                                 \
            {                                                                                              \
                if (this != &other)                                                                        \
                    t = nullptr == other.t ? nullptr : other.t->clone();                                   \
                return *this;                                                                              \
            }                                                                                              \
            Matchable & operator=(Matchable &&) = default;                                                 \
            std::string const & as_string() const                                                          \
            {                                                                                              \
                static std::string const nil_str{"nil"};                                                   \
                return nullptr == t ? nil_str : t->as_string();                                            \
            }                                                                                              \
            std::string as_text() const                                                                    \
                { std::string s{as_string()}; std::replace(s.begin(), s.end(), '_', ' '); return s; }      \
            int as_index() const { return nullptr == t ? -1 : t->as_index(); }                             \
            bool is_nil() const { return nullptr == t; }                                                   \
            matchable::FlowControl match(MatchParamWithFlowControl const & mb) const                       \
            {                                                                                              \
                matchable::FlowControl lc;                                                                 \
                if (mb.is_set(*this))                                                                      \
                    mb.at(*this)(lc);                                                                      \
                return lc;                                                                                 \
            }                                                                                              \
            void match(MatchParam const & mb) const                                                        \
            {                                                                                              \
                if (mb.is_set(*this))                                                                      \
                    mb.at(*this)();                                                                        \
            }                                                                                              \
            static std::vector<Matchable> const & variants() { return T::variants(); }                     \
            bool operator==(Matchable const & m) const { return as_string() == m.as_string(); }            \
            bool operator!=(Matchable const & m) const { return as_string() != m.as_string(); }            \
            bool operator<(Matchable const & m) const { return as_index() < m.as_index(); }                \
            bool lt_by_string(Matchable const & m) const { return as_string() < m.as_string(); }           \
            bool lt_by_index(Matchable const & m) const { return as_index() < m.as_index(); }              \
            friend std::ostream & operator<<(std::ostream & o, Matchable const & m)                        \
            {                                                                                              \
                return o << m.as_string();                                                                 \
            }                                                                                              \
        private:                                                                                           \
            std::shared_ptr<T> t;


#define _matchable_create_type_end(_t)                                                                     \
        };                                                                                                 \
    }


#define _matchable_define(_t)                                                                              \
    namespace _t                                                                                           \
    {                                                                                                      \
        using Flags = matchable::MatchBox<Type, void>;                                                     \
        inline std::vector<Type> const & variants() { return I##_t::variants(); }                          \
        static const Type nil{};                                                                           \
        inline Type from_index(int index)                                                                  \
        {                                                                                                  \
            if (index < 0 || index >= (int) I##_t::variants().size())                                      \
                return nil;                                                                                \
            return I##_t::variants().at(index);                                                            \
        }                                                                                                  \
        inline Type from_string(std::string const & str)                                                   \
        {                                                                                                  \
            for (auto const & v : I##_t::variants())                                                       \
                if (v.as_string() == str)                                                                  \
                    return v;                                                                              \
            return nil;                                                                                    \
        }                                                                                                  \
        static bool const register_##_t = I##_t::register_variant(nil, nullptr);                           \
        inline bool I##_t::register_variant(Type const & variant, int * i)                                 \
        {                                                                                                  \
            if (variant.is_nil())                                                                          \
            {                                                                                              \
                private_variants().clear();                                                                \
            }                                                                                              \
            else                                                                                           \
            {                                                                                              \
                if (nullptr != i)                                                                          \
                    *i = static_cast<int>(variants().size());                                              \
                private_variants().push_back(variant);                                                     \
            }                                                                                              \
            return true;                                                                                   \
        }                                                                                                  \
    }


#define _matchable_create_variant_begin(_t, _v)                                                            \
    namespace _t                                                                                           \
    {                                                                                                      \
        class _v : public I##_t                                                                            \
        {                                                                                                  \
        public:                                                                                            \
            _v() = default;                                                                                \
            std::string const & as_string() const override { static std::string const s{#_v}; return s; }  \
            int as_index() const override { return *m_index(); }                                           \
            static Type grab() { return Type(create()); }                                                  \
            static int * m_index() { static int i{-1}; return &i; }                                        \
        private:                                                                                           \
            std::shared_ptr<I##_t> clone() const  override { return create(); }                            \
            static std::shared_ptr<_v> create() { return std::make_shared<_v>(); }


#define _matchable_create_variant_end(_t, _v)                                                              \
        };                                                                                                 \
        static bool const register_me_##_t##_v = I##_t::register_variant(_v::grab(), _v::m_index());       \
    }


#define _matchable_create_variant(_t, _v)                                                                  \
    _matchable_create_variant_begin(_t, _v)                                                                \
    _matchable_create_variant_end(_t, _v)


#define _matchable_concat_variant(_t, _v) _t::_v::grab(),


#define   _mcv_0(_m, _t, ...)
#define   _mcv_1(_m, _t, _v)      _m(_t, _v)
#define   _mcv_2(_m, _t, _v, ...) _m(_t, _v)   _mcv_1(_m, _t, __VA_ARGS__)
#define   _mcv_3(_m, _t, _v, ...) _m(_t, _v)   _mcv_2(_m, _t, __VA_ARGS__)
#define   _mcv_4(_m, _t, _v, ...) _m(_t, _v)   _mcv_3(_m, _t, __VA_ARGS__)
#define   _mcv_5(_m, _t, _v, ...) _m(_t, _v)   _mcv_4(_m, _t, __VA_ARGS__)
#define   _mcv_6(_m, _t, _v, ...) _m(_t, _v)   _mcv_5(_m, _t, __VA_ARGS__)
#define   _mcv_7(_m, _t, _v, ...) _m(_t, _v)   _mcv_6(_m, _t, __VA_ARGS__)
#define   _mcv_8(_m, _t, _v, ...) _m(_t, _v)   _mcv_7(_m, _t, __VA_ARGS__)
#define   _mcv_9(_m, _t, _v, ...) _m(_t, _v)   _mcv_8(_m, _t, __VA_ARGS__)
#define  _mcv_10(_m, _t, _v, ...) _m(_t, _v)   _mcv_9(_m, _t, __VA_ARGS__)
#define  _mcv_11(_m, _t, _v, ...) _m(_t, _v)  _mcv_10(_m, _t, __VA_ARGS__)
#define  _mcv_12(_m, _t, _v, ...) _m(_t, _v)  _mcv_11(_m, _t, __VA_ARGS__)
#define  _mcv_13(_m, _t, _v, ...) _m(_t, _v)  _mcv_12(_m, _t, __VA_ARGS__)
#define  _mcv_14(_m, _t, _v, ...) _m(_t, _v)  _mcv_13(_m, _t, __VA_ARGS__)
#define  _mcv_15(_m, _t, _v, ...) _m(_t, _v)  _mcv_14(_m, _t, __VA_ARGS__)
#define  _mcv_16(_m, _t, _v, ...) _m(_t, _v)  _mcv_15(_m, _t, __VA_ARGS__)
#define  _mcv_17(_m, _t, _v, ...) _m(_t, _v)  _mcv_16(_m, _t, __VA_ARGS__)
#define  _mcv_18(_m, _t, _v, ...) _m(_t, _v)  _mcv_17(_m, _t, __VA_ARGS__)
#define  _mcv_19(_m, _t, _v, ...) _m(_t, _v)  _mcv_18(_m, _t, __VA_ARGS__)
#define  _mcv_20(_m, _t, _v, ...) _m(_t, _v)  _mcv_19(_m, _t, __VA_ARGS__)
#define  _mcv_21(_m, _t, _v, ...) _m(_t, _v)  _mcv_20(_m, _t, __VA_ARGS__)
#define  _mcv_22(_m, _t, _v, ...) _m(_t, _v)  _mcv_21(_m, _t, __VA_ARGS__)
#define  _mcv_23(_m, _t, _v, ...) _m(_t, _v)  _mcv_22(_m, _t, __VA_ARGS__)
#define  _mcv_24(_m, _t, _v, ...) _m(_t, _v)  _mcv_23(_m, _t, __VA_ARGS__)
#define  _mcv_25(_m, _t, _v, ...) _m(_t, _v)  _mcv_24(_m, _t, __VA_ARGS__)
#define  _mcv_26(_m, _t, _v, ...) _m(_t, _v)  _mcv_25(_m, _t, __VA_ARGS__)
#define  _mcv_27(_m, _t, _v, ...) _m(_t, _v)  _mcv_26(_m, _t, __VA_ARGS__)
#define  _mcv_28(_m, _t, _v, ...) _m(_t, _v)  _mcv_27(_m, _t, __VA_ARGS__)
#define  _mcv_29(_m, _t, _v, ...) _m(_t, _v)  _mcv_28(_m, _t, __VA_ARGS__)
#define  _mcv_30(_m, _t, _v, ...) _m(_t, _v)  _mcv_29(_m, _t, __VA_ARGS__)
#define  _mcv_31(_m, _t, _v, ...) _m(_t, _v)  _mcv_30(_m, _t, __VA_ARGS__)
#define  _mcv_32(_m, _t, _v, ...) _m(_t, _v)  _mcv_31(_m, _t, __VA_ARGS__)
#define  _mcv_33(_m, _t, _v, ...) _m(_t, _v)  _mcv_32(_m, _t, __VA_ARGS__)
#define  _mcv_34(_m, _t, _v, ...) _m(_t, _v)  _mcv_33(_m, _t, __VA_ARGS__)
#define  _mcv_35(_m, _t, _v, ...) _m(_t, _v)  _mcv_34(_m, _t, __VA_ARGS__)
#define  _mcv_36(_m, _t, _v, ...) _m(_t, _v)  _mcv_35(_m, _t, __VA_ARGS__)
#define  _mcv_37(_m, _t, _v, ...) _m(_t, _v)  _mcv_36(_m, _t, __VA_ARGS__)
#define  _mcv_38(_m, _t, _v, ...) _m(_t, _v)  _mcv_37(_m, _t, __VA_ARGS__)
#define  _mcv_39(_m, _t, _v, ...) _m(_t, _v)  _mcv_38(_m, _t, __VA_ARGS__)
#define  _mcv_40(_m, _t, _v, ...) _m(_t, _v)  _mcv_39(_m, _t, __VA_ARGS__)
#define  _mcv_41(_m, _t, _v, ...) _m(_t, _v)  _mcv_40(_m, _t, __VA_ARGS__)
#define  _mcv_42(_m, _t, _v, ...) _m(_t, _v)  _mcv_41(_m, _t, __VA_ARGS__)
#define  _mcv_43(_m, _t, _v, ...) _m(_t, _v)  _mcv_42(_m, _t, __VA_ARGS__)
#define  _mcv_44(_m, _t, _v, ...) _m(_t, _v)  _mcv_43(_m, _t, __VA_ARGS__)
#define  _mcv_45(_m, _t, _v, ...) _m(_t, _v)  _mcv_44(_m, _t, __VA_ARGS__)
#define  _mcv_46(_m, _t, _v, ...) _m(_t, _v)  _mcv_45(_m, _t, __VA_ARGS__)
#define  _mcv_47(_m, _t, _v, ...) _m(_t, _v)  _mcv_46(_m, _t, __VA_ARGS__)
#define  _mcv_48(_m, _t, _v, ...) _m(_t, _v)  _mcv_47(_m, _t, __VA_ARGS__)
#define  _mcv_49(_m, _t, _v, ...) _m(_t, _v)  _mcv_48(_m, _t, __VA_ARGS__)
#define  _mcv_50(_m, _t, _v, ...) _m(_t, _v)  _mcv_49(_m, _t, __VA_ARGS__)
#define  _mcv_51(_m, _t, _v, ...) _m(_t, _v)  _mcv_50(_m, _t, __VA_ARGS__)
#define  _mcv_52(_m, _t, _v, ...) _m(_t, _v)  _mcv_51(_m, _t, __VA_ARGS__)
#define  _mcv_53(_m, _t, _v, ...) _m(_t, _v)  _mcv_52(_m, _t, __VA_ARGS__)
#define  _mcv_54(_m, _t, _v, ...) _m(_t, _v)  _mcv_53(_m, _t, __VA_ARGS__)
#define  _mcv_55(_m, _t, _v, ...) _m(_t, _v)  _mcv_54(_m, _t, __VA_ARGS__)
#define  _mcv_56(_m, _t, _v, ...) _m(_t, _v)  _mcv_55(_m, _t, __VA_ARGS__)
#define  _mcv_57(_m, _t, _v, ...) _m(_t, _v)  _mcv_56(_m, _t, __VA_ARGS__)
#define  _mcv_58(_m, _t, _v, ...) _m(_t, _v)  _mcv_57(_m, _t, __VA_ARGS__)
#define  _mcv_59(_m, _t, _v, ...) _m(_t, _v)  _mcv_58(_m, _t, __VA_ARGS__)
#define  _mcv_60(_m, _t, _v, ...) _m(_t, _v)  _mcv_59(_m, _t, __VA_ARGS__)
#define  _mcv_61(_m, _t, _v, ...) _m(_t, _v)  _mcv_60(_m, _t, __VA_ARGS__)
#define  _mcv_62(_m, _t, _v, ...) _m(_t, _v)  _mcv_61(_m, _t, __VA_ARGS__)
#define  _mcv_63(_m, _t, _v, ...) _m(_t, _v)  _mcv_62(_m, _t, __VA_ARGS__)
#define  _mcv_64(_m, _t, _v, ...) _m(_t, _v)  _mcv_63(_m, _t, __VA_ARGS__)
#define  _mcv_65(_m, _t, _v, ...) _m(_t, _v)  _mcv_64(_m, _t, __VA_ARGS__)
#define  _mcv_66(_m, _t, _v, ...) _m(_t, _v)  _mcv_65(_m, _t, __VA_ARGS__)
#define  _mcv_67(_m, _t, _v, ...) _m(_t, _v)  _mcv_66(_m, _t, __VA_ARGS__)
#define  _mcv_68(_m, _t, _v, ...) _m(_t, _v)  _mcv_67(_m, _t, __VA_ARGS__)
#define  _mcv_69(_m, _t, _v, ...) _m(_t, _v)  _mcv_68(_m, _t, __VA_ARGS__)
#define  _mcv_70(_m, _t, _v, ...) _m(_t, _v)  _mcv_69(_m, _t, __VA_ARGS__)
#define  _mcv_71(_m, _t, _v, ...) _m(_t, _v)  _mcv_70(_m, _t, __VA_ARGS__)
#define  _mcv_72(_m, _t, _v, ...) _m(_t, _v)  _mcv_71(_m, _t, __VA_ARGS__)
#define  _mcv_73(_m, _t, _v, ...) _m(_t, _v)  _mcv_72(_m, _t, __VA_ARGS__)
#define  _mcv_74(_m, _t, _v, ...) _m(_t, _v)  _mcv_73(_m, _t, __VA_ARGS__)
#define  _mcv_75(_m, _t, _v, ...) _m(_t, _v)  _mcv_74(_m, _t, __VA_ARGS__)
#define  _mcv_76(_m, _t, _v, ...) _m(_t, _v)  _mcv_75(_m, _t, __VA_ARGS__)
#define  _mcv_77(_m, _t, _v, ...) _m(_t, _v)  _mcv_76(_m, _t, __VA_ARGS__)
#define  _mcv_78(_m, _t, _v, ...) _m(_t, _v)  _mcv_77(_m, _t, __VA_ARGS__)
#define  _mcv_79(_m, _t, _v, ...) _m(_t, _v)  _mcv_78(_m, _t, __VA_ARGS__)
#define  _mcv_80(_m, _t, _v, ...) _m(_t, _v)  _mcv_79(_m, _t, __VA_ARGS__)
#define  _mcv_81(_m, _t, _v, ...) _m(_t, _v)  _mcv_80(_m, _t, __VA_ARGS__)
#define  _mcv_82(_m, _t, _v, ...) _m(_t, _v)  _mcv_81(_m, _t, __VA_ARGS__)
#define  _mcv_83(_m, _t, _v, ...) _m(_t, _v)  _mcv_82(_m, _t, __VA_ARGS__)
#define  _mcv_84(_m, _t, _v, ...) _m(_t, _v)  _mcv_83(_m, _t, __VA_ARGS__)
#define  _mcv_85(_m, _t, _v, ...) _m(_t, _v)  _mcv_84(_m, _t, __VA_ARGS__)
#define  _mcv_86(_m, _t, _v, ...) _m(_t, _v)  _mcv_85(_m, _t, __VA_ARGS__)
#define  _mcv_87(_m, _t, _v, ...) _m(_t, _v)  _mcv_86(_m, _t, __VA_ARGS__)
#define  _mcv_88(_m, _t, _v, ...) _m(_t, _v)  _mcv_87(_m, _t, __VA_ARGS__)
#define  _mcv_89(_m, _t, _v, ...) _m(_t, _v)  _mcv_88(_m, _t, __VA_ARGS__)
#define  _mcv_90(_m, _t, _v, ...) _m(_t, _v)  _mcv_89(_m, _t, __VA_ARGS__)
#define  _mcv_91(_m, _t, _v, ...) _m(_t, _v)  _mcv_90(_m, _t, __VA_ARGS__)
#define  _mcv_92(_m, _t, _v, ...) _m(_t, _v)  _mcv_91(_m, _t, __VA_ARGS__)
#define  _mcv_93(_m, _t, _v, ...) _m(_t, _v)  _mcv_92(_m, _t, __VA_ARGS__)
#define  _mcv_94(_m, _t, _v, ...) _m(_t, _v)  _mcv_93(_m, _t, __VA_ARGS__)
#define  _mcv_95(_m, _t, _v, ...) _m(_t, _v)  _mcv_94(_m, _t, __VA_ARGS__)
#define  _mcv_96(_m, _t, _v, ...) _m(_t, _v)  _mcv_95(_m, _t, __VA_ARGS__)
#define  _mcv_97(_m, _t, _v, ...) _m(_t, _v)  _mcv_96(_m, _t, __VA_ARGS__)
#define  _mcv_98(_m, _t, _v, ...) _m(_t, _v)  _mcv_97(_m, _t, __VA_ARGS__)
#define  _mcv_99(_m, _t, _v, ...) _m(_t, _v)  _mcv_98(_m, _t, __VA_ARGS__)
#define _mcv_100(_m, _t, _v, ...) _m(_t, _v)  _mcv_99(_m, _t, __VA_ARGS__)
#define _mcv_101(_m, _t, _v, ...) _m(_t, _v) _mcv_100(_m, _t, __VA_ARGS__)
#define _mcv_102(_m, _t, _v, ...) _m(_t, _v) _mcv_101(_m, _t, __VA_ARGS__)
#define _mcv_103(_m, _t, _v, ...) _m(_t, _v) _mcv_102(_m, _t, __VA_ARGS__)
#define _mcv_104(_m, _t, _v, ...) _m(_t, _v) _mcv_103(_m, _t, __VA_ARGS__)
#define _mcv_105(_m, _t, _v, ...) _m(_t, _v) _mcv_104(_m, _t, __VA_ARGS__)
#define _mcv_106(_m, _t, _v, ...) _m(_t, _v) _mcv_105(_m, _t, __VA_ARGS__)
#define _mcv_107(_m, _t, _v, ...) _m(_t, _v) _mcv_106(_m, _t, __VA_ARGS__)
#define _mcv_108(_m, _t, _v, ...) _m(_t, _v) _mcv_107(_m, _t, __VA_ARGS__)


#define _nth(                                                                                              \
      _0,   _1,   _2,   _3,   _4,   _5,   _6,   _7,   _8,  _9,                                             \
     _10,  _11,  _12,  _13,  _14,  _15,  _16,  _17,  _18, _19,                                             \
     _20,  _21,  _22,  _23,  _24,  _25,  _26,  _27,  _28, _29,                                             \
     _30,  _31,  _32,  _33,  _34,  _35,  _36,  _37,  _38, _39,                                             \
     _40,  _41,  _42,  _43,  _44,  _45,  _46,  _47,  _48, _49,                                             \
     _50,  _51,  _52,  _53,  _54,  _55,  _56,  _57,  _58, _59,                                             \
     _60,  _61,  _62,  _63,  _64,  _65,  _66,  _67,  _68, _69,                                             \
     _70,  _71,  _72,  _73,  _74,  _75,  _76,  _77,  _78, _79,                                             \
     _80,  _81,  _82,  _83,  _84,  _85,  _86,  _87,  _88, _89,                                             \
     _90,  _91,  _92,  _93,  _94,  _95,  _96,  _97,  _98, _99,                                             \
    _100, _101, _102, _103, _104, _105, _106, _107, _108,   N, ...                                         \
) N


#define _mcv(_macro, _t, ...)                                                                              \
    _nth(                                                                                                  \
        "107",                                                                                             \
        ##__VA_ARGS__,                                                                                     \
                 _mcv_108,_mcv_107,_mcv_106,_mcv_105,_mcv_104,_mcv_103,_mcv_102,_mcv_101,_mcv_100,         \
         _mcv_99, _mcv_98, _mcv_97, _mcv_96, _mcv_95, _mcv_94, _mcv_93, _mcv_92, _mcv_91, _mcv_90,         \
         _mcv_89, _mcv_88, _mcv_87, _mcv_86, _mcv_85, _mcv_84, _mcv_83, _mcv_82, _mcv_81, _mcv_80,         \
         _mcv_79, _mcv_78, _mcv_77, _mcv_76, _mcv_75, _mcv_74, _mcv_73, _mcv_72, _mcv_71, _mcv_70,         \
         _mcv_69, _mcv_68, _mcv_67, _mcv_66, _mcv_65, _mcv_64, _mcv_63, _mcv_62, _mcv_61, _mcv_60,         \
         _mcv_59, _mcv_58, _mcv_57, _mcv_56, _mcv_55, _mcv_54, _mcv_53, _mcv_52, _mcv_51, _mcv_50,         \
         _mcv_49, _mcv_48, _mcv_47, _mcv_46, _mcv_45, _mcv_44, _mcv_43, _mcv_42, _mcv_41, _mcv_40,         \
         _mcv_39, _mcv_38, _mcv_37, _mcv_36, _mcv_35, _mcv_34, _mcv_33, _mcv_32, _mcv_31, _mcv_30,         \
         _mcv_29, _mcv_28, _mcv_27, _mcv_26, _mcv_25, _mcv_24, _mcv_23, _mcv_22, _mcv_21, _mcv_20,         \
         _mcv_19, _mcv_18, _mcv_17, _mcv_16, _mcv_15, _mcv_14, _mcv_13, _mcv_12, _mcv_11, _mcv_10,         \
          _mcv_9,  _mcv_8,  _mcv_7,  _mcv_6,  _mcv_5,  _mcv_4,  _mcv_3,  _mcv_2,  _mcv_1,  _mcv_0          \
    )(_macro, _t, ##__VA_ARGS__)


#define MATCHABLE(_t, ...)                                                                                 \
    _matchable_create_type_begin(_t)                                                                       \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define _spread_matchable_amend_type(_s, _t)                                                               \
    public:                                                                                                \
        _s::Type as_##_s() const { return nullptr == t ? T::nil_##_s() : t->as_##_s(); }                   \
        void set_##_s(_s::Type s) { if (nullptr == t) T::nil_##_s() = s; else t->set_##_s(s); }



#define _spread_matchable_amend_declaration(_s, _t)                                                        \
    public:                                                                                                \
        _s::Type as_##_s() const { return _s##_mb().at(Type(clone())); }                                   \
        void set_##_s(_s::Type s) { _s##_mb().set(Type(clone()), s); }                                     \
    private:                                                                                               \
        static matchable::MatchBox<_t::Type, _s::Type> & _s##_mb()                                         \
            { static matchable::MatchBox<_t::Type, _s::Type> m; return m; }                                \
        static _s::Type nil_##_s() { static _s::Type ns; return ns; }


#define SPREADx1_MATCHABLE(_s0, _t, ...)                                                                   \
    _matchable_create_type_begin(_t)                                                                       \
    _spread_matchable_amend_type(_s0, _t)                                                                  \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _spread_matchable_amend_declaration(_s0, _t)                                                           \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define SPREADx2_MATCHABLE(_s0, _s1, _t, ...)                                                              \
    _matchable_create_type_begin(_t)                                                                       \
    _spread_matchable_amend_type(_s0, _t)                                                                  \
    _spread_matchable_amend_type(_s1, _t)                                                                  \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _spread_matchable_amend_declaration(_s0, _t)                                                           \
    _spread_matchable_amend_declaration(_s1, _t)                                                           \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define SPREADx3_MATCHABLE(_s0, _s1, _s2, _t, ...)                                                         \
    _matchable_create_type_begin(_t)                                                                       \
    _spread_matchable_amend_type(_s0, _t)                                                                  \
    _spread_matchable_amend_type(_s1, _t)                                                                  \
    _spread_matchable_amend_type(_s2, _t)                                                                  \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _spread_matchable_amend_declaration(_s0, _t)                                                           \
    _spread_matchable_amend_declaration(_s1, _t)                                                           \
    _spread_matchable_amend_declaration(_s2, _t)                                                           \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define SPREADx4_MATCHABLE(_s0, _s1, _s2, _s3, _t, ...)                                                    \
    _matchable_create_type_begin(_t)                                                                       \
    _spread_matchable_amend_type(_s0, _t)                                                                  \
    _spread_matchable_amend_type(_s1, _t)                                                                  \
    _spread_matchable_amend_type(_s2, _t)                                                                  \
    _spread_matchable_amend_type(_s3, _t)                                                                  \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _spread_matchable_amend_declaration(_s0, _t)                                                           \
    _spread_matchable_amend_declaration(_s1, _t)                                                           \
    _spread_matchable_amend_declaration(_s2, _t)                                                           \
    _spread_matchable_amend_declaration(_s3, _t)                                                           \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define VARIANT_SPREADVARIANT(_t, _v, _st, _sv)                                                            \
    static bool const VARIANT_SPREADVARIANT_init_##_t##_##_v##_##_st##_##_sv =                             \
        [](){_t::_v::grab().set_##_st(_st::_sv::grab()); return true;}();


#define _spreadvectof_matchable_amend_type(_s, _t)                                                         \
    public:                                                                                                \
        std::vector<_s::Type> as_##_s##_vect() const                                                       \
            { return nullptr == t ? T::nil_##_s##_vect() : t->as_##_s##_vect(); }                          \
        void set_##_s##_vect(std::vector<_s::Type> s_vect)                                                 \
            { if (nullptr == t) T::nil_##_s##_vect() = s_vect; else t->set_##_s##_vect(s_vect); }


#define _spreadvectof_matchable_amend_declaration(_s, _t)                                                  \
    public:                                                                                                \
        std::vector<_s::Type> as_##_s##_vect() const { return _s##_vect_mb().at(Type(clone())); }          \
        void set_##_s##_vect(std::vector<_s::Type> s_vect) { _s##_vect_mb().set(Type(clone()), s_vect); }  \
    private:                                                                                               \
        static matchable::MatchBox<_t::Type, std::vector<_s::Type>> & _s##_vect_mb()                       \
            { static matchable::MatchBox<_t::Type, std::vector<_s::Type>> m; return m; }                   \
        static std::vector<_s::Type> nil_##_s##_vect() { static std::vector<_s::Type> ns; return ns; }


/**
 * Usage: SPREADVECTOF_MATCHABLE(spread, type, variant...)
 *
 * Where: spread is: a type defined by MATCHABLE() or SPREAD*_MATCHABLE()
 */
#define SPREADVECTOF_MATCHABLE(_s, _t, ...)                                                                \
    _matchable_create_type_begin(_t)                                                                       \
    _spreadvectof_matchable_amend_type(_s, _t)                                                             \
    _matchable_create_type_end(_t)                                                                         \
    _matchable_declare_begin(_t)                                                                           \
    _spreadvectof_matchable_amend_declaration(_s, _t)                                                      \
    _matchable_declare_end(_t)                                                                             \
    _matchable_define(_t)                                                                                  \
    _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


#define VARIANT_SPREADVARIANTVECT(_t, _v, _st, ...)                                                        \
    static bool const VARIANT_SPREADVARIANTVECT_init_##_t##_##_v##_##_st =                                 \
        [](std::vector<_st::Type> sv) { _t::_v::grab().set_##_st##_##vect(sv); return true; }              \
            ({_mcv(_matchable_concat_variant, _st, ##__VA_ARGS__)});


/**
 * Remove variants for the current scope (when the scope exits the removed variants are restored).
 * Usage: UNMATCHABLE(type, variant...)
 */
#define UNMATCHABLE(_t, ...)                                                                               \
    matchable::Unmatchable<_t::Type> unm_##_t{{_mcv(_matchable_concat_variant, _t, ##__VA_ARGS__)}}


#define NAMESPACEx1_UNMATCHABLE(_n0, _t, ...)                                                              \
    matchable::Unmatchable<_n0::_t::Type> unm_##_n0##_##_t                                                 \
        {{_mcv(_matchable_concat_variant, _n0::_t, ##__VA_ARGS__)}}


#define NAMESPACEx2_UNMATCHABLE(_n0, _n1, _t, ...)                                                         \
    matchable::Unmatchable<_n0::_n1::_t::Type> unm_##_n0##_##_n1##_##_t                                    \
        {{_mcv(_matchable_concat_variant, _n0::_n1::_t, ##__VA_ARGS__)}}


#define NAMESPACEx3_UNMATCHABLE(_n0, _n1, _n2, _t, ...)                                                    \
    matchable::Unmatchable<_n0::_n1::_n2::_t::Type> unm_##_n0##_##_n1##_##_n2##_##_t                       \
        {{_mcv(_matchable_concat_variant, _n0::_n1::_n2::_t, ##__VA_ARGS__)}}


#define NAMESPACEx4_UNMATCHABLE(_n0, _n1, _n2, _n3, _t, ...)                                               \
    matchable::Unmatchable<_n0::_n1::_n2::_n3::_t::Type> unm_##_n0##_##_n1##_##_n2##_##_n3##_##_t          \
        {{_mcv(_matchable_concat_variant, _n0::_n1::_n2::_n3::_t, ##__VA_ARGS__)}}


#define NAMESPACEx5_UNMATCHABLE(_n0, _n1, _n2, _n3, _n4, _t, ...)                                          \
    matchable::Unmatchable<_n0::_n1::_n2::_n3::_n4::_t::Type>                                              \
        unm_##_n0##_##_n1##_##_n2##_##_n3##_##_n4##_##_t                                                   \
            {{_mcv(_matchable_concat_variant, _n0::_n1::_n2::_n3::_n4::_t, ##__VA_ARGS__)}}



/**
 * Test if a given matchable instance is contained within a given list of variants
 */
#define MATCHABLE_INSTANCE_IN(_t, _i, ...)                                                                 \
    [](_t::Type const & t, std::vector<_t::Type> const & v)                                                \
        { return std::find(v.begin(), v.end(), t) != v.end(); }                                            \
            (_i, {_mcv(_matchable_concat_variant, _t, ##__VA_ARGS__)})



#define MATCHABLE_GROW(_t, ...) _mcv(_matchable_create_variant, _t, ##__VA_ARGS__)


// FLOW CONTROL MACROS
#define MATCH_WITH_FLOW_CONTROL { matchable::FlowControl fc =
#define EVAL_FLOW_CONTROL if (fc.brk_requested()) break; if (fc.cont_requested()) continue; }
#define EVAL_BREAK_ONLY if (fc.brk_requested()) break; }
