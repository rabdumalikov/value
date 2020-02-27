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
#include <queue>

int main()
{  
    /* Helper function 'value' allow get extract value from 'queue' */

    std::queue< int > q; q.push( 11 );

    assert( value( q )  == 11 );
    assert( q.empty() );
}