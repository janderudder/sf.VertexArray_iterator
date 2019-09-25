#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <iostream>

#include "VertexArray_iterator.hpp"
#include "test-tool.hpp"


void test_iterator();
void test_const_iterator();


int main()
{
    std::boolalpha(std::cout);

    std::cerr << "test sf::VertexArray's iterator\n";
    test_iterator();

    std::cerr << "test sf::VertexArray's const_iterator\n";
    test_const_iterator();
}




void test_iterator()
{
    sf::VertexArray va;

    VertexArray_iterator        beg_it1 = {va, 0};
    auto                        beg_it2 = sf::begin(va);
    VertexArray_iterator        end_it1 = {va, 0};
    auto                        end_it2 = sf::end(va);

    std::cerr << "--- Array is empty\n";
    ENSURE(beg_it1 == beg_it2, "two begin of same VA are equal");
    ENSURE(end_it1 == end_it2, "two end of same VA are equal");
    ENSURE((VertexArray_iterator{va, 0}) == sf::begin(va), "manual begin and sf::begin() equal");
    ENSURE((VertexArray_iterator{va, 0} == sf::end(va)), "manual end and sf::end() equal");
    ENSURE(beg_it2 == end_it1, "empty VA begin and end are equal");
    ENSURE(sf::end(va) - sf::begin(va) == 0, "end - begin should be 0 when array empty");
    ENSURE((sf::end(va) - sf::end(va)) == 0, "subtract it to itself should be 0");
    ENSURE((sf::begin(va) - sf::begin(va)) == 0, "subtract it to itself should be 0");
    ENSURE((++sf::begin(va) - ++sf::begin(va)) == 0, "subtract it to itself should be 0");
    ENSURE(++sf::begin(va) == (sf::begin(va) + 1), "pre-increment begin == begin+1");
    ENSURE([&]{ auto it=sf::begin(va); it++; return it == (sf::begin(va) + 1);}(),
        "post-increment begin == begin+1");
    ENSURE((sf::begin(va) + (std::size_t)(2)) == sf::begin(va)+2, "can add it and ULL");
    ENSURE((sf::begin(va) + (int)(3)) == sf::begin(va)+3, "can add it and int");

    ENSURE((sf::begin(va) + va.getVertexCount()) == sf::end(va), "begin + (VA's size) should be equal to end");
    ENSURE((sf::end(va) - va.getVertexCount()) == sf::begin(va), "end - (VA's size) should be equal to begin");

    ENSURE(std::distance(beg_it1, end_it2) == 0, "empty VA begin and end: no distance");
    ENSURE(std::distance(sf::begin(va), sf::end(va)) == 0, "empty VA sf::begin() and sf::end(): no distance");
    ENSURE(std::distance(++beg_it1, end_it2) == -1, "++begin and end: distance == -1");
    ENSURE(std::distance(sf::begin(va), sf::end(va)+1) == 1, "empty VA sf::begin() and sf::end()+1: distance 1");

    va.append({}); va[0].position.x++;
    va.append({}); va[1].position.x++;
    va.append({}); va[2].position.x++;
    std::cerr << "\n--- Array has 3 elements now\n";

    for (auto const& vertex : std::as_const(va))
        ENSURE(vertex.position.x == 1, "const iteration: position.x should be 1");

    for (auto& vertex : va)
        ENSURE(++vertex.position.x == 2, "mut iteration: position.x should increment to 2");

    ENSURE((sf::end(va) - sf::end(va)) == 0, "subtract it to itself should still be 0");
    ENSURE((sf::begin(va) - sf::begin(va)) == 0, "subtract it to itself should still be 0");
    ENSURE((++sf::begin(va) - ++sf::begin(va)) == 0, "subtract it to itself should still be 0");

    ENSURE((sf::begin(va) + va.getVertexCount()) == sf::end(va), "begin + (VA's size) should still be equal to end");
    ENSURE((sf::end(va) - va.getVertexCount()) == sf::begin(va), "end - (VA's size) should still be equal to begin");
    ENSURE(sf::end(va) - sf::begin(va) == (std::ptrdiff_t)va.getVertexCount(), "end - begin should be VA's size when array empty");
    ENSURE((unsigned)std::distance(sf::begin(va), sf::end(va)) == va.getVertexCount(), "distance should be == VA's size");

    ENSURE(([&]
    {
        std::size_t count = 0;
        for (auto const& v : std::as_const(va)) {
            ++count;
        }
        return count == va.getVertexCount();
    })(), "range-for loop iterates (VA's size) times");
}




void test_const_iterator()
{
    sf::VertexArray va;

    VertexArray_const_iterator  beg_cit1 = {va, 0};
    auto                        beg_cit2 = sf::cbegin(va);
    VertexArray_const_iterator  end_cit1 = {va, 0};
    auto                        end_cit2 = sf::cend(va);

    std::cerr << "--- Array is empty\n";
    ENSURE(beg_cit1 == beg_cit2, "two begin of same VA are equal");
    ENSURE(end_cit1 == end_cit2, "two end of same VA are equal");
    ENSURE((VertexArray_const_iterator{va, 0}) == sf::cbegin(va), "manual begin and sf::begin() equal");
    ENSURE((VertexArray_const_iterator{va, 0} == sf::cend(va)), "manual end and sf::end() equal");
    ENSURE(beg_cit2 == end_cit1, "empty VA begin and end are equal");
    ENSURE(sf::cend(va) - sf::cbegin(va) == 0, "end - begin should be 0 when array empty");
    ENSURE((sf::cend(va) - sf::cend(va)) == 0, "subtract it to itself should be 0");
    ENSURE((sf::cbegin(va) - sf::cbegin(va)) == 0, "subtract it to itself should be 0");
    ENSURE((++sf::cbegin(va) - ++sf::cbegin(va)) == 0, "subtract it to itself should be 0");
    ENSURE(++sf::cbegin(va) == (sf::cbegin(va) + 1), "pre-increment begin == begin+1");
    ENSURE([&]{ auto it=sf::cbegin(va); it++; return it == (sf::cbegin(va) + 1);}(),
        "post-increment begin == begin+1");
    ENSURE((sf::cbegin(va) + (std::size_t)(2)) == sf::cbegin(va)+2, "can add it and ULL");
    ENSURE((sf::cbegin(va) + (int)(3)) == sf::cbegin(va)+3, "can add it and int");

    ENSURE((sf::cbegin(va) + va.getVertexCount()) == sf::cend(va), "begin + (VA's size) should be equal to end");
    ENSURE((sf::cend(va) - va.getVertexCount()) == sf::cbegin(va), "end - (VA's size) should be equal to begin");

    ENSURE(std::distance(beg_cit1, end_cit2) == 0, "empty VA begin and end: no distance");
    ENSURE(std::distance(sf::cbegin(va), sf::cend(va)) == 0, "empty VA sf::begin() and sf::end(): no distance");
    ENSURE(std::distance(++beg_cit1, end_cit2) == -1, "++begin and end: distance == -1");
    ENSURE(std::distance(sf::cbegin(va), sf::cend(va)+1) == 1, "empty VA sf::begin() and sf::end()+1: distance 1");

    va.append({}); va[0].position.x++;
    va.append({}); va[1].position.x++;
    va.append({}); va[2].position.x++;
    std::cerr << "\n--- Array has 3 elements now\n";

    for (auto const& vertex : std::as_const(va))
        ENSURE(vertex.position.x == 1, "const iteration: position.x should be 1");

    for (auto& vertex : va)
        ENSURE(++vertex.position.x == 2, "mut iteration: position.x should increment to 2");

    ENSURE((sf::cend(va) - sf::cend(va)) == 0, "subtract it to itself should still be 0");
    ENSURE((sf::cbegin(va) - sf::cbegin(va)) == 0, "subtract it to itself should still be 0");
    ENSURE((++sf::cbegin(va) - ++sf::cbegin(va)) == 0, "subtract it to itself should still be 0");

    ENSURE((sf::cbegin(va) + va.getVertexCount()) == sf::cend(va), "begin + (VA's size) should still be equal to end");
    ENSURE((sf::cend(va) - va.getVertexCount()) == sf::cbegin(va), "end - (VA's size) should still be equal to begin");
    ENSURE(sf::cend(va) - sf::cbegin(va) == (std::ptrdiff_t)va.getVertexCount(), "end - begin should be VA's size when array empty");
    ENSURE((unsigned)std::distance(sf::cbegin(va), sf::cend(va)) == va.getVertexCount(), "distance should be == VA's size");

    ENSURE(([&]
    {
        std::size_t count = 0;
        for (auto const& v : std::as_const(va)) {
            ++count;
        }
        return count == va.getVertexCount();
    })(), "range-for loop iterates (VA's size) times");
}
