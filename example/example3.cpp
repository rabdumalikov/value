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
#include <boost/variant.hpp>

int main()
{  
    /* Helper function 'value' allow get access to variant value via 'indexes' instead of 'types' */

    boost::variant< int, std::string > var = std::string{ "Hi" };

    assert( value< 1 >( var )  == "Hi" );
}