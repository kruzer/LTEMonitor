#include "AngularServer.hpp"
#include "Simple-Web-Server/server_http.hpp"
#include "Simple-Web-Server/status_code.hpp"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>
#include <sstream>
#include "AngularResources.h"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using namespace boost::property_tree;

//------------------------------------- AngularServer

AngularServer::AngularServer(RouterClient &routerClient, const int port) : rc(routerClient){
	server.config.port = port;

	//out_header.emplace("Access-Control-Allow-Origin","*");
	server.resource["^/ping"]["GET"] = servePingGet;
	server.resource["^/api/(.+)"]["GET"] = std::bind(&AngularServer::serveApiGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/api/(.+)"]["PUT"] = std::bind(&AngularServer::serveApiGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/api/(.+)"]["POST"] = std::bind(&AngularServer::serveApiGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/api/(.+)"]["DELETE"] = std::bind(&AngularServer::serveApiGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/config"]["GET"] = std::bind(&AngularServer::serveConfigGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/config"]["PUT"] = std::bind(&AngularServer::serveConfigPut, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/config"]["OPTIONS"] = std::bind(&AngularServer::serveConfigPut, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/status"]["GET"] = std::bind(&AngularServer::serveStatusGet, this, std::placeholders::_1, std::placeholders::_2);
	server.resource["^/stop"]["GET"] = std::bind(&AngularServer::serveStopGet, this, std::placeholders::_1, std::placeholders::_2);
	server.default_resource["GET"] = std::bind(&AngularServer::serveResources, this, std::placeholders::_1, std::placeholders::_2);

	thr=std::thread([this]() {
			server.start();
			});
	std::this_thread::sleep_for(std::chrono::microseconds(100));
#ifdef _WIN32
	system("open http://localhost:8080");
#elif __APPLE__
	system("open http://localhost:8080");
#elif __linux__
	system("xdg-open http://localhost:8080");
#elif __unix__
#endif
}

void AngularServer::stop(){
	server.stop();
}

void AngularServer::serveResources(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req ){
	BOOST_LOG_TRIVIAL(info) << "sending resources [" << req->path << "] method " << req->method ;
	std::string url = req->path.erase(0,1);
	BOOST_LOG_TRIVIAL(info) << "url [" << url << "]";
	if(url == "") url = "index.html";
	if(url == "router") url = "index.html";
	if(url == "console") url = "index.html";
	if(url == "config") url = "index.html";
	if(url == "signal") url = "index.html";
	BOOST_LOG_TRIVIAL(info) << "url [" << url << "]";
	auto found =  ResourcesMap.find(url);
	if(found != ResourcesMap.end()){
		if(endsWith(req->path, ".js")) out_header.emplace("Content-Type","application/javascript; charset=UTF-8");
		if(endsWith(req->path, ".png")) out_header.emplace("Content-Type","image/png; charset=UTF-8");
		if(endsWith(req->path, ".css")) out_header.emplace("Content-Type","text/css");
		if(endsWith(url, ".html")) out_header.emplace("Content-Type","text/html");
		BOOST_LOG_TRIVIAL(info) << "found:" << req->path;
		std::string mystr(found->second.second, found->second.second + found->second.first);
		res->write(SimpleWeb::StatusCode::success_ok,mystr,out_header);
	} else {
		BOOST_LOG_TRIVIAL(info) << "not found:" << req->path;
		res->write(SimpleWeb::StatusCode::success_ok,out_header);
	}
}

void AngularServer::serveStatusGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> ){
	BOOST_LOG_TRIVIAL(info) << "sending status ";
	ptree tree;
	tree.put("status.connected", rc.isConnected());
	tree.put("status.loggedin", rc.isLoggedIn());
	std::stringstream ss;
	write_json(ss,tree);
	res->write(ss,out_header);
}
void AngularServer::serveConfigGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> ){
	BOOST_LOG_TRIVIAL(info) << "sending config ";
	ptree tree;
	tree.put("config.server", rc.getServer());
	tree.put("config.user", rc.getUserName());
	tree.put("config.password", rc.getPassword());
	std::stringstream ss;
	write_json(ss,tree);
	res->write(ss,out_header);
}

void AngularServer::serveConfigPut(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req){
	BOOST_LOG_TRIVIAL(info) << "setting new config";
	if(req->method == "OPTIONS"){
		SimpleWeb::CaseInsensitiveMultimap head;
		head.emplace("Access-Control-Allow-Methods","POST, GET, OPTIONS, PUT");
		head.emplace("Access-Control-Allow-Origin","*");
		head.emplace("Access-Control-Allow-Headers","Content-Type, Authorization, X-Requested-With");
		res->write("",head);
		return;
	}
	ptree tree, pt;
	read_json(req->content, pt);
	rc.login(pt.get<std::string>("config.server"),pt.get<std::string>("config.user"),pt.get<std::string>("config.password"));
	tree.put("config.server", rc.getServer());
	tree.put("config.user", rc.getUserName());
	tree.put("config.password", rc.getPassword());
	std::stringstream ss;
	write_json(ss,tree);
	res->write(ss,out_header);
}

void AngularServer::servePingGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> ){
	BOOST_LOG_TRIVIAL(info) << "sending ping";
	ptree pt;
	pt.put("ping", "ok");
	std::stringstream ss;
	write_json(ss,pt);
	res->write(ss);
}

void AngularServer::serveApiGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req){
	//std::string mypath=req->path_match[1].str();
	BOOST_LOG_TRIVIAL(info) << "sending api " << req->path ;
	std::string  r = rc.Query(req->method, req->path, req->content.string());
	res->write(r,out_header);
}

void AngularServer::serveStopGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> ){
	BOOST_LOG_TRIVIAL(info) << "stopping" ;
	res->write("stopping");
	std::this_thread::sleep_for(std::chrono::microseconds(1000));
	stop();
}

AngularServer::~AngularServer(){
	thr.join();
}
