#include "utils.h"


/*
*	we have to take indianess into account	
*/

void Utils::val_to_sc(unsigned long value, std::vector<uint8_t> &shellcode){
	union u {
        unsigned long val;
        uint8_t chars[sizeof(long)];
    } d;

  	d.val = value;

	for(int i = 0; i < sizeof(long); i++){
		shellcode.push_back(d.chars[i]);
	}
}


/*
* 	get address of a memory mapping from /proc/<pid>/maps
*/


void Utils::get_addr(pid_t pid_num, std::string name, mem_mapping *mem_el){
	std::ostringstream os;    
    std::string line;

    os << "/proc/" << pid_num << "/maps";
    std::ifstream maps(os.str());

	while(std::getline(maps, line)) {
		
		if(line.find(name) != std::string::npos){
			auto mem = line.substr(0, line.find(' '));

			int split = mem.find('-');
			std::string beg = line.substr(0,split);
			std::string end = line.substr(split+1,line.length());

			mem_el->beg = stoul(beg, NULL, 16);
			mem_el->end = stoul(end, NULL, 16);

			int name_pos = line.find_last_of(" ");
			mem_el->name = line.substr(name_pos+1);
			break;
		}
	}
}

/*
* 	inject code into process adress space
*/

void
Utils::poke_data(unsigned long dest, unsigned long *src, int size, pid_t pid_num){

	union u {
        unsigned long val;
        uint8_t chars[sizeof(long)];

    } d;

	for(int i = 0; i < size/8; i++){
		ptrace(PTRACE_POKEDATA, pid_num, dest, (void*) *src);
		dest += 8;
		src ++;
	}

	int bc = size % 8;

	if(bc != 0){

		d.val = ptrace(PTRACE_PEEKDATA, pid_num, dest, 0);
		
		for(int i = 0; i < bc; i++)
			d.chars[i] = (uint8_t)*(((uint8_t*)src) + i);

		ptrace(PTRACE_POKEDATA, pid_num, dest , d.val);
	}
}

/*
*	debugging utility for printing stack memory segment
*/

void 
Utils::dbg_stack_dump(unsigned long stack_beg, unsigned long stack_end, pid_t pid_num){
	int size = stack_end - stack_beg;

	union u {
        unsigned long val;
        uint8_t chars[sizeof(long)];
    } d;

	for(int i = 0; i < size/sizeof(long); i++){
		d.val = ptrace(PTRACE_PEEKTEXT, pid_num, stack_beg, 0);

		std::cout << "\033[0;31m";
		std::cout << std::hex << stack_beg << "  :  ";
		std::cout << "\033[0m";

		for(int i = 0; i < 8; i++){
			std::cout << std::hex << +d.chars[i];
		}

		std::cout << std::endl;
		stack_beg += 8;
	}
}

/*
*	debugging utility for printing data pointed by instruction pointer
*/

void 
Utils::dbg_rip_dump(unsigned long pc, pid_t pid_num, int size){
	union u {
        unsigned long val;
        uint8_t chars[sizeof(long)];
    } d;

	d.val = pc;

	size += size%8;

	for(int i = 0; i < size; i+=8){
		d.val = ptrace(PTRACE_PEEKTEXT, pid_num, pc + i, 0);
		std::cout << "INSTRUCTION POINTER WORD: ";

		for(int j = 0; j < 8; j++)
			std::cout << std::hex << +d.chars[j];

		std::cout << std::endl;

	}
}