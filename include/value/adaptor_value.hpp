#pragma once

#include "value.hpp"
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/traversal.hpp>

namespace boost {
	namespace adaptors {
		struct value_internal
		{

		};

		namespace {
			const auto valued = value_internal{};
		}
} }

template< class TIter, class TCategory >
class value_iterator
: public boost::iterator_facade< value_iterator< TIter, TCategory >,
		std::decay_t< recursive_value_trait_t< TIter > >,
	  	TCategory,
	  	std::conditional_t< std::is_const< TIter >::value, 
	  		const std::decay_t< recursive_value_trait_t< TIter > >, 
			      std::decay_t< recursive_value_trait_t< TIter > > >
        >
{
   public:
      using iterator = value_iterator< std::decay_t< TIter >, TCategory >;
      using const_iterator = value_iterator< const std::decay_t< TIter >, TCategory >;
      using difference_type = std::streamoff;

      friend class boost::iterator_core_access;

      value_iterator( std::decay_t< TIter > iter )
      : iter_( iter )
      {
      }

   private:
      bool equal( const iterator & rhs ) const
      {
		  return iter_ == rhs.iter_;
      }

      bool equal( const const_iterator & rhs ) const
      {
		  return iter_ == rhs.iter_;
      }

      void increment()
      {
         ++iter_;
      }

      void decrement()
      {
         --iter_;
      }

      difference_type distance_to( const iterator & rhs) const
      {
         return rhs.iter_ - iter_;
      }

      difference_type distance_to( const const_iterator & rhs) const
      {
         return rhs.iter_ - iter_;
      }

      typename iterator::reference dereference()
      {
		  return recursive_value( iter_ );
      }

      typename const_iterator::reference dereference() const
      {
		  return recursive_value( iter_ );
      }

      void advance( std::streamoff n )
      {
         iter_ += n;
      }

      std::decay_t< TIter > iter_;
};

template< typename SinglePassRange, typename TCategory >
struct value_range
    : boost::iterator_range<
        value_iterator< 
            std::conditional_t< std::is_const< SinglePassRange >::value, 
                const std::decay_t< typename boost::range_iterator< SinglePassRange >::type >, 
                typename boost::range_iterator< SinglePassRange >::type >, 
            TCategory 
        >
    >
{
	using iterator = value_iterator< 
            std::conditional_t< std::is_const< SinglePassRange >::value, 
                const std::decay_t< typename boost::range_iterator< SinglePassRange >::type >, 
                typename boost::range_iterator< SinglePassRange >::type >, 
            TCategory 
        >;

    using base_t = boost::iterator_range< iterator >;

    BOOST_RANGE_CONCEPT_ASSERT((
        boost::SinglePassRangeConcept<SinglePassRange>));
public:

    value_range( SinglePassRange & r )
        : base_t( iterator( std::begin( r ) ), iterator( std::end( r ) )  )
    {
    }
};

namespace boost {
	namespace adaptors {

template< typename SinglePassRange >
inline auto operator|( SinglePassRange& r, boost::adaptors::value_internal )
{
    BOOST_RANGE_CONCEPT_ASSERT((
        boost::SinglePassRangeConcept< SinglePassRange >
    ));

    return value_range< SinglePassRange, typename boost::range_traversal< SinglePassRange >::type >( r );
}

template< typename SinglePassRange >
inline auto operator|( const SinglePassRange& r, boost::adaptors::value_internal )
{
    BOOST_RANGE_CONCEPT_ASSERT((
        boost::SinglePassRangeConcept< const SinglePassRange >
    ));

    return value_range< const SinglePassRange, typename boost::range_traversal< const SinglePassRange >::type >( r );
}

template< typename SinglePassRange >
auto value( SinglePassRange && rng )
{
	return rng | boost::adaptors::valued;
}

} } /// namespace