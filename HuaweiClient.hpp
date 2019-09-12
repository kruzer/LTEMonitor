#ifndef HUAWEICLIENT_H
#define HUAWEICLIENT_H
#include "RouterClient.hpp"

class HuaweiAuth{
	public:
		std::string user;
		std::string password;
		HuaweiAuth(const std::string &user, const std::string &password) : user(user), password(password), confirmedWrong(false) {};
		std::string encodeWithToken(const std::string &Token);
		std::string getXmlWithToken(const std::string &Token);
		bool confirmedWrong;
};	

class HuaweiClient : public RouterClient{
	private:
		std::string server;
		std::string cookie;
		HuaweiAuth auth;
		bool connected;
		bool loggedIn;

	public:
		HuaweiClient(const std::string &server, const std::string &user, const std::string &password) :  server(server), auth(user, password), connected(false), loggedIn(false) {};
		/**
		 * Default Constructor - deleted
		 */
		//HuaweiClient()=delete;
		/**
		 * Copy Constructor - deleted
		 */
		HuaweiClient(const HuaweiClient &val) = delete;
		/**
		 * Destructor
		 */
		~HuaweiClient();
		/**
		 * Login
		 */
		void login(const std::string &server,const std::string &user,const std::string &password);
		void login(); bool isConnected() const {return connected;}
		bool isLoggedIn() const {return loggedIn;}
		std::string getUserName() {return auth.user;};
		std::string getPassword() {return auth.password;};
		std::string getServer() {return server;};
		std::string getCookie() {return cookie;}
		std::string Query(const std::string &method, const std::string &path, const std::string &data = std::string());
};

#endif /* HUAWEICLIENT_H */
