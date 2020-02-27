//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "gtest/gtest.h"
#include "adaptor_value.hpp"
#include "value.hpp"
#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <string>
#include <boost/range/adaptor/indexed.hpp>

TEST( value_utilities, compile_time_utilities )
{
    static_assert( is_std_pair< std::pair< int, int > >::value, "!!" );
    static_assert(!is_std_pair< std::map< int, int > >::value, "!!" );

    static_assert( is_std_pair_v< std::pair< int, int > >, "!!" );
    static_assert(!is_std_pair_v< std::map< int, int > >, "!!" );

    static_assert( is_std_reference_wrapper< std::reference_wrapper< int > >::value, "!!" );
    static_assert(!is_std_reference_wrapper< std::vector< int > >::value, "!!" );

    static_assert( is_std_reference_wrapper_v< std::reference_wrapper< int > >, "!!" );
    static_assert(!is_std_reference_wrapper_v< std::vector< int > >, "!!" );

    static_assert( has_get_method_v< std::reference_wrapper< int > >, "!!" );
    static_assert( has_get_method_v< boost::optional< int > >, "!!" );
    static_assert(!has_get_method_v< std::vector< int >::iterator >, "!!" );

    static_assert( has_deref_operator_v< std::vector< int >::iterator >, "!!" );
    static_assert( has_deref_operator_v< boost::optional< int > >, "!!" );
    static_assert(!has_deref_operator_v< std::reference_wrapper< int > >, "!!" );

    static_assert( std::is_same< deref_t< boost::optional< int > >, int&& >::value, "!!" );
    static_assert( std::is_same< deref_t< std::vector< int >::iterator >, int& >::value, "!!" );
    static_assert( std::is_same< deref_t< std::vector< std::pair< int, int > >::const_iterator >, const std::pair< int, int >& >::value, "!!" );
    static_assert( std::is_same< 
        std::remove_reference_t< deref_t< std::vector< std::pair< int, int > >::const_iterator > >&, const std::pair< int, int >& >::value, "!!" );

    static_assert( std::is_same< 
        const std::remove_reference_t< deref_t< std::vector< std::pair< int, int > >::const_iterator > >::second_type&, const int & >::value, "!!" );

    static_assert( std::is_reference< deref_t< std::vector< std::pair< int, int > >::const_iterator > >::value, "!!" );

    static_assert( std::is_same< deref_t< int >, int >::value, "!!" );

    static_assert( std::is_same< method_get_t< boost::optional< int > >, int& >::value, "!!" );
    static_assert( std::is_same< method_get_t< std::vector< double >::iterator >, std::vector< double >::iterator >::value, "!!" );
    static_assert( std::is_same< deref_t< std::pair< int, int > >, std::pair< int, int > >::value, "!!" );


    static_assert( std::is_same< type_by_index_t< 0, int, double, std::string >, int >::value, "!!" );
    static_assert( std::is_same< type_by_index_t< 1, int, double, std::string >, double >::value, "!!" );
    static_assert( std::is_same< type_by_index_t< 2, int, double, std::string >, std::string >::value, "!!" );

    static_assert( std::is_same< type_by_index_t< 0, int&, double*, std::string*& >, int& >::value, "!!" );
    static_assert( std::is_same< type_by_index_t< 1, int&, double*, std::string*& >, double* >::value, "!!" );
    static_assert( std::is_same< type_by_index_t< 2, int&, double*, std::string*& >, std::string*& >::value, "!!" );
}

TEST( value_functionality, return_types )
{
    static_assert( std::is_same< recursive_value_trait_t< std::reference_wrapper< int > >, int& >::value, "!!" );
    static_assert( std::is_same< recursive_value_trait_t< boost::optional< int > >, int&& >::value, "!!" );

    using d = const std::vector< std::pair< int, std::string > >;
    static_assert( std::is_same< recursive_value_trait_t< typename d::iterator >, std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( recursive_value( typename d::iterator() ) ), std::string & >::value, "!!" );

    std::vector< std::pair< int, std::string > > _d;
    static_assert( std::is_same< recursive_value_trait_t< decltype( _d.begin() ) >, std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( recursive_value( std::make_move_iterator( _d.begin() ) ) ), std::string && >::value, "!!" );

    static_assert( std::is_same< recursive_value_trait_t< decltype( _d.begin() ) >, std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( recursive_value( std::make_move_iterator( _d.begin() ) ) ), std::string && >::value, "!!" );

    d::iterator iter;
    static_assert( std::is_same< recursive_value_trait_t< decltype( iter ) >, std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( recursive_value( iter ) ), std::string& >::value, "!!" );

    d::const_iterator const_iter;
    static_assert( std::is_same< recursive_value_trait_t< decltype( const_iter ) >, const std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( recursive_value( const_iter ) ), const std::string& >::value, "!!" );

    static_assert( std::is_same< decltype( *std::declval< typename d::const_iterator >() ), const std::pair< int, std::string >& >::value, "!!" );



    /////////////////////////////////////////////////////////////////////

    auto ipair = std::make_pair( 9, 11 );
    static_assert( std::is_same< decltype( value( ipair ) ), int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( ipair ) ) ), int&& >::value, "!!" );

    const auto cipair = std::make_pair( 9, 11 );
    static_assert( std::is_same< decltype( value( cipair ) ), const int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( cipair ) ) ), const int& >::value, "!!" );

    auto pair = std::make_pair( 9, std::string{} );
    static_assert( std::is_same< decltype( value( pair ) ), std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( pair ) ) ), std::string&& >::value, "!!" );

    const auto cpair = std::make_pair( 9, std::string{} );
    static_assert( std::is_same< decltype( value( cpair ) ), const std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( cpair ) ) ), const std::string& >::value, "!!" );
 
    std::string str;
    auto ref = std::ref( str );
    static_assert( std::is_same< decltype( value( ref ) ), std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( ref ) ) ), std::string& >::value, "!!" );

    auto cref = std::cref( str );
    static_assert( std::is_same< decltype( value( cref ) ), const std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( cref ) ) ), const std::string& >::value, "!!" );

    auto opt = boost::make_optional( std::string{ "" } );
    static_assert( std::is_same< decltype( value( opt ) ), std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( opt ) ) ), std::string&& >::value, "!!" );

    const auto copt = boost::make_optional( std::string{ "" } );
    static_assert( std::is_same< decltype( value( copt ) ), const std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::move( copt ) ) ), const std::string& >::value, "!!" );

    auto var = boost::variant< int, double, std::string >( std::string{ "" } );
    static_assert( std::is_same< decltype( value< 2 >( var ) ), std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value< 2 >( std::move( var ) ) ), std::string&& >::value, "!!" );

    const auto cvar = boost::variant< int, double, std::string >( std::string{ "" } );
    static_assert( std::is_same< decltype( value< 2 >( cvar ) ), const std::string& >::value, "!!" );
    static_assert( std::is_same< decltype( value< 2 >( std::move( cvar ) ) ), const std::string& >::value, "!!" );

    auto que = std::queue< int >(); que.push( 10 );
    static_assert( std::is_same< decltype( value( que ) ), int >::value, "!!" );

    auto vec = std::vector< int >{ 1, 2, 3 };
    static_assert( std::is_same< decltype( value( vec.begin() ) ), int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( vec.cbegin() ) ), const int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::make_move_iterator( vec.begin() ) ) ), int&& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::make_move_iterator( vec.cbegin() ) ) ), const int&& >::value, "!!" );

    const auto cvec = std::vector< int >{ 1, 2, 3 };
    static_assert( std::is_same< decltype( value( cvec.begin() ) ), const int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( cvec.cbegin() ) ), const int& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::make_move_iterator( cvec.begin() ) ) ), const int&& >::value, "!!" );
    static_assert( std::is_same< decltype( value( std::make_move_iterator( cvec.cbegin() ) ) ), const int&& >::value, "!!" );
}

template< typename TRange >
static void check_range( TRange && range )
{
    for( auto val : range | boost::adaptors::valued | boost::adaptors::indexed() )
    { 
        ASSERT_EQ( val.value(), recursive_value( range[ val.index() ] ) );
    }

    for( auto val : boost::adaptors::value( range ) | boost::adaptors::indexed() )
    { 
        ASSERT_EQ( val.value(), recursive_value( range[ val.index() ] ) );
    }
}

TEST( recursive_value, recursive_value )
{
    {
        int i = 10;
        auto rw = std::ref( i );
        ASSERT_EQ( recursive_value( rw ), i );

        const auto crw = std::cref( i );
        ASSERT_EQ( recursive_value( crw ), i );    
    }

    {
        int   i = 10;
        int * j = &i;
        ASSERT_EQ( recursive_value( j ), i );

        int * cj = &i;
        ASSERT_EQ( recursive_value( cj ), i );    
    }
    
    {
        auto pr = std::make_pair( 10, std::string{"Hello"} );
        ASSERT_EQ( recursive_value( pr ), std::string{"Hello"} );

        const auto cpr = std::make_pair( 10, std::string{"Hello"} );
        ASSERT_EQ( recursive_value( cpr ), std::string{"Hello"} );

    }
    {
        boost::optional< int > opt = 101;
        ASSERT_EQ( recursive_value( opt ), 101 );

        const boost::optional< int > copt = 101;
        ASSERT_EQ( recursive_value( copt ), 101 );
    }
    {
        int i = 99;
        auto pr = std::make_pair( i, std::ref( i ) );
        ASSERT_EQ( recursive_value( pr ), 99 );

        const auto cpr = std::make_pair( i, std::cref( i ) );
        ASSERT_EQ( recursive_value( cpr ), 99 );
    }
    {
        boost::optional< std::string > opt = std::string{ "optional" };
        auto pr = std::make_pair( 1, std::ref( opt ) );
        ASSERT_EQ( recursive_value( pr ), "optional" );

        const boost::optional< std::string > copt = std::string{ "optional" };
        const auto cpr = std::make_pair( 1, std::cref( copt ) );
        ASSERT_EQ( recursive_value( cpr ), "optional" );
    }
    {
        boost::optional< std::string > opt = std::string{ "optional+vector" };
        auto pr = std::make_pair( 1, std::ref( opt ) );
        std::vector< decltype( pr ) > v{ pr };

        ASSERT_EQ( recursive_value( v.begin() ), "optional+vector" );


        const boost::optional< std::string > copt = std::string{ "optional+vector" };
        auto cpr = std::make_pair( 1, std::ref( copt ) );

        const std::vector< decltype( cpr ) > cv{ cpr };

        ASSERT_EQ( recursive_value( cv.begin() ), "optional+vector" );
        ASSERT_EQ( recursive_value( std::make_move_iterator( cv.begin() ) ), "optional+vector" );
    }
    {
        boost::optional< std::string > opt = std::string{ "optional+vector+pair" };
        auto pr = std::make_pair( 1, std::ref( opt ) );
        std::vector< decltype( pr ) > v{ pr };
        auto apr = std::make_pair( 1, v.begin() );

        ASSERT_EQ( recursive_value( apr ), "optional+vector+pair" );

        const boost::optional< std::string > copt = std::string{ "optional+vector+pair" };
        auto cpr = std::make_pair( 1, std::cref( copt ) );
        const std::vector< decltype( cpr ) > cv{ cpr };
        const auto capr = std::make_pair( 1, cv.begin() );

        ASSERT_EQ( recursive_value( capr ), "optional+vector+pair" );
    }
    {
        boost::optional< std::string > opt = std::string{ "optional+vector+pair+ref" };
        auto pr = std::make_pair( 1, std::ref( opt ) );
        std::vector< decltype( pr ) > v{ pr };
        auto iter = v.begin();
        auto apr = std::make_pair( 1, std::ref( iter ) );

        ASSERT_EQ( recursive_value( apr ), "optional+vector+pair+ref" );

        const boost::optional< std::string > copt = std::string{ "optional+vector+pair+ref" };
        auto cpr = std::make_pair( 1, std::cref( copt ) );
        const std::vector< decltype( cpr ) > cv{ cpr };
        const auto citer = cv.cbegin();
        auto capr = std::make_pair( 1, std::cref( citer ) );

        ASSERT_EQ( recursive_value( capr ), "optional+vector+pair+ref" );
    }
}

class A
{
    int i = 0;
public:
    A() = default;

    A( const A& ) { i = 1; }
    A& operator==( const A& ) { i = 2; return *this; }

    A( A&& ) { i = 3; }
    A& operator==( A&& ) { i = 3; return *this; }

    int get_i(){ return i; }
};

TEST( recursive_value, moving_from_pair )
{
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto mv = recursive_value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto cpy = recursive_value( pair );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        std::pair< int, A > pair = std::make_pair( 1, A() );
        auto mv = recursive_value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        std::pair< int, A > pair = std::make_pair( 1, A() );
        auto cpy = recursive_value( pair );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        std::pair< int, const A > pair = std::make_pair( 1, A() );
        auto mv = recursive_value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        std::pair< int, const A > pair = std::make_pair( 1, A() );
        auto cpy = recursive_value( pair );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        const A a;
        auto pair = std::make_pair( 1, a );
        auto mv = recursive_value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        const A a;
        auto pair = std::make_pair( 1, a );
        auto cpy = recursive_value( pair );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
}
TEST( recursive_value, moving_from_pair_opt )
{
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto mv = recursive_value( std::move( opt ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        const auto opt = boost::make_optional( pair );
        auto mv = recursive_value( std::move( opt ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto cpy = recursive_value( opt );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
}
TEST( recursive_value, moving_from_pair_opt_iter )
{
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        std::vector< decltype( opt ) > v{ opt };
        auto mv = recursive_value( std::make_move_iterator( v.begin() ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        std::vector< decltype( opt ) > v{ opt };
        auto mv = recursive_value( v.begin() );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        std::vector< decltype( opt ) > v{ opt };
        auto mv = recursive_value( std::make_move_iterator( v.cbegin() ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        const std::vector< decltype( opt ) > v{ opt };
        auto mv = recursive_value( std::make_move_iterator( v.begin() ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        std::vector< decltype( opt ) > v{ opt };
        auto cpy = recursive_value( v.begin() );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
}

TEST( recursive_value, moving_from_pair_opt_ref )
{
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto ref = std::ref( opt );
        auto mv = recursive_value( std::move( ref ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto ref = std::cref( opt );
        auto mv = recursive_value( std::move( ref ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto ref = std::ref( opt );
        auto mv = recursive_value( ref );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto opt = boost::make_optional( pair );
        auto ref = std::cref( opt );
        auto mv = recursive_value( ref );
        ASSERT_EQ( mv.get_i(), 1 );
    }
}

TEST( recursive_value, reference_wrapper )
{
	int i = 1;
	int j = 2;
	int k = 3;

    {
        const std::vector< std::reference_wrapper< int > > v { std::ref( i ), std::ref( j ), std::ref( k ) };

        check_range( v );
    }

    {
        std::vector< std::reference_wrapper< int > > v { std::ref( i ), std::ref( j ), std::ref( k ) };

        check_range( v );
    }
}

TEST( recursive_value, pair )
{
    {
        std::vector< std::pair< int, std::string > > v { std::make_pair( 1, std::string( "Helo" ) ), std::make_pair( 2, std::string( "Hola" ) ) };
        check_range( v );
	}

    {
        std::vector< std::pair< int, std::string > > v { std::make_pair( 1, std::string( "Helo" ) ), std::make_pair( 2, std::string( "Hola" ) ) };
        check_range( v );
	}
}


TEST( recursive_value, optional )
{
    {
        const std::vector< boost::optional< int > > v { boost::make_optional( 11 ), boost::make_optional( 22 ) };
        check_range( v );
	}

    {
        std::vector< boost::optional< int > > v { boost::make_optional( 11 ), boost::make_optional( 22 ) };
        check_range( v );
	}
}

TEST( recursive_value, shared )
{
    {
        const std::vector< std::shared_ptr< int > > v { std::make_shared< int >( 45 ), std::make_shared< int >( 33 ) };
        check_range( v );
	}

    {
        std::vector< std::shared_ptr< int > > v { std::make_shared< int >( 45 ), std::make_shared< int >( 33 ) };
        check_range( v );
	}
}

TEST( recursive_value, pointers )
{
    int i = 1;
    int j = 2;
    int k = 3;
    {
        const std::vector< int* > v { &i, &j, &k };
        check_range( v );
	}

    {
        const std::vector< int* > v { &i, &j, &k };
        check_range( v );
	}
}


TEST( recursive_value, optional_plus_ref_wrap )
{
    int i = 99;
    int j = 88;

    {
        const std::vector< boost::optional< std::reference_wrapper< int > > > v { boost::make_optional( std::ref( i ) ), boost::make_optional( std::ref( j ) ) };
        check_range( v );
	}

    {
        std::vector< boost::optional< std::reference_wrapper< int > > > v { boost::make_optional( std::ref( i ) ), boost::make_optional( std::ref( j ) ) };
        check_range( v );
	}
}

TEST( recursive_value, map )
{
    {
        std::map< int, std::string > v { std::make_pair( 0, std::string( "Senior" ) ), std::make_pair( 1, std::string( "Maddam" ) ) };
        check_range( v );
	}

    {
        const std::map< int, std::string > v { std::make_pair( 0, std::string( "Senior" ) ), std::make_pair( 1, std::string( "Maddam" ) ) };
        
        for( auto val : v | boost::adaptors::valued | boost::adaptors::indexed() )
        { 
            ASSERT_EQ( val.value(), recursive_value( *v.find( val.index() ) ) );	
        }

        for( auto val : boost::adaptors::value( v ) | boost::adaptors::indexed() )
        { 
            ASSERT_EQ( val.value(), recursive_value( *v.find( val.index() ) ) );	
        }
	}
}


TEST( recursive_value, list_plus_pair )
{
    {
	    std::list< std::pair< int, std::string > > v { std::make_pair( 1, std::string( "Sir" ) ), std::make_pair( 2, std::string( "Miss" ) ) };

        auto iter = v.begin();
        for( auto val : v | boost::adaptors::valued )
        { 
            ASSERT_EQ( val, recursive_value( *( iter++ ) ) );	
        }

        iter = v.begin();
        for( auto val : boost::adaptors::value( v ) )
        { 
            ASSERT_EQ( val, recursive_value( *( iter++ ) ) );	
        }
	}

    {
	    const std::list< std::pair< int, std::string > > v { std::make_pair( 1, std::string( "Sir" ) ), std::make_pair( 2, std::string( "Miss" ) ) };

        auto iter = v.begin();
        for( auto val : v | boost::adaptors::valued )
        { 
            ASSERT_EQ( val, recursive_value(  *( iter++ ) ) );	
        }

        iter = v.begin();
        for( auto val : boost::adaptors::value( v ) )
        { 
            ASSERT_EQ( val, recursive_value( *( iter++ ) ) );
        }
	}
}

TEST(eswitch_v4_case, opt_refw_pair_refw )
{
    int i = 976;
    int j = 345;
    std::pair< int, std::reference_wrapper< int > > ir = std::make_pair( i, std::ref( i ) );
    std::pair< int, std::reference_wrapper< int > > jr = std::make_pair( i, std::ref( j ) );

	{
        std::vector< 
            boost::optional< 
                std::reference_wrapper< 
                    std::pair< int, 
                        std::reference_wrapper< int > 
                    > 
                > 
            > 
        > v{ boost::make_optional( std::ref( ir ) ), boost::make_optional( std::ref( jr ) ) };

        check_range( v );
	}

    {
        const std::vector< 
            boost::optional< 
                std::reference_wrapper< 
                    std::pair< int, 
                        std::reference_wrapper< int > 
                    > 
                > 
            > 
        > v{ boost::make_optional( std::ref( ir ) ), boost::make_optional( std::ref( jr ) ) };

        check_range( v );
	}
}


TEST( value, without_redirection )
{
    {
        int i = 10;;

        ASSERT_EQ( value( i ), i );
    }
    {
        const int i = 10;;

        ASSERT_EQ( value( i ), i );
    }
    {
        std::string str{ " " };

        ASSERT_EQ( value( str ), str );
    }
    {        
        const std::string str{ " " };

        ASSERT_EQ( value( str ), str );
    }
}

TEST( value, without_redirection_and_move )
{
    {
        std::string str{ " " };

        ASSERT_EQ( value( std::move( str ) ), str );
    }
    {        
        const std::string str{ " " };

        ASSERT_EQ( value( std::move( str ) ), str );
    }
}

TEST( value, with_ref )
{
    {
        int i = 10;
        auto ref = std::ref( i );

        ASSERT_EQ( value( ref ), i );
    }
    {
        int i = 10;
        auto ref = std::cref( i );

        ASSERT_EQ( value( ref ), i );
    }
}

TEST( value, with_opt )
{
    {
        auto opt = boost::optional< int >( 10 );

        ASSERT_EQ( value( opt ), *opt );
    }
    {
        const auto opt = boost::optional< int >( 10 );

        ASSERT_EQ( value( opt ), *opt );
    }
}

TEST( value, with_ptr )
{
    {   
        int i = 16;
        int * j = &i;
        ASSERT_EQ( value( j ), i );
    }
    {
        const int i = 16;
        const  int * j = &i;
        ASSERT_EQ( value( j ), i );
    }
}

TEST( value, with_iter )
{
    {
        auto v = std::vector< int >{ 1, 2, 3 };

        ASSERT_EQ( value( v.begin() ), v[ 0 ] );
    }
    {
        auto v = std::vector< int >{ 1, 2, 3 };

        ASSERT_EQ( value( v.cbegin() ), v[ 0 ] );
    }
    {
        const auto v = std::vector< int >{ 1, 2, 3 };

        ASSERT_EQ( value( v.begin() ), v[ 0 ] );
    }
    {
        const auto v = std::vector< int >{ 1, 2, 3 };

        ASSERT_EQ( value( v.cbegin() ), v[ 0 ] );
    }

}

TEST( value, with_pair )
{
    {
        auto pair = std::make_pair( 1, 2 );

        ASSERT_EQ( value( pair ), pair.second );
    }
    {
        const auto pair = std::make_pair( 1, 2 );

        ASSERT_EQ( value( pair ), pair.second );
    }
}

TEST( value, with_queue )
{
    std::queue< int > q;
    q.push( 87 );
    q.push( 88 );
    
    ASSERT_EQ( value( q ), 87 );        
    ASSERT_EQ( q.size(), 1 );        

    ASSERT_EQ( value( q ), 88 );
    ASSERT_EQ( q.size(), 0 );        
}


TEST( value, with_variant )
{
    {
        boost::variant< int, std::string > var( 10 );
        ASSERT_EQ( value< 0 >( var ), *boost::get< int >( &var ) );
    }
    {
        boost::variant< int, std::string > var( std::string{ "Value" } );
        ASSERT_EQ( value< 1 >( var ), *boost::get< std::string >( &var ) );
    }
    
    {
        const boost::variant< int, std::string > var( 10 );
        ASSERT_EQ( value< 0 >( var ), *boost::get< int >( &var ) );
    }
    {
        const boost::variant< int, std::string > var( std::string{ "Value" } );
        ASSERT_EQ( value< 1 >( var ), *boost::get< std::string >( &var ) );
    }
}


TEST( value, moving_from_pair )
{
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto mv = value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        std::pair< int, A > pair = std::make_pair( 1, a );
        auto cpy = value( pair );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        A a;
        const std::pair< int, A > pair = std::make_pair( 1, a );
        auto mv = value( std::move( pair ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
}


TEST( value, moving_from_opt )
{
    {
        A a;
        auto opt = boost::optional< A >( a );
        auto mv = value( std::move( opt ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        const auto opt = boost::optional< A >( a );
        auto mv = value( std::move( opt ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        auto opt = boost::optional< A >( a );
        auto cpy = value( opt );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        A a;
        const auto opt = boost::optional< A >( a );
        auto cpy = value( opt );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
}

TEST( value, moving_from_variant )
{
    {
        A a;
        auto var = boost::variant< A >( a );
        auto mv = value< 0 >( std::move( var ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        const auto var = boost::variant< A >( a );
        auto mv = value< 0 >( std::move( var ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        auto var = boost::variant< A >( a );
        auto cpy = value< 0 >( var );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
    {
        A a;
        const auto var = boost::variant< A >( a );
        auto cpy = value< 0 >( var );
        ASSERT_EQ( cpy.get_i(), 1 );
    }
}

TEST( value, moving_from_direct )
{
    {
        A a;
        auto mv = value( std::move( a ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        const A a;
        auto mv = value( std::move( a ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        auto mv = value( a );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        const A a;
        auto mv = value( a );
        ASSERT_EQ( mv.get_i(), 1 );
    }
}


TEST( value, moving_from_iter )
{
    {
        A a;
        std::vector< A > v{ a };
        auto mv = value( v.begin() );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        const std::vector< A > v{ a };
        auto mv = value( v.begin() );
        ASSERT_EQ( mv.get_i(), 1 );
    }
    {
        A a;
        std::vector< A > v{ a };
        auto mv = value( std::make_move_iterator( v.begin() ) );
        ASSERT_EQ( mv.get_i(), 3 );
    }
    {
        A a;
        const std::vector< A > v{ a };
        auto mv = value( std::make_move_iterator( v.begin() ) );
        ASSERT_EQ( mv.get_i(), 1 );
    }
}

TEST( value, iter_plus_pair )
{
    std::vector< std::pair< int, int > > v{ { 1, 22 } };
    ASSERT_EQ( value( v.begin() ), std::make_pair( 1, 22 ) );
}

TEST( value, iter_plus_opt )
{
    std::vector< boost::optional< int > > v{ boost::make_optional( 2 ) };
    ASSERT_EQ( value( v.begin() ), boost::make_optional( 2 ) );
}