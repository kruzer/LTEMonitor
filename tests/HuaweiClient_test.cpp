//#define BOOST_TEST_DYN_LINK //http://neyasystems.com/an-engineers-guide-to-unit-testing-cmake-and-boost-unit-tests/
#define BOOST_TEST_MODULE HuaweiClient_Test
#include <boost/test/unit_test.hpp>
#include "Simple-Web-Server/client_http.hpp"
#include "Simple-Web-Server/server_http.hpp"
#include "HuaweiClient.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

std::string json1_test = R"({
    "response": {
        "TokInfo": "kb8qjdiIu4bpoc9SexiTpCR0uHzgtF4M",
        "SesInfo": "pNvigDHhDmoR9XL5B0GMiHiLn0Eno1H5i8C9Syiy3RXWICIJkGMvQi0Ye7qaWGaYK84YdOTyVKZD8GlPfl0vQq5q2Hktko4Xoj6D6kfZ7lir6xN6A000xNFy9bw3dfsZ"
    }
}
)";

std::string json2_test = R"({
    "response": "OK"
}
)";

struct F{
	std::thread thr;
	HttpServer server;

	F() {
		server.config.port = 8080;
		server.resource["/api/webserver/SesTokInfo"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> ) {
			try {
				response->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response><TokInfo>kb8qjdiIu4bpoc9SexiTpCR0uHzgtF4M</TokInfo><SesInfo>pNvigDHhDmoR9XL5B0GMiHiLn0Eno1H5i8C9Syiy3RXWICIJkGMvQi0Ye7qaWGaYK84YdOTyVKZD8GlPfl0vQq5q2Hktko4Xoj6D6kfZ7lir6xN6A000xNFy9bw3dfsZ</SesInfo></response>");
			}
			catch(const std::exception &e) {
				response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
			}
		};
		server.resource["/api/user/login"]["POST"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			try {
				if(request->content.string().find("wronguser") == std::string::npos)
					response->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?><response>OK</response>", {{"Set-Cookie", "admin-cookie"}});
				else
					response->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?><error><message></message><count>1</count><code>108006</code></error>");
			}
			catch(const std::exception &e) {
				response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
			}
		};
		thr=std::thread([this]() {
				server.start();
		});
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	
	~F(){
		server.stop();
		thr.join();
	}
};

BOOST_AUTO_TEST_CASE( auth ) {
	HuaweiAuth ha("admin","passwd");
	BOOST_CHECK_EQUAL(ha.encodeWithToken("token"),"NDg4OTM0YjUyOGU2MTE2ZDczNTE4ZGVkMzM2ZjczNmQxYzYzNTU3N2U0ZDkxZmI2ZDhiMDgxNGQ4ZDA4MjYxMA==");
	BOOST_CHECK_EQUAL(ha.getXmlWithToken("token"),"<?xml version 1.0 encoding=UTF-8?><request><Username>admin</Username><Password>NDg4OTM0YjUyOGU2MTE2ZDczNTE4ZGVkMzM2ZjczNmQxYzYzNTU3N2U0ZDkxZmI2ZDhiMDgxNGQ4ZDA4MjYxMA==</Password><password_type>4</password_type></request>");
}

BOOST_FIXTURE_TEST_CASE( connection , F) {
	HuaweiClient hc("localhost:8080","admin","passwd");
	//HuaweiClient hc("192.168.0.1","admin","aaaaaaA1A1");
	BOOST_CHECK(!hc.isConnected());
	BOOST_CHECK(!hc.isLoggedIn());

	hc.login();
	BOOST_CHECK(hc.isConnected());
	BOOST_CHECK(hc.isLoggedIn());
	BOOST_CHECK_EQUAL(hc.getCookie(),"admin-cookie");
	
	hc.login("localhost:8080","wronguser","wrongpass");
	BOOST_CHECK(hc.isConnected());
	BOOST_CHECK(!hc.isLoggedIn());
	
	hc.login("localhost:8080","admin","passwd");
	BOOST_CHECK(hc.isConnected());
	BOOST_CHECK(hc.isLoggedIn());
	
	hc.login("localhost:8081","admin","passwd");
	BOOST_CHECK(!hc.isConnected());
	BOOST_CHECK(!hc.isLoggedIn());
	
	hc.login("localhost:8080","admin","passwd");
	BOOST_CHECK(hc.isConnected());
	BOOST_CHECK(hc.isLoggedIn());
	//json = std::string("#") + json + "#";
	//json_test = std::string("#") + json_test + "#";
	BOOST_CHECK_EQUAL(hc.Query("GET","/api/webserver/SesTokInfo"), json1_test);
	BOOST_CHECK_EQUAL(hc.Query("POST","/api/user/login"), json2_test);

}
