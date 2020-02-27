#pragma once

#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <queue>
#include <map>
#include <boost/optional.hpp>

//template< class... >
//using void_t = void;

/// GCC workaround
namespace void_details {
    template < class... >
    struct make_void { using type = void; };
}

template < class ... T > 
using void_t = typename void_details::make_void<T...>::type;

template< typename T, typename = void_t<> >
struct has_deref_operator : std::false_type {};

template< typename T >
struct has_deref_operator< T, void_t< decltype( *std::declval< T >() ) > > : std::true_type {};

template< typename T >
constexpr bool has_deref_operator_v = has_deref_operator< T >::value;

template< typename T, typename = void_t<> >
struct has_get_method : std::false_type {};

template< typename T >
struct has_get_method< T, void_t< decltype( std::declval< T >().get() ) > > : std::true_type {};

template< typename T >
constexpr bool has_get_method_v = has_get_method< T >::value;

template< typename T, typename = void >
struct deref
{
	using type = T;
};

template< typename T >
struct deref< T, std::enable_if_t< has_deref_operator_v< T >, void > >
{
	using type = decltype( *std::declval< T >() );
};

template< typename T >
using deref_t = typename deref< T >::type;

template< typename T, typename = void >
struct method_get
{
	using type = T;
};

template< typename T >
struct method_get< T, std::enable_if_t< has_get_method_v< T >, void > >
{
	using type = decltype( std::declval< T >().get() );
};

template< typename T >
using method_get_t = typename method_get< T >::type;


template< int Idx, typename ... Ts >
struct type_by_index
{
	static_assert( Idx < sizeof...( Ts ), "INDEX is [out of range]!" );

	using types = boost::mpl::vector< Ts... >;

	using type = typename boost::mpl::at< types, boost::mpl::int_< Idx > >::type;
};

template< int Idx, typename ... Ts >
using type_by_index_t = typename type_by_index< Idx, Ts... >::type;



template< typename T1, typename T2 >
auto & value( std::pair< T1, T2 > & pair )
{
    return pair.second;
}

template< typename T1, typename T2 >
const auto & value( const std::pair< T1, T2 > & pair )
{
    return pair.second;
}

template< typename T1, typename T2 >
auto && value( std::pair< T1, T2 > && pair )
{
    return std::move( pair.second );
}

template< typename T1, typename T2 >
const auto & value( const std::pair< T1, T2 > && pair )
{
    return pair.second;
}

template< typename T >
auto value( std::queue< T > & q)
{
    auto val = std::move( q.front() );
    q.pop();

    return val;
}

template< int Idx, typename ... TArgs >
const auto & value(const boost::variant< TArgs... >& var)
{    
    return *boost::get< type_by_index_t< Idx, TArgs... > >( &var );
}

template< int Idx, typename ... TArgs >
auto & value( boost::variant< TArgs... >& var)
{    
    return *boost::get< type_by_index_t< Idx, TArgs... > >( &var );
}


template< int Idx, typename ... TArgs >
auto && value( boost::variant< TArgs... > && var)
{    
    return std::move( *boost::get< type_by_index_t< Idx, TArgs... > >( &var ) );
}

template< typename T, std::enable_if_t< 
	boost::mpl::and_< 
		boost::mpl::not_< has_deref_operator< T > >,
		boost::mpl::not_< has_get_method< T > > 
	>::value, int > = 0 >
auto && value( T && val )
{
    return std::forward< T >( val );
}

template< typename T, std::enable_if_t< 
	boost::mpl::and_< 
		has_deref_operator< T >,
		boost::mpl::not_< has_get_method< T > > 
	>::value, int > = 0 >
auto && value( T && ptr )
{
    return std::forward< decltype( *std::forward< T >( ptr ) ) >( *std::forward< T >( ptr ) );
}

template< typename T, std::enable_if_t< 
	boost::mpl::and_< 
		boost::mpl::not_< has_deref_operator< T > >,
		has_get_method< T > 
	>::value, int > = 0 >
auto && value( T && ptr )
{
    return std::forward< decltype( std::forward< T >( ptr ).get() ) >( std::forward< T >( ptr ).get() );
}

template< typename T, std::enable_if_t< 
	boost::mpl::and_< 
		has_deref_operator< T >,
		has_get_method< T > 
	>::value, int > = 0 >
auto && value( T && ptr )
{
    return std::forward< decltype( *std::forward< T >( ptr ) ) >( *std::forward< T >( ptr ) );
}

namespace details
{
	template< typename T1, typename T2 >
	static constexpr std::true_type  is_pair_impl( const std::pair< T1, T2 > & );

	template< typename T >
	static constexpr std::false_type is_pair_impl( const T & );

	template< typename T >
	static constexpr std::true_type  is_ref_impl( const std::reference_wrapper< T > & );

	template< typename T >
	static constexpr std::false_type is_ref_impl( const T & );
} // namespace details

template< typename T >
struct is_std_pair : decltype( details::is_pair_impl( std::declval< std::decay_t< T > >() ) ) {};

template< typename T >
constexpr bool is_std_pair_v = is_std_pair< T >::value;

template< typename T >
struct is_std_reference_wrapper : decltype( details::is_ref_impl( std::declval< std::decay_t< T > >() ) ) {};

template< typename T >
constexpr bool is_std_reference_wrapper_v = is_std_reference_wrapper< T >::value;

template< typename T >
struct Recursive_value_trait
{
	template< typename T_ >
	struct return_type_of
	{
		using type = decltype( Recursive_value_trait< T_ >:: template impl< T_ >() );
	};

	template< typename T_ >
	using return_type_of_t = typename return_type_of< T_ >::type;
	


	template< typename T_, std::enable_if_t<
		boost::mpl::and_<
			boost::mpl::not_< has_deref_operator< T_ > >,
			boost::mpl::not_< is_std_pair< T_ > >,
			boost::mpl::not_< is_std_reference_wrapper< T_ > >
		>::value, int > = 0 
		>
	static constexpr T_ 
		impl();

	template< typename T_, std::enable_if_t< is_std_pair_v< T_ >, int > = 0 >
	static constexpr 
		std::conditional_t< std::is_reference< T_ >::value, 
			typename boost::mpl::if_< std::is_const< std::remove_reference_t< T_ > >,
					const return_type_of_t< typename std::remove_reference_t< T_ >::second_type >&,	
						  return_type_of_t< typename std::remove_reference_t< T_ >::second_type >& >::type,
			typename boost::mpl::if_< std::is_const< std::remove_reference_t< T_ > >,
					const return_type_of_t< typename std::remove_reference_t< T_ >::second_type >,	
						  return_type_of_t< typename std::remove_reference_t< T_ >::second_type > >::type
		>
		impl();

	template< typename T_, std::enable_if_t< has_deref_operator_v< T_ >, int > = 0 >
	static constexpr return_type_of_t< deref_t< T_ > >
		impl();

	template< typename T_, std::enable_if_t< is_std_reference_wrapper_v< T_ >, int > = 0 >
	static constexpr return_type_of_t< method_get_t< T_ > >
		impl();

	using type = decltype( Recursive_value_trait::impl< T >() );
};

template< typename T >
using recursive_value_trait_t = typename Recursive_value_trait< T >::type;

struct Recursive_value_details
{
	template< typename T, 
		std::enable_if_t< 
			boost::mpl::and_<
				boost::mpl::not_< is_std_pair< T > >,
				boost::mpl::not_< has_deref_operator< T > >,
				boost::mpl::not_< has_get_method< T > >	
		>::value, int > = 0 >
	static constexpr auto && recursive_value( T && val )
	{
		return std::forward< T >( val );
	}



	template< typename T1, typename T2 >
	static constexpr auto & recursive_value( const std::pair< T1, T2 > & pair )
	{
		return recursive_value( pair.second );
	}

	template< typename T1, typename T2 >
	static constexpr auto & recursive_value( std::pair< T1, T2 > & pair )
	{
		return recursive_value( pair.second );
	}

	template< typename T1, typename T2 >
	static constexpr auto && recursive_value( std::pair< T1, T2 > && pair )
	{
		return recursive_value( std::move( pair.second ) );
	}



	template< typename T, std::enable_if_t< 
		boost::mpl::and_< 
			has_deref_operator< T >, 
			has_get_method< T > >
		::value, int > = 0 >
	static constexpr auto && recursive_value( T && ptr )
	{
        return recursive_value( std::forward< decltype( *std::forward< T >( ptr ) ) >( *std::forward< T >( ptr ) ) );
	}

	template< typename T, std::enable_if_t< 
		boost::mpl::and_< 
			has_deref_operator< T >, 
			boost::mpl::not_< has_get_method< T > > >
		::value, int > = 0 >
	static constexpr auto && recursive_value( T && ptr )
	{
		return recursive_value( std::forward< decltype( *std::forward< T >( ptr ) ) >( *std::forward< T >( ptr ) ) );
	}

	template< typename T, std::enable_if_t< 
		boost::mpl::and_< 
			boost::mpl::not_< has_deref_operator< T > >, 
			has_get_method< T > >
		::value, int > = 0 >
	static constexpr auto && recursive_value( T && ptr )
    {
        return recursive_value( std::forward< decltype( std::forward< T >( ptr ).get() ) >( std::forward< T >( ptr ).get() ) );
    }
};

template< typename T >
auto&& recursive_value( T && val )
{
	return Recursive_value_details::recursive_value( std::forward< T >( val ) );
}