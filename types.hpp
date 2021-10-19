#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP

#define MTL_PASTE_DETAIL \
template<typename _Ty> \
	using variant_t  = std::variant<_Ty>; \
template<typename _Ty> \
    using optional_t = std::optional<_Ty>; \
} }

#ifdef __BORLANDC__ 
// C++ Builder ver <= 10.4
// Incorrect working std::variant and std::optional
// Using boost::variant and boost::optional
# if __BORLANDC__ <= 0x0750 

# include <boost/variant.hpp>
# include <boost/optional.hpp>

# define MTL_NULLOPT boost::none
namespace mtl { namespace detail {
template<typename _Ty>
    using variant_t  = boost::variant<_Ty>;
template<typename _Ty>
    using optional_t = boost::optional<_Ty>; 
} }
// any version
# else

# include <variant>
# include <optional>

# define MTL_NULLOPT std::nullopt
namespace mtl { namespace detail {
MTL_PASTE_DETAIL
} }
# endif
// any ide
#else

#define MTL_NULLOPT std::nullopt
// paste types
namespace mtl { namespace detail {
MTL_PASTE_DETAIL
} }
#endif
// delete macro
#undef MTL_PASTE_DETAIL

#endif