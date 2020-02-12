#ifndef INJECTOR
#define INJECTOR

#include <string>
#include <vector>

#include <elf.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "elf-parser.h"
#include "utils.h"
#include "regs-defs.h"

/*
*
* Class containing ELF binary format parsing utilities
*
*/

class Injector {
    public:
        int process_hijack(pid_t, std::string);

    private:
        int process_inject(pid_t, std::string, unsigned long,
                         unsigned long, mem_mapping&);
        ElfParser elf_parser;               // elf parsing utilities
        Utils as_utils;                     // address space utilities
        
};

#endif