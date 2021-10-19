﻿
#include "tests.hpp"
#include "../sparse/io.hpp"
template<typename _Ty = void>
  struct vector_fixture { };

template<>
  struct vector_fixture<std::ptrdiff_t>
  {
    mtl::sparse::vector<std::ptrdiff_t> v;
    vector_fixture()
        : v( { 1, 2, 2, 4, 5, 6, 7, 8, 9 } )
      { BOOST_TEST_MESSAGE( v ); }
  };

BOOST_AUTO_TEST_SUITE( test_suite_sparse_vector )

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_create, vector_fixture<std::ptrdiff_t> )
{ try {
  BOOST_TEST_MESSAGE( "checking sparse::vector<std::ptrdiff_t> v" );
  BOOST_REQUIRE( v.frequent() == 2 );
  BOOST_REQUIRE( v.sparse() );
  BOOST_REQUIRE_NO_THROW( v[3] == 4 );
  BOOST_REQUIRE_NO_THROW( v[2] == 2 );
  BOOST_REQUIRE( std::size( v ) == 9 );
  BOOST_REQUIRE( v.amount() == 7 );
} MTL_CATCH }

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_copy_and_compare, vector_fixture<std::ptrdiff_t> )
{ try {
  decltype(v) v_(v);
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_TEST_MESSAGE( "checking v != v_" );
  BOOST_REQUIRE( v == v_ );
  BOOST_TEST_MESSAGE( "!( checking v == v_ )" );
  BOOST_REQUIRE( !( v != v_ ) );
} MTL_CATCH }

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_assign, vector_fixture<std::ptrdiff_t> )
{ try {
  BOOST_TEST_MESSAGE( "assignment v to v1" );
  decltype(v) v1;
  v1 = v;
  BOOST_TEST_MESSAGE( "v1 :" );
  BOOST_TEST_MESSAGE( v1 );
  BOOST_CHECK( v == v1 );
} MTL_CATCH }

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_operations, vector_fixture<std::ptrdiff_t> )
{ try {
  BOOST_TEST_MESSAGE( "copying v to v_" );
  decltype(v) v_(v);
  BOOST_TEST_MESSAGE( "checking v_ *= 1" );
  v_ *= 1;
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v_.frequent() == 2 );
  BOOST_CHECK( v_.sparse() );
  BOOST_CHECK( v_[3] == 4 );
  BOOST_CHECK( v_[2] == 2 );
  BOOST_CHECK( std::size( v_ ) == 9 );
  BOOST_CHECK( v_.amount() == 7 );
  BOOST_TEST_MESSAGE( "checking v_ /= 1" );
  v_ /= 1;
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v_.frequent() == 2 );
  BOOST_CHECK( v_.sparse() );
  BOOST_CHECK( v_[3] == 4 );
  BOOST_CHECK( v_[2] == 2 );
  BOOST_CHECK( std::size( v_ ) == 9 );
  BOOST_CHECK( v_.amount() == 7 );
  BOOST_TEST_MESSAGE( "checking v_ /= 0" );
  BOOST_CHECK_THROW( v_ /= 0, mtl::detail::exception_t );
  BOOST_TEST_MESSAGE( "checking v_ += empty" );
  decltype(v) empty( std::size( v_ ), 0 );
  v_ += empty;
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v_.frequent() == 2 );
  BOOST_CHECK( v_.sparse() );
  BOOST_CHECK( v_[3] == 4 );
  BOOST_CHECK( v_[2] == 2 );
  BOOST_CHECK( std::size( v_ ) == 9 );
  BOOST_CHECK( v_.amount() == 7 );
  BOOST_TEST_MESSAGE( "checking v_ -= empty" );
  v_ -= empty;
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v_.frequent() == 2 );
  BOOST_CHECK( v_.sparse() );
  BOOST_CHECK( v_[3] == 4 );
  BOOST_CHECK( v_[2] == 2 );
  BOOST_CHECK( std::size( v_ ) == 9 );
  BOOST_CHECK( v_.amount() == 7 );
} MTL_CATCH }

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_push, vector_fixture<std::ptrdiff_t> )
{ try {
  BOOST_TEST_MESSAGE( "pushing { 1, 1 }" );
  for( std::size_t i(0); i < 2; i++ )
    v.push_back( 1 );
  BOOST_CHECK( !v.sparse() );
  BOOST_CHECK( !v.frequent() );
  v.do_sparse();
  BOOST_TEST_MESSAGE( "creating v_" );
  decltype(v) v_ { 1, 2, 2, 4, 5, 6, 7, 8, 9, 1, 1 };
  BOOST_TEST_MESSAGE( "v :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v.frequent() == 1 );
  BOOST_CHECK( v[9] == 1 );
  BOOST_CHECK( v[10] == 1 );
  BOOST_CHECK( std::size( v ) == 11 );
  BOOST_CHECK( v.amount() == 8 );
} MTL_CATCH }

BOOST_FIXTURE_TEST_CASE( test_sparse_vector_erase, vector_fixture<std::ptrdiff_t> )
{ try {
  BOOST_TEST_MESSAGE( "erasing i = { 0, 3, 6 }" );
  for( std::size_t i(0); i < std::size( v ); i += 3 ) {
      v.erase( i );
  }
  BOOST_CHECK( !v.sparse() );
  BOOST_CHECK( !v.frequent() );
  BOOST_TEST_MESSAGE( "v :" );
  BOOST_TEST_MESSAGE( v );
  v.do_sparse();
  BOOST_TEST_MESSAGE( "v :" );
  BOOST_TEST_MESSAGE( v );
  BOOST_TEST_MESSAGE( "creating v_" );
  decltype(v) v_ { 2, 2, 4, 6, 7, 8 };
  BOOST_TEST_MESSAGE( "v_ :" );
  BOOST_TEST_MESSAGE( v_ );
  BOOST_CHECK( v == v_ );
  BOOST_CHECK( v.frequent() == 2 );
  BOOST_CHECK( std::size( v ) == 6 );
  BOOST_CHECK( v.amount() == 4 );
} MTL_CATCH }

BOOST_AUTO_TEST_SUITE_END()
