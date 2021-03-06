#include <burst/container/make_forward_list.hpp>
#include <burst/range/cache_one.hpp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cache_one)
    BOOST_AUTO_TEST_CASE(caches_each_dereferenced_value_of_a_range)
    {
        const auto items = burst::make_forward_list({1.1, 2.2, 3.3});

        auto call_count = 0ul;
        const auto count_calls =
            [& call_count] (auto x)
            {
                ++call_count;
                return x;
            };

        auto cached_range = burst::cache_one(items | boost::adaptors::transformed(count_calls));

        BOOST_REQUIRE_EQUAL(call_count, 0);

        cached_range.front();
        cached_range.front();
        BOOST_CHECK_EQUAL(call_count, 1);

        cached_range.advance_begin(2);
        cached_range.front();
        cached_range.front();
        BOOST_CHECK_EQUAL(call_count, 2);

        cached_range.advance_begin(1);
        BOOST_CHECK_EQUAL(call_count, 2);

        BOOST_REQUIRE(cached_range.empty());
    }
BOOST_AUTO_TEST_SUITE_END()
