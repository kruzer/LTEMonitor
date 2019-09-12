#define BOOST_TEST_MODULE AngularServer_Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Simple-Web-Server/client_http.hpp"
#include "Simple-Web-Server/server_http.hpp"
#include "AngularServer.hpp"
#include "HuaweiClient.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
using namespace boost::property_tree;

struct mockClient : public RouterClient{
	int logCount = 0;
	
	bool calLogin = false;
	mockClient(){};
	mockClient(const std::string &, const std::string &, const std::string &) { };
	void login(const std::string &, const std::string &, const std::string &){ calLogin = true; };
	bool isConnected() const {return true;};
	bool isLoggedIn() const {return true;};
	std::string getUserName(){return "";};
	std::string getPassword(){return "";};
	std::string getServer(){return "";};
	std::string Query(const std::string &, const std::string &, const std::string &){return "{ \"fakeresponse\":\"ok\"}";};
};

struct F{
		mockClient mc;
		AngularServer as {mc,8080};
		HttpClient client {"localhost:8080"};
		ptree pt;
		~F(){ as.stop(); }
};

struct G{
		HuaweiClient hc {"fakehost","fakeuser","fakepasswd"};
		AngularServer as {hc,8080};
		HttpClient client {"localhost:8080"};
		ptree pt;
		~G(){ as.stop(); }
};

BOOST_FIXTURE_TEST_CASE( serving_functions , F ){
	auto r1 = client.request("GET", "/config");
	ptree pt;
	read_json(r1->content, pt);
	BOOST_CHECK_EQUAL(pt.get("config.server",""),"");
	BOOST_CHECK_EQUAL(pt.get("config.user",""),"");
	BOOST_CHECK_EQUAL(pt.get("config.password",""),"");
	BOOST_CHECK_EQUAL(mc.calLogin, false);
	r1 = client.request("GET", "/api/test");
	read_json(r1->content, pt);
	BOOST_CHECK_EQUAL(pt.get("fakeresponse",""),"ok");
	auto it = r1->header.find("Access-Control-Allow-Origin");
	BOOST_CHECK_MESSAGE( it != r1->header.end(), "Header should have Origin set");
}

BOOST_FIXTURE_TEST_CASE( basic_init, G ) {
	auto r1 = client.request("GET", "/config");
	read_json(r1->content, pt);
	BOOST_CHECK_EQUAL(pt.get("config.server",""),"fakehost");
	BOOST_CHECK_EQUAL(pt.get("config.user",""),"fakeuser");
	BOOST_CHECK_EQUAL(pt.get("config.password",""),"fakepasswd");
}

BOOST_FIXTURE_TEST_CASE( serving_data, G ) {
	auto r = client.request("GET", "/config");
	read_json(r->content, pt);
	BOOST_CHECK_EQUAL(pt.get("config.server",""),"fakehost");
	BOOST_CHECK_EQUAL(pt.get("config.user",""),"fakeuser");
	BOOST_CHECK_EQUAL(pt.get("config.password",""),"fakepasswd");

	r = client.request("GET","/ping");
	read_json(r->content, pt);
	BOOST_CHECK_EQUAL(pt.get("ping",""),"ok");

	r = client.request("GET","/status");
	read_json(r->content, pt);
	BOOST_CHECK_EQUAL(pt.get("status.connected",""),"false");
	BOOST_CHECK_EQUAL(pt.get("status.loggedin",""),"false");
	
	ptree outpt;
	outpt.put("config.server","new-server");
	outpt.put("config.user","new-user");
	outpt.put("config.password","new-password");
	
	std::stringstream ss;
	write_json(ss, outpt);
	r = client.request("PUT","/config",ss);
	read_json(r->content, pt);
	BOOST_CHECK_EQUAL(pt.get("config.server",""),"new-server");
	BOOST_CHECK_EQUAL(pt.get("config.user",""),"new-user");
	BOOST_CHECK_EQUAL(pt.get("config.password",""),"new-password");

}

