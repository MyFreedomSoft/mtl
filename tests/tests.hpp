#pragma once
#ifndef MTL_TESTS_TESTS_HPP
#define MTL_TESTS_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#define MTL_CATCH catch( std::exception const& e ) { BOOST_TEST_MESSAGE( e.what() ); }

#endif