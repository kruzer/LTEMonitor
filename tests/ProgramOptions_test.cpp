/*
 * ProgramOptions_test.cpp
 *
 *  Created on: 27 wrz 2017
 *      Author: mikey
 */
//#define BOOST_TEST_DYN_LINK //http://neyasystems.com/an-engineers-guide-to-unit-testing-cmake-and-boost-unit-tests/
#define BOOST_TEST_MODULE ProgramOptions_Test
#include "ProgramOptions.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <iostream>

struct stream_redir {
	stream_redir(std::ostream &src_stream, std::streambuf * new_buf) : src(src_stream), old( src_stream.rdbuf( new_buf )){ }

	~stream_redir( ) {
		src.rdbuf( old );
	}

	private:
	std::ostream	&src;
	std::streambuf * old;
};

BOOST_AUTO_TEST_CASE( assigments ) {
	char const *argv[]={"LTEMonitor","-u", "adm", "-s","192.168.8.1","-p","pass","-o","4200","-b"};
	ProgramOptions po(sizeof(argv)/sizeof(argv[0]),argv);
	BOOST_CHECK_MESSAGE( po.user == "adm" , "user is:" << po.user);
	BOOST_CHECK_MESSAGE( po.server == "192.168.8.1" , "server is:" << po.server);
	BOOST_CHECK_MESSAGE( po.password == "pass" , "password is:" << po.password);
	BOOST_CHECK_MESSAGE( po.port == 4200 , "local port is:" << po.port);
	BOOST_CHECK_MESSAGE( po.preventGui == true , "prevent gui is:" << po.preventGui);
	BOOST_CHECK(po.proceed);
}

BOOST_AUTO_TEST_CASE( defaults ) {
	char const *argv[]={"LTEMonitor"};
	ProgramOptions po(sizeof(argv)/sizeof(argv[0]),argv);
	BOOST_CHECK_MESSAGE( po.user == "admin" , "default user is:" << po.user);
	BOOST_CHECK_MESSAGE( po.server == "192.168.0.1" , "default server is:" << po.server);
	BOOST_CHECK_EQUAL( po.password , "admin");
	BOOST_CHECK_EQUAL( po.port , 8080);
	BOOST_CHECK_EQUAL( po.preventGui , false);
	BOOST_CHECK(po.proceed);
}

BOOST_AUTO_TEST_CASE( output_help ) {
	char const *argv[]={"LTEMonitor","-h"};
	boost::test_tools::output_test_stream output;
	{
		stream_redir straznik( std::cout, output.rdbuf( ) );
		ProgramOptions po(sizeof(argv)/sizeof(argv[0]),argv);
		BOOST_CHECK( !po.proceed);
	}
	BOOST_CHECK( !output.is_empty( false ));
	BOOST_CHECK( output.str().find("Options:") != std::string::npos );
	BOOST_CHECK( output.str().find("--user") != std::string::npos );
	BOOST_CHECK( output.str().find("--password") != std::string::npos );
}
BOOST_AUTO_TEST_CASE( output_ver ) {
	char const *argv[]={"LTEMonitor","-v"};
	boost::test_tools::output_test_stream output;
	{
		stream_redir straznik( std::cout, output.rdbuf( ) );
		ProgramOptions po(sizeof(argv)/sizeof(argv[0]),argv);
		BOOST_CHECK( !po.proceed);
	}
	BOOST_CHECK( !output.is_empty( false ));
	BOOST_CHECK( output.str().find("version ") != std::string::npos );
}

BOOST_AUTO_TEST_CASE( output_invalid ) {
	char const *argv[]={"LTEMonitor","-z"};
	boost::test_tools::output_test_stream output;
	{
		stream_redir straznik( std::cerr, output.rdbuf( ) );
		ProgramOptions po(sizeof(argv)/sizeof(argv[0]),argv);
		BOOST_CHECK( !po.proceed);
	}
	BOOST_CHECK( !output.is_empty( false ));
	BOOST_CHECK( output.str().find("unrecognised") != std::string::npos );
}
