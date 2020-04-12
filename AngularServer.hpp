#ifndef ANGULARSERVER_H
#define ANGULARSERVER_H
#include <string>
#include "RouterClient.hpp"
#include "Simple-Web-Server/server_http.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class AngularServer {
	private:
		std::thread thr;
		RouterClient &rc;
		HttpServer	server;
		SimpleWeb::CaseInsensitiveMultimap out_header;
		void setSingleInHeader(const std::string &key, const std::string &val);
	public:
		AngularServer()=delete;
		AngularServer(const AngularServer &val) = delete;
		//AngularServer(RouterClient &routerClient, const int port);
		AngularServer(RouterClient &routerClient, const int port,const std::function<void()>& afterStart = {});
		/**
		 * Destructor
		 */
		~AngularServer();
		void stop();
		void serveStatusGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		void serveConfigGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		void serveConfigPut(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		void serveApiGet(   std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		void serveStopGet(  std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		void serveResources(  std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		static void servePingGet(std::shared_ptr<HttpServer::Response> res, std::shared_ptr<HttpServer::Request> req);
		static bool endsWith(const std::string& str, const std::string& suffix)
		{
			    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
		};
};

#endif /* ANGULARSERVER_H */
