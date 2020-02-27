//  Copyright (c) 2020 Rustam Abdumalikov
//
//  "value" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include "value/value.hpp"
#include "memory"
#include <iostream>

int main()
{  
    /* Helper function 'value' allow to work with container/array consistently i.e. via free function */

    std::vector< int > v{ 1, 2, 3, 4, 5 };
    for( auto iter = std::begin( v ); iter != std::end( v ); iter = std::next( iter ) )
    {
        assert( value( iter ) == *iter );
    }

    int arr[] = { 1, 2, 3, 4, 5, 6 };
    for( auto iter = std::begin( arr ); iter != std::end( arr ); iter = std::next( iter ) )
    {
        std::cout << value( iter ) << std::endl;
        assert( value( iter ) == *iter );
    }
}