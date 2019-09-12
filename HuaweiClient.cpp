#include "HuaweiClient.hpp"
#include "Simple-Web-Server/client_http.hpp"
#include "Simple-Web-Server/crypto.hpp"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <sstream>

using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
using Crypto = SimpleWeb::Crypto;
using namespace boost::property_tree;

//------------------------------------- HuaweiAuth
std::string HuaweiAuth::encodeWithToken(const std::string &token){
	std::ostringstream ss;
	ss << user << Crypto::Base64::encode(Crypto::to_hex_string(Crypto::sha256(password))) << token;
	std::string passString=Crypto::Base64::encode(Crypto::to_hex_string(Crypto::sha256(ss.str())));
	return passString;
}

std::string HuaweiAuth::getXmlWithToken(const std::string &token){
	std::ostringstream xml;
	xml	<< "<?xml version 1.0 encoding=UTF-8?><request><Username>" \
		<< user << "</Username><Password>" << encodeWithToken(token) << \
		"</Password><password_type>4</password_type></request>";
	return xml.str();
}
//------------------------------------- HuaweiClient
void HuaweiClient::login(const std::string &srv,const std::string &user,const std::string &password){
	server = srv;
	cookie = "";
	auth = HuaweiAuth(user,password);
	login();
}

HuaweiClient::~HuaweiClient(){
}

void HuaweiClient::login(){
	HttpClient client(server);
	connected = false;
	loggedIn = false;
	try {
		BOOST_LOG_TRIVIAL(info) << "Logging with pass: " << auth.password ;
		auto r1 = client.request("GET", "/api/webserver/SesTokInfo");
		ptree pt;
		read_xml(r1->content, pt, xml_parser::trim_whitespace);
		std::string token = pt.get("response.TokInfo","");
		std::string session = pt.get("response.SesInfo","");
		SimpleWeb::CaseInsensitiveMultimap header;
		header.emplace("__RequestVerificationToken", token);
		header.emplace("Cookie", session);
		std::string xml = auth.getXmlWithToken(token);
		auto r2 = client.request("POST", "/api/user/login",xml, header);
		connected = true;
		read_xml(r2->content, pt, xml_parser::trim_whitespace);
		auto setCookie = r2->header.find("Set-Cookie");
		if(setCookie != r2->header.end()) cookie = setCookie->second;
		if(pt.get("response","") == "OK") loggedIn = true;
		else auth.confirmedWrong = true;
	}
	catch(const SimpleWeb::system_error &e) {
		BOOST_LOG_TRIVIAL(error) << "Client request error: " << e.what() ;
	}
}


std::string HuaweiClient::Query(const std::string &method, const std::string &path, const std::string &data){
	HttpClient client(server);
	try {
		SimpleWeb::CaseInsensitiveMultimap header;
		header.emplace("Cookie", cookie);
		auto r1 = client.request(method,path,data,header);
		ptree pt;
		read_xml(r1->content, pt, xml_parser::trim_whitespace);
		if(pt.get("error.code",0) == 125002 && !auth.confirmedWrong){
			login();
			r1 = client.request(method,path,data,header);
		}
		std::stringstream ss;
		write_json(ss, pt);
		return ss.str();
	}
	catch(const SimpleWeb::system_error &e) {
		BOOST_LOG_TRIVIAL(error) << "Client request error: " << e.what() ;
		return "Client request error:";
	}
}
