# sf::VertexArray iterator

`VertexArray_iterator` serves the purpose of iterating through the vertices of the `sf::VertexArray` container.

The term `VertexArray_iterator` in this file refers to a class of that name but also to its sibling classes `VertexArray_const_iterator`, `VertexArray_reverse_iterator` and ``VertexArray_const_reverse_iterator`.


## Features

`VertexArray_iterator` supports range-based for loop on `sf::VertexArray` and
iterator arithmetic (`std::advance`, `std::distance`, increment...).


## Implementation

To achieve the wanted features, this header
    - injects `begin`, `end`, `rbegin`, `cbegin`, `crbegin` etc. in `sf` namespace
    - specializes the `iterator_traits` class template in `std` namespace

`VertexArray_iterator` types satisfy the `LegacyRandomAccessIterator` C++ named requirement.
