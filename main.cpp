#include "ProgramOptions.hpp"
#include "HuaweiClient.hpp"
#include "AngularServer.hpp"


using namespace std;

int main(int argc, const char * argv[]){
	ProgramOptions po(argc, argv);
	if(!po.proceed){
		std::cout << "exiting ... " << std::endl;
		return 0;
	}
	//HuaweiClient hc("192.168.0.1","admin","aaaaaaA1A1");
	HuaweiClient hc(po.server,po.user,po.password);
	thread login_thread([&hc]{
			cout << "Background login to: " << hc.getServer() << endl;
			hc.login();
			});
	login_thread.detach();

	AngularServer as(hc,po.port);
}

