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
    /* Helper function 'recursive_value' allow to handle multiple indirections.
      'recursive_value' recognize following indirections 
       operator-'*' method-'get' 'std::pair', the former will perform indirections until
       it find underlying type without supported properties. 
       
       You can use 'recursive_value_trait_t' to find out underlying type which will 
       terminate indirection.       
     */

    int i = 1;
    int j = 2;
    int k = 3;

    std::vector< int* > v{ &i, &j, &k };
    for( auto iter = std::begin( v ); iter != std::end( v ); ++iter )
    {
        assert( recursive_value( iter ) == **iter );
    }



    std::vector< std::shared_ptr< int > > vsp{ 
        std::make_shared< int >( i ), 
        std::make_shared< int >( j ), 
        std::make_shared< int >( k ) };

    for( auto iter = std::begin( vsp ); iter != std::end( vsp ); ++iter )
    {
        assert( recursive_value( iter ) == **iter );
    }



    std::vector< std::reference_wrapper< int > > vrf{ 
        std::ref( i ), 
        std::ref( j ), 
        std::ref( k ) };

    for( auto iter = std::begin( vrf ); iter != std::end( vrf ); ++iter )
    {
        assert( recursive_value( iter ) == ( *iter ).get() );
    }



    std::vector< boost::optional< int > > vopt{ 
        boost::make_optional( i ), 
        boost::make_optional( j ), 
        boost::make_optional( k ) };

    for( auto iter = std::begin( vopt ); iter != std::end( vopt ); ++iter )
    {
        assert( recursive_value( iter ) == **iter );
        assert( recursive_value( iter ) == ( *iter ).get() );
    }
}