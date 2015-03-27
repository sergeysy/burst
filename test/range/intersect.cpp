#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include <boost/range/irange.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <burst/range/intersect.hpp>

BOOST_AUTO_TEST_SUITE(intersect)
    BOOST_AUTO_TEST_CASE(intersecting_empty_ranges_results_empty_range)
    {
        std::vector<int> first;
        std::vector<int> second;
        auto ranges = {boost::make_iterator_range(first), boost::make_iterator_range(second)};

        auto intersected_range = burst::intersect(ranges);

        BOOST_CHECK(intersected_range.empty());
    }

    BOOST_AUTO_TEST_CASE(intersecting_one_range_results_the_same_range)
    {
        auto intersected_range = burst::intersect({boost::irange(1, 5)});

        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(boost::irange(1, 5)), std::end(boost::irange(1, 5)),
            std::begin(intersected_range), std::end(intersected_range)
        );
    }

    BOOST_AUTO_TEST_CASE(intersecting_equal_ranges_results_range_equal_to_both_of_them)
    {
        auto first = {4, 5, 6, 7};
        auto second = first;
        auto ranges = {boost::make_iterator_range(first), boost::make_iterator_range(second)};

        auto intersected_range = burst::intersect(ranges);

        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(first), std::end(first)
        );
        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(second), std::end(second)
        );
    }

    BOOST_AUTO_TEST_CASE(intersecting_nested_ranges_results_shortest_of_them)
    {
        std::string  long_range("abcdef");
        std::string short_range("cde");
        auto ranges = {boost::make_iterator_range(long_range), boost::make_iterator_range(short_range)};

        auto intersected_range = burst::intersect(ranges);

        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(short_range), std::end(short_range)
        );
    }

    BOOST_AUTO_TEST_CASE(intersecting_saw_toothed_ranges_results_empty_range)
    {
        std::list<char> first{'h', 'f', 'd', 'b'};
        std::list<char> second{'g', 'e', 'c', 'a'};
        auto ranges = {boost::make_iterator_range(first), boost::make_iterator_range(second)};

        auto intersected_range = burst::intersect(ranges, std::greater<char>());

        BOOST_CHECK(intersected_range.empty());
    }

    BOOST_AUTO_TEST_CASE(intersecting_two_overlaying_ranges_results_overlayed_part)
    {
        auto  first = {3, 2, 1};
        auto second = {4, 3, 2};
        auto ranges = {boost::make_iterator_range(first), boost::make_iterator_range(second)};

        auto intersected_range = burst::intersect(ranges, std::greater<int>());

        std::vector<int> expected_collection{3, 2};
        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(expected_collection), std::end(expected_collection)
        );
    }

    BOOST_AUTO_TEST_CASE(intersecting_pairwise_overlaying_but_mutual_disjoint_ranges_results_empty_range)
    {
        auto  first = {1, 2};
        auto second = {2, 3};
        auto  third = {3, 4};
        auto ranges =
        {
            boost::make_iterator_range(first),
            boost::make_iterator_range(second),
            boost::make_iterator_range(third)
        };

        auto intersected_range = burst::intersect(ranges);

        BOOST_CHECK(intersected_range.empty());
    }

    BOOST_AUTO_TEST_CASE(intersecting_several_consecutive_sorted_ranges_results_empty_range)
    {
        auto  first = {1, 2, 3};
        auto second = {4, 5, 6};
        auto  third = {7, 8, 9};
        auto ranges =
        {
            boost::make_iterator_range(first),
            boost::make_iterator_range(second),
            boost::make_iterator_range(third)
        };

        auto intersected_range = burst::intersect(ranges, std::less<int>());

        BOOST_CHECK(intersected_range.empty());
    }

    BOOST_AUTO_TEST_CASE(repeating_elements_do_not_produce_excess_matches)
    {
        auto  first = {1, 1, 1};
        auto second = {1, 1, 1};
        auto  third = {1, 1, 1};
        auto ranges =
        {
            boost::make_iterator_range(first),
            boost::make_iterator_range(second),
            boost::make_iterator_range(third)
        };

        auto intersected_range = burst::intersect(ranges, std::less<int>());

        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(first), std::end(first)
        );
    }

    BOOST_AUTO_TEST_CASE(intersect_function_accepts_inplace_initializer_list)
    {
        std::vector<int> natural{1, 2, 3, 4, 5, 6, 7};
        std::vector<int> prime{2, 3, 5, 7};
        std::vector<int> odd{1, 3, 5, 7, 9};

        auto intersected_range = burst::intersect
        ({
            boost::make_iterator_range(natural),
            boost::make_iterator_range(prime),
            boost::make_iterator_range(odd)
        });

        std::vector<int> expected{3, 5, 7};
        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(expected), std::end(expected)
        );
    }

    BOOST_AUTO_TEST_CASE(no_element_considered_twice)
    {
        auto  first = {0, 0, 1};
        auto second = {0, 1, 1};
        auto ranges = {boost::make_iterator_range(first), boost::make_iterator_range(second)};

        auto intersected_range = burst::intersect(ranges);

        auto expected_collection = {0, 1};
        BOOST_CHECK_EQUAL_COLLECTIONS
        (
            std::begin(intersected_range), std::end(intersected_range),
            std::begin(expected_collection), std::end(expected_collection)
        );
    }
BOOST_AUTO_TEST_SUITE_END()
