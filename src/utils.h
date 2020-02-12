#ifndef UTILS
#define UTILS

#include <vector>
#include <string>
#include <cstring>

#include <sys/mman.h>
#include <sys/ptrace.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "regs-defs.h"

struct mem_mapping{
	std::string name;
	unsigned long beg;
	unsigned long end;
};

/*
*
* Class containing utilities for process address space parsing / modificating
*
*/

class Utils {
    public:
        void poke_data(unsigned long, unsigned long*, int, pid_t);
		void val_to_sc(unsigned long value, std::vector<uint8_t> &shellcode);
		void get_addr(pid_t pid_num, std::string name, mem_mapping *mem_el);

		void dbg_stack_dump(unsigned long, unsigned long, pid_t);
		void dbg_rip_dump(unsigned long, pid_t, int);  
};

#endif