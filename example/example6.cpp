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
#include "value/adaptor_value.hpp"
#include "memory"
#include <iostream>
#include <boost/range/adaptor/indexed.hpp>

int main()
{  
    /* 
        It is possible to use 'recursive_value' with boost::range library.
     */

    int i = 1;
    int j = 2;
    int k = 3;

    int managed_by_hand_index = 0;

    std::vector< int* > vptr{ &i, &j, &k };
    for( auto val : vptr | boost::adaptors::valued )
    {
        assert( val == *vptr[ managed_by_hand_index++ ] );
    }


    managed_by_hand_index = 0;

    std::vector< std::shared_ptr< int > > vsp{ 
        std::make_shared< int >( i ), 
        std::make_shared< int >( j ), 
        std::make_shared< int >( k ) };

    for( auto val : boost::adaptors::value( vsp ) )
    {
        assert( val == *vsp[ managed_by_hand_index++ ] );
    }


    std::vector< std::reference_wrapper< int > > vrf{ 
        std::ref( i ), 
        std::ref( j ), 
        std::ref( k ) };

    for( auto val : boost::adaptors::value( vrf ) | boost::adaptors::indexed() )
    {
        assert( val.value() == vrf[ val.index() ].get() );
    }



    std::vector< boost::optional< int > > vopt{ 
        boost::make_optional( i ), 
        boost::make_optional( j ), 
        boost::make_optional( k ) };

    for( auto val : vopt | boost::adaptors::valued | boost::adaptors::indexed() )
    {
        assert( val.value() == *vopt[ val.index() ] );
        assert( val.value() ==  vopt[ val.index() ].get() );

    }
}