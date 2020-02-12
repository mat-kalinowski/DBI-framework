
#include <stdlib.h>
#include <cstring>
#include <string>

#include "injector.h"

//	usage:  ./prog --pid <pid_num> --lib <lib_name>

int main(int argc, char* argv[]) {

	Injector injector;
	std::string lib_name;
	pid_t pid_num = -1;

	if(argc < 4 ){
		std::cout << "not enough arguments passed !" << std::endl;
		return 0;
	}

	for(int i = 0; i < argc; i ++){
		std::string current_opt = argv[i];

		if(!current_opt.compare("--pid") && i+1 < argc)
			pid_num = atoi(argv[i+1]);
		
		if(!current_opt.compare("--lib") && i+1 < argc)
			lib_name = argv[i+1];
	}

	if(pid_num == -1 || !lib_name.compare("")){
		std::cout << "wrong types of arguments" << std::endl;
		return 0;
	}

	injector.process_hijack(pid_num, lib_name);

	return 0;
}

