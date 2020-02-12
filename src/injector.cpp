#include "injector.h"

#ifdef __X86__ARCH

std::vector<uint8_t> shellcode = {
  	0x48,0x31,0xc0,
	0x48,0xc7,0xc6,0x01,0x00,0x00,0x00,
  	0x48,0x8d,0x3d,0xc1,0x00,0x00,0x00,
	0xff,0x15,0xb3,0x00,0x00,0x00,
	0x48,0x8b,0x25,0x4c,0x00,0x00,0x00,
	0x48,0x8b,0x1d,0x4d,0x00,0x00,0x00, 
	0x48,0x8b,0x05,0x4e,0x00,0x00,0x00,
	0x48,0x8b,0x0d,0x4f,0x00,0x00,0x00,
	0x48,0x8b,0x15,0x50,0x00,0x00,0x00,
	0x48,0x8b,0x35,0x51,0x00,0x00,0x00,
	0x48,0x8b,0x3d,0x52,0x00,0x00,0x00,
	0x4c,0x8b,0x05,0x53,0x00,0x00,0x00,
	0x4c,0x8b,0x0d,0x54,0x00,0x00,0x00,
	0x4c,0x8b,0x15,0x55,0x00,0x00,0x00,
	0x4c,0x8b,0x1d,0x56,0x00,0x00,0x00,
	0xff,0x25,0x58,0x00,0x00,0x00
};

#endif

#ifdef __ARM32__ARCH

std::vector<uint8_t> shellcode = {
  	0x10, 0xa0, 0x01, 0x21, 0x0e, 0x4a, 
	0x03, 0x23, 0xfe, 0x46, 0x9e, 0x44,
  	0x97, 0x46, 0x05, 0x48, 0x05, 0x49, 
	0x06, 0x4a, 0x06, 0x4b, 0x07, 0x4f,
  	0xbe, 0x46, 0x08, 0x4f, 0xbd, 0x46,
	0x06, 0x4f, 0x38, 0x47, 0xc0, 0x46
};

#endif 

int Injector::process_hijack(pid_t pid_num, std::string lib_name){

	mem_mapping stack, libc, libdl;

    as_utils.get_addr(pid_num, "[stack]", &stack);  	//stack
	as_utils.get_addr(pid_num, "libc-", &libc);  	    //mprotect
	as_utils.get_addr(pid_num, "libdl", &libdl);   		//dlopen

	elf_parser.load_elf(libc.name);
	unsigned long ld_addr = elf_parser.get_addr("mprotect") + libc.beg; 

	elf_parser.load_elf(libdl.name);
	unsigned long dl_addr = elf_parser.get_addr("dlopen") + libdl.beg;

    return this->process_inject(pid_num, lib_name, dl_addr, ld_addr, stack);  
}

#ifdef __X86__ARCH

int 
Injector::process_inject(pid_t pid_num, std::string lib_name, unsigned long dl_addr,
                         unsigned long ld_addr, mem_mapping &stack){

	regs_struct regs;

	int ret = ptrace(PTRACE_ATTACH, pid_num, NULL, NULL);

    	if(ret < 0){
		std::cout << "failed to attach to process, exiting ..." << std::endl;
        	return ret;
    	}

	waitpid(pid_num, NULL, WUNTRACED);
	ptrace(PTRACE_SYSEMU, pid_num, 0, 0);

	waitpid(pid_num, NULL, WUNTRACED);
	ptrace(PTRACE_GETREGS, pid_num, NULL, &regs);

	std::vector<unsigned long> registers = {
		regs.x86_sp, regs.x86_bx, regs.x86_ax, regs.x86_cx,
		regs.x86_dx, regs.x86_si, regs.x86_di, regs.x86_r8,
		regs.x86_r9, regs.x86_r10, regs.x86_r11, regs.x86_ip, dl_addr };

	for(auto &val: registers){
		as_utils.val_to_sc(val, shellcode);
	}

	for(const uint8_t &byte: lib_name){
		shellcode.push_back(byte);
	}

	shellcode.push_back(0x00);

	unsigned long shellcode_addr = regs.x86_sp - shellcode.size();
	regs.x86_sp = shellcode_addr - shellcode_addr % 16 - 8;

	as_utils.poke_data(shellcode_addr, (unsigned long*)&shellcode[0], shellcode.size(), pid_num);
	as_utils.poke_data(regs.x86_sp, (unsigned long*) &shellcode_addr, sizeof(shellcode_addr),pid_num);
	
	regs.x86_ip = ld_addr;
	regs.x86_di = stack.beg;   
	regs.x86_si = stack.end - stack.beg;
	regs.x86_dx = PROT_READ|PROT_WRITE|PROT_EXEC;

	ptrace(PTRACE_SETREGS, pid_num, 0, &regs);
	ptrace(PTRACE_DETACH, pid_num, 0, (void *)SIGCONT);

	return 0;
}

#endif

#ifdef __ARM32__ARCH

int Injector::process_inject(pid_t pid_num, std::string lib_name, unsigned long dl_addr,
                         	 unsigned long ld_addr, mem_mapping &stack) {
	regs_struct regs;

	int ret = ptrace(PTRACE_ATTACH, pid_num, NULL, NULL);

	if(ret < 0){
		return ret;
	}

	waitpid(pid_num, NULL, WUNTRACED);
	//ptrace(PTRACE_SYSCALL, pid_num, 0, 0);
	ptrace(PTRACE_GETREGS, pid_num, NULL, &regs);

	/*
	 * If we are in thumb mode -> switch to ARM mode
	 * We need this mode for mprotect and dlopen
	 */

	if( !(regs.ARM_cpsr & (1 << 5)) ){
		std::cout << "ARM mode: switching to 16 bit THUMB mode" << std::endl;
		regs.ARM_cpsr |= 1<<5;
	}

	std::vector<unsigned long> registers = {
		regs.ARM_r0, regs.ARM_r1, regs.ARM_r2, regs.ARM_r3, 
		regs.ARM_lr, regs.ARM_pc, regs.ARM_sp, dl_addr
	};

	for(auto &val: registers){
		as_utils.val_to_sc(val, shellcode);
	}
	
	for(const uint8_t &byte: lib_name){
		shellcode.push_back(byte);
	}

	shellcode.push_back(0x00);

	unsigned long shellcode_addr = regs.ARM_sp - shellcode.size();
	as_utils.poke_data(shellcode_addr, (unsigned long*)&shellcode[0], shellcode.size(), pid_num);
	
	regs.ARM_r0 = stack.beg;
	regs.ARM_r1 = stack.end - stack.beg; 
	regs.ARM_r2 = PROT_READ|PROT_WRITE|PROT_EXEC;
	regs.ARM_sp = shellcode_addr - shellcode_addr %4;

	//shellcode_addr |= 1; 
	regs.ARM_lr = (shellcode_addr | 1);
	regs.ARM_sp = ld_addr; 

	ptrace(PTRACE_SETREGS, pid_num, 0, &regs);
	ptrace(PTRACE_DETACH, pid_num, 0, (void *)SIGCONT);
}

#endif
