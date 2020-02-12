#ifndef ELF_PARSER
#define ELF_PARSER

#include <elf.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#define clear_stream(f) f.clear(); \
                        f.seekg(0, std::ios_base::beg);


struct Elf64 {
    Elf64_Ehdr elf_header;                  // main ELF file header
    Elf64_Shdr *sh_array;                   // section headers array

    std::vector<Elf64_Sym> symtab;          // symbols
    std::vector<char> sym_names;            // symbol's names
};

/*
*
* Class containing ELF binary format parsing utilities
*
*/

class ElfParser {
    public:
        ElfParser();
        void load_elf(std::string file_name);
        unsigned long get_addr(std::string sym_name);

    private:
        std::string string_from_vector(std::vector<char> &names, int index);
        void load_symbols(std::fstream &file, std::vector<char> &shstr_array);
        Elf64 ef;
};

#endif