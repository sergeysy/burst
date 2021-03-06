#include <burst/tuple/apply.hpp>

#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <functional>
#include <string>
#include <tuple>

BOOST_AUTO_TEST_SUITE(apply)
    BOOST_AUTO_TEST_CASE(applies_function_to_elements_of_unwrapped_tuple)
    {
        auto a = burst::apply(std::plus<>{});
        auto r = a(std::make_tuple(1, 2));
        BOOST_CHECK_EQUAL(r, 3);
    }

    struct dummy
    {
        dummy ()
        {
            ++instances_count;
        }

        dummy (const dummy &)
        {
            ++instances_count;
        }

        dummy (dummy &&)
        {
            ++instances_count;
        }

        ~dummy ()
        {
            --instances_count;
        }

        static int instances_count;
    };

    int dummy::instances_count = 0;

    BOOST_AUTO_TEST_CASE(passed_function_is_stored_inside)
    {
        const auto old_instances_count = dummy::instances_count;

        auto a = burst::apply(dummy{});
        static_cast<void>(a);

        BOOST_CHECK_EQUAL(dummy::instances_count, old_instances_count + 1);
    }

    BOOST_AUTO_TEST_CASE(function_is_not_stored_if_passed_by_ref)
    {
        auto d = dummy{};
        const auto old_instances_count = dummy::instances_count;

        auto a = burst::apply(std::ref(d));
        static_cast<void>(a);

        BOOST_CHECK_EQUAL(dummy::instances_count, old_instances_count);
    }

    struct const_lvalue_caller
    {
        const_lvalue_caller (std::size_t & calls):
            calls(calls)
        {
        }

        template <typename ... T>
        auto operator () (T...) const &
        {
            ++calls;
            return true;
        }

        template <typename ... T>
        auto operator () (T...) & = delete;

        template <typename ... T>
        auto operator () (T...) && = delete;

        std::size_t & calls;
    };

    BOOST_AUTO_TEST_CASE(stored_function_invokes_as_const_lvalue_when_apply_is_const_lvalue)
    {
        auto calls = std::size_t{0};
        const auto a = burst::apply(const_lvalue_caller{calls});

        a(std::make_tuple(1, "qwe"));

        BOOST_CHECK_EQUAL(calls, 1);
    }

    struct lvalue_caller
    {
        lvalue_caller (std::size_t & calls):
            calls(calls)
        {
        }

        template <typename ... T>
        auto operator () (T...) const & = delete;

        template <typename ... T>
        auto operator () (T...) &
        {
            ++calls;
            return true;
        }

        template <typename ... T>
        auto operator () (T...) && = delete;

        std::size_t & calls;
    };

    BOOST_AUTO_TEST_CASE(stored_function_invokes_as_lvalue_when_apply_is_lvalue)
    {
        auto calls = std::size_t{0};
        auto a = burst::apply(lvalue_caller{calls});

        a(std::make_tuple(1, "qwe"));

        BOOST_CHECK_EQUAL(calls, 1);
    }

    struct rvalue_caller
    {
        rvalue_caller (std::size_t & calls):
            calls(calls)
        {
        }

        template <typename ... T>
        auto operator () (T...) const & = delete;

        template <typename ... T>
        auto operator () (T...) & = delete;

        template <typename ... T>
        auto operator () (T...) &&
        {
            ++calls;
            return true;
        }

        std::size_t & calls;
    };

    BOOST_AUTO_TEST_CASE(stored_function_invokes_as_rvalue_when_apply_is_rvalue)
    {
        auto calls = std::size_t{0};
        auto c = rvalue_caller{calls};

        burst::apply(c)(std::make_tuple(1, "qwe"));

        BOOST_CHECK_EQUAL(calls, 1);
    }

    BOOST_AUTO_TEST_CASE(referenced_function_invokes_as_lvalue)
    {
        auto calls = std::size_t{0};
        auto f = lvalue_caller{calls};

        burst::apply(std::ref(f))(std::make_tuple(1, "qwe"));

        BOOST_CHECK_EQUAL(calls, 1);
    }

    BOOST_AUTO_TEST_CASE(const_referenced_function_invokes_as_const_lvalue)
    {
        auto calls = std::size_t{0};
        const auto f = const_lvalue_caller{calls};

        burst::apply(std::ref(f))(std::make_tuple(1, "qwe"));

        BOOST_CHECK_EQUAL(calls, 1);
    }

    BOOST_AUTO_TEST_CASE(is_a_constexpr_function)
    {
        constexpr auto a = burst::apply(std::multiplies<>{});
        constexpr auto r = a(std::make_tuple(4, 5));
        BOOST_CHECK_EQUAL(r, 20);
    }
BOOST_AUTO_TEST_SUITE_END()
