/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    VertexArray_iterator
    VertexArray_const_iterator
    VertexArray_reverse_iterator
    VertexArray_const_reverse_iterator

    This file defines the above iterator types,
    specializes `std::iterator_traits` for those types,
    and injects begin() and end() functions in the `sf` namespace
    to enable range-based for loop for `sf::VertexArray`.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    class VertexArray_iterator_interface
    exposes the (mostly) common API for the various iterator types.

    @param  Concrete_it the concrete iterator child type
    @param  is_const_iterator a boolean specifying the constness of the concrete
            iterator
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template <typename Concrete_it, bool is_const_iterator>
class VertexArray_iterator_interface
{
public:
    using value_type = std::conditional_t<is_const_iterator,
        sf::Vertex const,
        sf::Vertex
    >;
    using difference_type   = std::ptrdiff_t;
    using pointer           = std::add_pointer_t<value_type>;
    using reference         = std::add_lvalue_reference_t<value_type>;
    using iterator_category = std::bidirectional_iterator_tag;

protected:
    using array_pointer_t = std::conditional_t<is_const_iterator,
        sf::VertexArray const*,
        sf::VertexArray*
    >;

    array_pointer_t         m_array;
    mutable std::size_t     m_index;

protected:
    VertexArray_iterator_interface(array_pointer_t, std::size_t) noexcept;

public:
    reference               operator[](std::size_t) noexcept;
    reference               operator[](std::size_t) const noexcept;

    reference               operator*() noexcept;
    reference               operator*() const noexcept;
    pointer                 operator->() noexcept;
    pointer                 operator->() const noexcept;

    Concrete_it&            operator++() noexcept;
    Concrete_it&            operator++(int) noexcept;
    Concrete_it&            operator--() noexcept;
    Concrete_it&            operator--(int) noexcept;
    Concrete_it&            operator+=(int) noexcept;
    Concrete_it&            operator-=(int) noexcept;
    Concrete_it&            operator+=(Concrete_it const&) noexcept;
    Concrete_it&            operator-=(Concrete_it const&) noexcept;

    friend Concrete_it operator+(Concrete_it const& it, int n)
    {
        return {*it.m_array, it.m_index+n};
    }

    friend Concrete_it operator-(Concrete_it const& it, int n)
    {
        return {*it.m_array, it.m_index-n};
    }

    friend difference_type operator+(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index + rhs.m_index;
    }

    friend difference_type operator-(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index - rhs.m_index;
    }

    friend bool operator==(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_array == rhs.m_array
            && lhs.m_index == rhs.m_index;
    }

    friend bool operator!=(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_array != rhs.m_array
            || lhs.m_index != rhs.m_index;
    }

    friend bool operator<(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index < rhs.m_index;
    }

    friend bool operator>(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index < rhs.m_index;
    }

    friend bool operator<=(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index <= rhs.m_index;
    }

    friend bool operator>=(Concrete_it const& lhs, Concrete_it const& rhs)
    {
        return lhs.m_index >= rhs.m_index;
    }

};




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Concrete iterator classes inheriting above interface
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
struct VertexArray_const_iterator
    : public VertexArray_iterator_interface<VertexArray_const_iterator, true>
{
    VertexArray_const_iterator(sf::VertexArray const& array, std::size_t idx)
    noexcept
        : VertexArray_iterator_interface{&array, idx}
    {}
};




struct VertexArray_iterator
    : public VertexArray_iterator_interface<VertexArray_iterator, false>
{
    VertexArray_iterator(sf::VertexArray& array, std::size_t idx) noexcept
        : VertexArray_iterator_interface{&array, idx}
    {}

    // need implicit conversion
    operator VertexArray_const_iterator() const noexcept {
        return {*m_array, m_index};
    }
};




struct VertexArray_const_reverse_iterator
    : public VertexArray_iterator_interface<
        VertexArray_const_reverse_iterator,
        true>
{
    VertexArray_const_reverse_iterator(sf::VertexArray const& array, std::size_t idx)
    noexcept
        : VertexArray_iterator_interface{&array, idx}
    {}

    using reference = typename VertexArray_iterator_interface::reference;
    using pointer   = typename VertexArray_iterator_interface::pointer;

    reference operator*() const noexcept
    {
        return (*m_array)[m_array->getVertexCount() - 1 - m_index];
    }

    pointer operator->() const noexcept
    {
        return &((*m_array)[m_array->getVertexCount() - 1 - m_index]);
    }

};




struct VertexArray_reverse_iterator
    : public VertexArray_iterator_interface<VertexArray_reverse_iterator, false>
{
    VertexArray_reverse_iterator(sf::VertexArray& array, std::size_t idx)
    noexcept
        : VertexArray_iterator_interface{&array, idx}
    {}

    using reference = typename VertexArray_iterator_interface::reference;
    using pointer   = typename VertexArray_iterator_interface::pointer;

    operator VertexArray_const_reverse_iterator() const noexcept
    {
        return {*m_array, m_index};
    }

    reference operator*() noexcept
    {
        return (*m_array)[m_array->getVertexCount() - 1 - m_index];
    }

    reference operator*() const noexcept
    {
        return (*m_array)[m_array->getVertexCount() - 1 - m_index];
    }

    pointer operator->() noexcept
    {
        return &((*m_array)[m_array->getVertexCount() - 1 - m_index]);
    }

    pointer operator->() const noexcept
    {
        return &((*m_array)[m_array->getVertexCount() - 1 - m_index]);
    }


};



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    specialize std::iterator_traits
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace std
{
template <> struct iterator_traits<VertexArray_iterator> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = sf::Vertex;
    using pointer           = sf::Vertex*;
    using reference         = sf::Vertex&;
    using iterator_category = std::random_access_iterator_tag;
};



template <> struct iterator_traits<VertexArray_const_iterator> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = sf::Vertex;
    using pointer           = sf::Vertex const*;
    using reference         = sf::Vertex const&;
    using iterator_category = std::random_access_iterator_tag;
};




template <> struct iterator_traits<VertexArray_reverse_iterator> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = sf::Vertex;
    using pointer           = sf::Vertex*;
    using reference         = sf::Vertex&;
    using iterator_category = std::random_access_iterator_tag;
};



template <> struct iterator_traits<VertexArray_const_reverse_iterator> {
    using difference_type   = std::ptrdiff_t;
    using value_type        = sf::Vertex;
    using pointer           = sf::Vertex const*;
    using reference         = sf::Vertex const&;
    using iterator_category = std::random_access_iterator_tag;
};


} // namespace std




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    begin() and end(), and reverse versions
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace sf
{
VertexArray_iterator                begin(sf::VertexArray&) noexcept;
VertexArray_iterator                end(sf::VertexArray&) noexcept;
VertexArray_const_iterator          begin(sf::VertexArray const&) noexcept;
VertexArray_const_iterator          end(sf::VertexArray const&) noexcept;
VertexArray_const_iterator          cbegin(sf::VertexArray const&) noexcept;
VertexArray_const_iterator          cend(sf::VertexArray const&) noexcept;
VertexArray_reverse_iterator        rbegin(sf::VertexArray&) noexcept;
VertexArray_reverse_iterator        rend(sf::VertexArray&) noexcept;
VertexArray_const_reverse_iterator  rbegin(sf::VertexArray const&) noexcept;
VertexArray_const_reverse_iterator  rend(sf::VertexArray const&) noexcept;
VertexArray_const_reverse_iterator  crbegin(sf::VertexArray const&) noexcept;
VertexArray_const_reverse_iterator  crend(sf::VertexArray const&) noexcept;




VertexArray_iterator begin(sf::VertexArray& va) noexcept
{
    return {va, 0};
}




VertexArray_iterator end(sf::VertexArray& va) noexcept
{
    return {va, va.getVertexCount()};
}




VertexArray_const_iterator begin(sf::VertexArray const& va) noexcept
{
    return {va, 0};
}




VertexArray_const_iterator end(sf::VertexArray const& va) noexcept
{
    return {va, va.getVertexCount()};
}




VertexArray_const_iterator cbegin(sf::VertexArray const& va) noexcept
{
    return {va, 0};
}




VertexArray_const_iterator cend(sf::VertexArray const& va) noexcept
{
    return {va, va.getVertexCount()};
}




VertexArray_reverse_iterator rbegin(sf::VertexArray& va) noexcept
{
    return {va, 0};
}




VertexArray_reverse_iterator rend(sf::VertexArray& va) noexcept
{
    return {va, va.getVertexCount()};
}




VertexArray_const_reverse_iterator rbegin(sf::VertexArray const& va) noexcept
{
    return {va, 0};
}




VertexArray_const_reverse_iterator rend(sf::VertexArray const& va) noexcept
{
    return {va, va.getVertexCount()};
}




VertexArray_const_reverse_iterator crbegin(sf::VertexArray const& va) noexcept
{
    return {va, 0};
}




VertexArray_const_reverse_iterator crend(sf::VertexArray const& va) noexcept
{
    return {va, va.getVertexCount()};
}



} // namespace sf




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    iterator interface implementation
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template <typename C, bool B>
VertexArray_iterator_interface<C,B>::
VertexArray_iterator_interface(array_pointer_t pointer, std::size_t index)
noexcept
    : m_array   {pointer}
    , m_index   {index}
{
}




template <typename Concrete_it, bool B>
auto VertexArray_iterator_interface<Concrete_it,B>::operator[](std::size_t idx) noexcept
    -> typename VertexArray_iterator_interface<Concrete_it,B>::reference
{
    return *(Concrete_it{*m_array, idx});
}




template <typename Concrete_it, bool B>
auto VertexArray_iterator_interface<Concrete_it,B>::operator[](std::size_t idx)
const noexcept
    -> typename VertexArray_iterator_interface<Concrete_it,B>::reference
{
    return *(Concrete_it{*m_array, idx});
}




template <typename Concrete_it, bool B>
auto VertexArray_iterator_interface<Concrete_it,B>::operator*() noexcept
    -> typename VertexArray_iterator_interface<Concrete_it,B>::reference
{
    return (*m_array)[m_index];
}




template <typename Concrete_it, bool B>
auto VertexArray_iterator_interface<Concrete_it,B>::operator*()
const noexcept
    -> typename VertexArray_iterator_interface<Concrete_it,B>::reference
{
    return (*m_array)[m_index];
}




template <typename Concrete_it, bool B>
auto VertexArray_iterator_interface<Concrete_it,B>::operator->() noexcept
    -> typename VertexArray_iterator_interface<Concrete_it,B>::pointer
{
    return &(*m_array)[m_index];
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator++()
noexcept
{
    ++m_index;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator++(int)
noexcept
{
    ++m_index;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator--()
noexcept
{
    ++m_index;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator--(int)
noexcept
{
    ++m_index;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator+=(int n) noexcept
{
    m_index += n;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator-=(int n) noexcept
{
    m_index -= n;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator+=(Concrete_it const& rhs) noexcept
{
    m_index += rhs.m_index;
    return (static_cast<Concrete_it&>(*this));
}




template <typename Concrete_it, bool B>
Concrete_it& VertexArray_iterator_interface<Concrete_it,B>::operator-=(Concrete_it const& rhs) noexcept
{
    m_index -= rhs.m_index;
    return (static_cast<Concrete_it&>(*this));
}
