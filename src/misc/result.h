#pragma once
#include "assert.h"
#include <concepts>
#include <type_traits>

// #include "../3rd-party/result.hpp"



namespace result {

template<typename E>
class NoResult {};


namespace detail {


template<typename T, typename E>
struct Result_Base;

template<typename T, typename E>
requires std::is_lvalue_reference_v<T>
class Result_Base<T, E> {
    // bool m_has_value = false;
    // union {
    //     std::remove_reference<T>* m_ok;
    //     E                         m_err;
    // };
};

template<typename T, typename E>
requires(!std::is_lvalue_reference_v<T>) class Result_Base<T, E> {
    // bool m_has_value = false;
    // union {
    //     T m_ok;
    //     E m_err;
    // };
};

} // namespace detail

template<typename T, typename E>
class Result /*: public detail::Result_Base<T, E>*/ {
public:
    using ValueType = T;
    using ErrorType = E;
    static_assert(
        !std::is_rvalue_reference_v<T>, "It is ill-formed for T to be an rvalue 'result type. "
                                        "Only lvalue references are valid.");
    static_assert(
        !std::is_reference_v<E>, "It is ill-formed for E to be a reference type. "
                                 "Only T types may be lvalue references");


    // Default ctors
    constexpr Result() = delete;
    // Copy ctors
    constexpr Result(const Result& o) = default;
    // Move ctors
    constexpr Result(Result&& o) = default;
    // Ctors



    Result(const ValueType& value)
        : m_has_value(true)
        , m_ok(value) {
        // new (&m_ok) ValueType(value);
    }

    Result(ErrorType&& error)
        : m_has_value(false)
        , m_err(std::move(error)) {
        // new (&m_err) ErrorType(std::move(error));
    }

    ~Result() {
        if (this->is_ok()) {
            m_ok.~T();
        } else {
            m_err.~E();
        }
    }
    constexpr auto is_ok() const -> bool { return m_has_value; }
    constexpr auto is_err() const -> bool { return !this->is_ok(); }

private:
    bool m_has_value = false;
    union {
        T m_ok;
        E m_err;
    };

    // cpp::result<T, E> m_r;
};


// template<typename T, typename E>
// class Result2 {

//     Option m_ok;
//     Option m_err;
// };

} // namespace result

// using result::Result;
