// c++17 @Tarnakin V.D.
//this header has a description of the matrix transforms
#pragma once
#ifndef MTL_MATH_HPP
#define MTL_MATH_HPP

#include "types.hpp"
#include "matrix.hpp"
#include "math_defines.hpp"
#include "algorithm.hpp"

#include <cmath>

namespace mtl {

template<typename _Ty>
    detail::optional_t< 
      dense::matrix_3xn_t<std::size_t> 
    > lee_neumann( sparse::matrix<_Ty> const& map,
                   std::size_t x_from, std::size_t y_from,
                   std::size_t x_to,   std::size_t y_to,
                   _Ty blank                               )
    {
      if( map.size() <= y_from || map.csize() <= x_from || map.size() <= y_to || map.csize() <= x_to ) {
          throw detail::exception_t("<mtl::lee_neumann> : out of range");
      }
      if( map[y_from][x_from] != blank || map[y_to][x_to] != blank ) {
          return MTL_NULLOPT;
      }

      using matrix_3xn_t = dense::matrix_3xn_t<std::size_t>;

      matrix_3xn_t way  = { y_from, x_from, 0 };
      const int    dx[] = { 1, 0, -1,  0 };
      const int    dy[] = { 0, 1,  0, -1 };
      bool         stop;

      auto wave_propagation = [&map,  &blank,
                               &way,  &stop,
                               &dx,   &dy,
                               &x_to, &y_to  ]( const auto y, const auto x, const auto & d ) -> int
      {
        for( std::size_t i(0); i < 4; i++ )
        {
            int iy = y + dy[i], ix = x + dx[i];
            if( (map.size() > iy || map.csize() > ix) &&
                 iy          >= 0                     &&
                 ix          >= 0                     &&
                 map[iy][ix] == blank                    )
            {
                if( way.push_back_if( { iy, ix, d + 1 },
                    [&ix, &iy]( auto const& v ) {
                    return !(v[0] == iy && v[1] == ix);
                }) )
                {
                    if( iy == y_to && ix == x_to) {
                        return std::size(way) - 1;
                    }
                    stop = false;
                }
            }
        }
        return 0;
      };

      std::ptrdiff_t y_end(0);
      std::size_t    d(0);

      do {
          stop = true;
          for( std::size_t i(0); i < std::size(way); i++ )
          {
              if( way[i][2] == d ) {
                  y_end = wave_propagation( way[i][0], way[i][1], d );
              }
              if( y_end ) {
                  break;
              }
          }
          d++;
      } while( !stop );

      if( !y_end ) return MTL_NULLOPT;

      auto neighbour = [&dx, &dy]( auto const& curr_v, auto const& last_v )
      {
        return ( ( last_v[0] - 1 == curr_v[0]   || last_v[0] + 1 == curr_v[0] ) &&
                 ( last_v[1]     == curr_v[1] ) ||
                 ( last_v[1] - 1 == curr_v[1]   || last_v[1] + 1 == curr_v[1] ) &&
                 ( last_v[0]     == curr_v[0] )                                    );
      };

      decltype( way[0] ) last_v( way[y_end] );
      d = way[y_end][2] - 1;
      y_end--;
      matrix_3xn_t min_w;
      min_w.push_back( { x_to, y_to, 1 } );

      while( d > 0 ) {
          auto l = d;
          for( std::size_t i(y_end); i != 0; i-- )
          {
              if( way[i][2] == d && neighbour(way[i], last_v) ) {
                  min_w.push_back( { way[i][1], way[i][0], 1 } );
                  last_v = way[i];
                  d--;
                  break;
              }
          }
          if( l == d ) {
              return MTL_NULLOPT;
          }
      }

      min_w.push_back( { x_from, y_from, 1 } );
      return min_w;
    }
// LU
template<typename _MatrixTy>
    typename std::enable_if< // if arithmetic type
      std::is_arithmetic_v<
        typename _MatrixTy::type_t
      >, // enable func and return pair<L, U>
      std::pair<_MatrixTy, _MatrixTy>
    >::type lu( _MatrixTy const& A )
    {
      auto size = A.csize();

      if( size != std::size( A ) ) {
          throw detail::exception_t( "<mtl::lu> : <matrix.size> != <matrix.csize>" );
      }

      _MatrixTy L( size );
      _MatrixTy U = A;

      using size_t = std::size_t;

      for( size_t i(0); i < size; i++ )
          for( size_t j(i); j < size; j++ )
              L[j][i] = U[j][i]/U[i][i];

      for( size_t k(1); k < size; k++ )
      {
          for( size_t i(k - 1); i < size; i++ )
              for( size_t j(i); j < size; j++ )
                  L[j][i] = U[j][i]/U[i][i];
          for( size_t i(k); i < size; i++)
              for( size_t j(k - 1); j < size; j++ )
                  U[i][j] = U[i][j] - L[i][k - 1]*U[k - 1][j];
      }
      return { L, U };
    }

template<typename _Ty>
    void move( dense::matrix_3xn_t<_Ty> & m_res, _Ty x0, _Ty y0, _Ty x1, _Ty y1 )
    {
      dense::matrix_3xn_t<_Ty> t_tr
      {   1,          0,       0,
          0,          1,       0,
          x1 - x0,    y1 - y0, 1   };

      m_res *= t_tr;
    }

template<typename _Ty>
    void move( dense::matrix_3xn_t<_Ty> & m_res, _Ty x, _Ty y ) {
      move( m_res, 0, 0, x, y );
    }

template<typename _Ty>
    void scale( dense::matrix_3xn_t<_Ty> & m_res, _Ty k_x, _Ty k_y )
    {
      _Ty m = m_res[0][0]*(1 - k_x);
      _Ty l = m_res[0][1]*(1 - k_y);

      dense::matrix_3xn_t<_Ty> t_scl
      {   k_x, 0,   0,
          0,   k_y, 0,
          m,   l,   1   };

      m_res *= t_scl;
    }

template<typename _Ty>
    void rotate( dense::matrix_3xn_t<_Ty> & m_res, int r_ang, _Ty x, _Ty y )
    {
      _Ty sin = std::sin(r_ang);
      _Ty cos = std::cos(r_ang);

      dense::matrix_3xn_t<_Ty> t_rot
      {   cos,                 sin,                 0,
         -sin,                 cos,                 0,
          x*(1 - cos) + y*sin, y*(1 - cos) - x*sin, 1   };

      m_res *= t_rot;
    }
} // mtl
#endif
