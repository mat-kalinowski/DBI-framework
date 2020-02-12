#ifndef REGS_DEFS
#define REGS_DEFS

/*
* registers structure used in PTRACE_GETREGS handler in linux kernel /x86/../ptrace.c
*/


#ifdef __ARM32__ARCH

#define REGS_SIZE		18

#define ARM_cpsr        uregs[16]
#define ARM_pc          uregs[15]
#define ARM_lr          uregs[14]
#define ARM_sp          uregs[13]
#define ARM_ip          uregs[12]
#define ARM_fp          uregs[11]
#define ARM_r10         uregs[10]
#define ARM_r9          uregs[9]
#define ARM_r8          uregs[8]
#define ARM_r7          uregs[7]
#define ARM_r6          uregs[6]
#define ARM_r5          uregs[5]
#define ARM_r4          uregs[4]
#define ARM_r3          uregs[3]
#define ARM_r2          uregs[2]
#define ARM_r1          uregs[1]
#define ARM_r0          uregs[0]
#define ARM_ORIG_r0     uregs[17]

#endif

#ifdef __X86__ARCH

#define REGS_SIZE		27

#define x86_r15  		uregs[0]
#define x86_r14			uregs[1]
#define x86_r13			uregs[2]
#define x86_r12			uregs[3]
#define x86_bp			uregs[4]
#define x86_bx			uregs[5]
#define x86_r11			uregs[6]
#define x86_r10			uregs[7]
#define x86_r9			uregs[8]
#define x86_r8			uregs[9]
#define x86_ax			uregs[10]
#define x86_cx			uregs[11]
#define x86_dx			uregs[12]
#define x86_si			uregs[13]
#define x86_di			uregs[14]
#define x86_orig_ax		uregs[15]
#define x86_ip			uregs[16]
#define x86_cs			uregs[17]
#define x86_flags		uregs[18]
#define x86_sp			uregs[19]
#define x86_ss			uregs[20]
#define x86_fs_base		uregs[21]
#define x86_gs_base		uregs[22]
#define x86_ds			uregs[23]
#define x86_es			uregs[24]
#define x86_fs			uregs[25]
#define x86_gs			uregs[26]

#endif

struct regs_struct{
	unsigned long uregs[REGS_SIZE];
};

#endif
