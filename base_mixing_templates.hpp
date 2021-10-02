// c++17 @Tarnakin V.D.
#pragma once
#ifndef MTL_BASE_MIXING_TEMPLATES_HPP
#define MTL_BASE_MIXING_TEMPLATES_HPP

#include "mtl/exception.hpp"
// macro for creating an accessor
#define MTL_DECLARE_ACCESSOR(name) \
template<typename _PrivateTysTy> \
	struct name \
	{ \
	  using private_types_t = _PrivateTysTy; \
	  static typename _PrivateTysTy::container_t& container( typename private_types_t::impl_t* impl ) \
	  { \
		return impl->container_; \
	  } \
	};

namespace mtl {
// helpers
template<typename ... _ArgsTy>
	struct mixing_list : public _ArgsTy ... { };

template<typename _ClsTy>
	struct private_types_list { };
// mixing for container class
template<typename _AccessorTy>
	class add_base_iterators
    {
public :
	  using accessor_t = _AccessorTy;
	  using derived_t  = typename _AccessorTy::private_types_t::impl_t;
private :
	  derived_t *derived_;
public :
	  add_base_iterators()
		: derived_( static_cast<derived_t*>( this ) )
      {
      }
    
	  auto begin() -> decltype( accessor_t::container( derived_ ).begin() ) {
		return accessor_t::container( derived_ ).begin();
	  }

	  auto end() -> decltype( accessor_t::container( derived_ ).end() ) {
		return accessor_t::container( derived_ ).end();
	  }

	  auto begin() const -> decltype( accessor_t::container( derived_ ).cbegin() ) {
		return accessor_t::container( derived_ ).begin();
	  }

	  auto end() const -> decltype( accessor_t::container( derived_ ).cend() ) {
		return accessor_t::container( derived_ ).end();
      }
    };

template<typename _AccessorTy>
    class add_const_iterators
    {
public :
	  using accessor_t = _AccessorTy;
	  using derived_t  = typename _AccessorTy::private_types_t::impl_t;
private :
	  derived_t *derived_;
public :
	  add_const_iterators()
		: derived_( static_cast<derived_t*>( this ) )
      {
      }

	  auto cbegin() const -> decltype( accessor_t::container( derived_ ).cbegin() ) {
		return accessor_t::container( derived_ ).cbegin();
      }

	  auto cend() const -> decltype( accessor_t::container( derived_ ).cbegin() ) {
		return accessor_t::container( derived_ ).cend();
      }
    };

template<typename _AccessorTy>
	using add_iterators = mixing_list
	<
	  add_base_iterators<_AccessorTy>,
	  add_const_iterators<_AccessorTy>
	>;

template<
	typename _DerivedTy,
	typename _AnyTy = _DerivedTy>
    struct add_non_equalable
    {
      virtual bool operator == (_AnyTy const&) = 0;

      bool operator != ( _AnyTy const& other ) {
        return !( *this == other );
      }
    };

template<
	typename _DerivedTy,
	typename _AnyTy = _DerivedTy>
	struct add_sum
    {
      virtual _DerivedTy& operator += (_AnyTy const&) = 0;

      _DerivedTy operator + ( _AnyTy const & other ) const
      {
        auto cpy = static_cast<const _DerivedTy&>( *this );
        return cpy += other;
      }
    };

template<
	typename _DerivedTy,
	typename _AnyTy = _DerivedTy>
    struct add_difference
    {
      virtual _DerivedTy& operator -= (_AnyTy const&) = 0;

      _DerivedTy operator - ( _AnyTy const & other ) const
      {
        auto cpy = static_cast<const _DerivedTy&>( *this );
        return cpy -= other;
      }
    };

template<
	typename _DerivedTy,
	typename _ElemTraitsTy,
	typename _ReturnTy = _DerivedTy>
    struct add_multiplying_by_value
    {
      virtual _DerivedTy& operator *= (typename _ElemTraitsTy::type_t const&) = 0;

      _ReturnTy operator * ( typename _ElemTraitsTy::type_t const & value )
      {
        _ReturnTy cpy = static_cast<_DerivedTy&>( *this );
        return cpy *= value;
      }
    };

template<
	typename _DerivedTy,
	typename _ElemTraitsTy,
	typename _ReturnTy = _DerivedTy>
    struct add_division_by_value
      : public add_multiplying_by_value<_DerivedTy, _ElemTraitsTy, _ReturnTy>
    {

      _DerivedTy& operator /= ( typename _ElemTraitsTy::type_t const & value )
      {
        if(value <= 0) {
			throw MTL_EXCEPTION( "bad operation : <value> = <0>" );
        }
        return *this *= (1.0/value);
      }

      _ReturnTy operator / ( typename _ElemTraitsTy::type_t const & value ) const
      {
        if(value <= 0) {
            throw MTL_EXCEPTION( "bad operation : <value> = <0>" );
        }
        _ReturnTy cpy = static_cast<const _DerivedTy&>( *this );
        return cpy *= ( 1.0/value );
      }
    };
} // mtl
#endif
