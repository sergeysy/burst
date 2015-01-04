#ifndef THRUST_ITERATOR_JOIN_ITERATOR_HPP
#define THRUST_ITERATOR_JOIN_ITERATOR_HPP

#include <iterator>

#include <thrust/iterator/detail/join_iterator.hpp>
#include <thrust/iterator/end_tag.hpp>

namespace thrust
{
    //!     Итератор склейки.
    /*!
            Предназначен для склейки нескольких диапазонов одного типа "на лету", то есть без
        использования дополнительной памяти для хранения результирующего диапазона.
            Принимает на вход набор диапазонов и перемещается по ним последовательно, то есть после
        последнего элемента i-го диапазона сразу переходит к первому элементу (i + 1)-го.
            Полученный в результате склейки диапазон изменяем. То есть из итератора можно не только
        прочитать значения, но можно и записать в него. В результате записи в итератор будет
        изменено значение в исходном хранилище.

        \tparam Range
            Тип принимаемого на вход диапазона.
            Если принимаемый диапазон произвольного доступа, то итератор склейки будет итератором
            произвольного доступа, правда, с одной оговоркой: продвижение итератора на n шагов
            будет происходить не за O(1), а за O(количества склеенных списков).
            Если принимаемый диапазон не произвольного доступа, а любой другой, то итератор склейки
            будет обычным однонаправленным итератором.

            Алгоритм работы.

        I. Итератор произвольного доступа.
            1. Входные диапазоны складываются в массив.
            2. Заводятся два индекса: индекс текущего диапазона в массиве и индекс в текущем
               диапазоне.
            3. При продвижении итератора в какую-либо сторону изменяются только эти два индекса, а
               сами диапазоны остаются нетронутыми.

        II. Все остальные итераторы.
            1. Входные диапазоны складываются в массив в обратном порядке.
            2. Каждый раз, когда требуется перейти к новому элементу, последний диапазон в массиве
               (первый в исходном наборе) продвигается ровно на один элемент вперёд.
               а. Если диапазон опустел, то он выбрасывается из массива.
               б. Если не опустел, то переход завершён.
            3. Пустой массив диапазонов означает, что пройдены все элементы всех входных
               диапазонов.
     */
    template <typename Range>
    class join_iterator: public detail::join_iterator_base
                                <
                                    Range,
                                    typename std::iterator_traits<typename Range::iterator>::iterator_category
                                >
    {
    private:
        typedef detail::join_iterator_base
        <
            Range,
            typename std::iterator_traits<typename Range::iterator>::iterator_category
        >
        base_type;

    public:
        template <typename BidirectionalRange>
        explicit join_iterator (const BidirectionalRange & ranges):
            base_type(ranges)
        {
        }

        template <typename BidirectionalRange>
        join_iterator (const BidirectionalRange & ranges, iterator::end_tag_t):
            base_type(ranges, iterator::end_tag)
        {
        }

        join_iterator () = default;
    };

    //!     Функция для создания итератора склейки.
    /*!
            Принимает на вход набор диапазонов, которые нужно склеить.
            Возвращает итератор на первый элемент склеенного списка, который является первым
        элементом первого из входных диапазонов.
     */
    template <typename RangeRange>
    join_iterator<typename RangeRange::value_type> make_join_iterator (const RangeRange & ranges)
    {
        return join_iterator<typename RangeRange::value_type>(ranges);
    }

    //!     Функция для создания итератора на конец склейки.
    /*!
            Принимает на вход набор диапазонов, который не используется, а нужен только для
        автоматического вывода типа итератора.
            Возвращает итератор на конец склеенного списка.
     */
    template <typename RangeRange>
    join_iterator<typename RangeRange::value_type> make_join_iterator (const RangeRange & ranges, iterator::end_tag_t)
    {
        return join_iterator<typename RangeRange::value_type>(ranges, iterator::end_tag);
    }
} // namespace thrust

#endif // THRUST_ITERATOR_JOIN_ITERATOR_HPP
