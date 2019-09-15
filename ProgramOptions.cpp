#include <iostream>
#include "ProgramOptions.hpp"
#include "Version.h"

namespace po = boost::program_options;

ProgramOptions::ProgramOptions(int argc, const char* argv[]) :
	desc("Options"),proceed(true),preventGui(false) {

		desc.add_options()("help,h", "Display this help message describing all options");
		desc.add_options()("block,b", "Prevent auto starting browser at http://locahost:[port]");
		desc.add_options()("version,v", "Display the version number");
		desc.add_options()("server,s", po::value<std::string>()->default_value("192.168.0.1"), "IP or router's domain name");
		desc.add_options()("user,u", po::value<std::string>()->default_value("admin"), "router's username");
		desc.add_options()("password,p", po::value<std::string>()->default_value("admin"), "password");
		desc.add_options()("port,o", po::value<int>()->default_value(8080), "local server port number");
		try {
			po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
			po::notify(vm);

			if (vm.count("help")) {
				std::cout << "LTE Monitor version " << lte_monitor_VERSION_MAJOR << "." << lte_monitor_VERSION_MINOR << std::endl;
				std::cout << desc;
				proceed = false;
			}

			if (vm.count("version")) {
				std::cout << "LTE Monitor version " << lte_monitor_VERSION_MAJOR << "." << lte_monitor_VERSION_MINOR << std::endl;
				std::cout << "";
				proceed = false;
			}

			if (vm.count("block")) preventGui = true;

			server = vm["server"].as<std::string>();
			user = vm["user"].as<std::string>();
			password = vm["password"].as<std::string>();
			port = vm["port"].as<int>();

		} catch (po::error const &err) {
			std::cerr << "problem: " << err.what() << std::endl;
			proceed = false;
		}
	}

ProgramOptions::~ProgramOptions() {
}

