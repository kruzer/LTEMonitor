#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H
#include <string>

struct RouterClient {
		virtual void login(const std::string &server, const std::string &user, const std::string &password) = 0;
		virtual bool isConnected() const = 0;
		virtual bool isLoggedIn() const = 0;
		virtual std::string getUserName() = 0;
		virtual std::string getPassword() = 0;
		virtual std::string getServer() = 0;
		virtual std::string Query(const std::string &method, const std::string &path, const std::string &data) = 0;
};

#endif /* ROUTERCLIENT_H */
