#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

/**
 *  Public interface,
 *  assertion-like macro.
 */
#define ENSURE(cond, reason)                                \
    ::test_tool::impl::ensure(                              \
        (cond), (#cond), (reason), (__PRETTY_FUNCTION__),   \
        (__FILE__), (__LINE__))



/**
 *  Impl.
 */
namespace test_tool::impl
{
//* Helper class tasked with outputting text and statistics
struct Statistics
{
    static inline auto tests_passed = 0ULL;
    static inline auto tests_failed = 0ULL;
    static inline bool fail_mode    = false;

    ~Statistics()
    {
        std::cout
            << "\n--Tests--\n"
            << " failed: " << std::setw(5) << std::right << tests_failed << "\n"
            << " passed: " << std::setw(5) << std::right << tests_passed << "\n"
            << " total:  " << std::setw(5) << std::right << (tests_failed + tests_passed) << "\n"
            ;
        tests_passed = 0;
        tests_failed = 0;
    }


    void pass(std::string_view reason) const noexcept
    {
        ++tests_passed;
        if (fail_mode) {std::cout << "\n";}
        fail_mode = false;
        std::cout << "test passed: " << reason.data()  << "\n";
    }


    void fail(
        std::string_view    cond_str,
        std::string_view    reason,
        std::string_view    func,
        std::string_view    file,
        const int           line
    )const noexcept
    {
        ++tests_failed;
        fail_mode = true;
        std::cout
            << "\ntest FAILED: " << reason.data() << "\n"
            << "condition:   " << cond_str.data() << "\n"
            << "in file:     " << file.data() << ":" << std::to_string(line) << "\n"
            << "@ function:  " << func.data() << "\n"
        ;
    }

};



//* Unique instance of our statistics helper
inline Statistics statistics;



struct Test
{
    const bool      condition       = false;
    const char*     cond_str;
    const char*     reason;
    const char*     func;
    const char*     file;
    const int       line            = 0;



    Test() = default;


    constexpr Test(
        bool        condition_,
        const char* cond_str_,
        const char* reason_,
        const char* func_,
        const char* file_,
        const int   line_
    ) noexcept
        : condition {condition_}
        , cond_str  {cond_str_}
        , reason    {reason_}
        , func      {func_}
        , file      {file_}
        , line      {line_}
    {
    }



    operator bool() const noexcept
    {
        return condition;
    }



    Test const& operator()() const
    {
        if (condition)
            statistics.pass(reason);

        else {
            statistics.fail(cond_str, reason, func, file, line);
        }

        return *this;
    }



    Test then(Test&& test) const
    {
        if (condition)
            return test();

        else return Test{};
    }

};



//* The ENSURE macro expands to that
inline Test ensure(
    bool                condition,
    std::string_view    cond_str,
    std::string_view    reason,
    std::string_view    func,
    std::string_view    file,
    int                 line
){
    return Test {
        condition,
        cond_str.data(),
        reason.data(),
        func.data(),
        file.data(),
        line
    }();
}



} // namespace test_tool::impl
