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
	HuaweiClient hc(po.server,po.user,po.password);
	thread login_thread([&hc]{
			cout << "Background login to: " << hc.getServer() << endl;
			hc.login();
			});
	login_thread.detach();

	AngularServer as(hc,po.port, [&po](){
			std::string url="xdg-open http://localhost:";
#ifdef _WIN32
			url="cmd /c start http://localhost:";
#elif __APPLE__
			url="open http://localhost:";
#endif
			if(!po.preventGui) system(url.append(std::to_string(po.port)).c_str());
			});
}
