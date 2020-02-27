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
#include <boost/optional.hpp>

int main()
{  
    /* Helper function 'value' allow uniformally get access to value */

    int i = 28;

    std::reference_wrapper< int > ref = i;
    boost::optional< int > opt = i;
    int * ptr = &i;
    auto pair = std::make_pair( 1, i );

    assert( value( ref )  == i );
    assert( value( opt )  == i );
    assert( value( ptr )  == i );
    assert( value( pair ) == i );
}