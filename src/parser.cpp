#include "elf-parser.h"

ElfParser::ElfParser(){}

std::string ElfParser::string_from_vector(std::vector<char> &names, int index){
    std::stringstream stream;

    for(int i = index; names[i] != '\0'; i++)
        stream << names[i];

    return stream.str();
}

/*
*
* function loading symbols array and its names into ef object
*
*/

void ElfParser::load_symbols(std::fstream &file, std::vector<char> &str_array){
    Elf64_Shdr *symtab_h = NULL;
    Elf64_Shdr *symstr_h = NULL;

    int size = ef.elf_header.e_shnum;

    for(auto i = 0; i < size; i++){
        std::string sh_name = string_from_vector(str_array,ef.sh_array[i].sh_name);

        if(!sh_name.compare(".symtab"))
            symtab_h = &ef.sh_array[i];
        
        else if(!sh_name.compare(".dynsym") && !symtab_h)
            symtab_h = &ef.sh_array[i];

        else if(!sh_name.compare(".dynstr") && !symstr_h)
            symstr_h = &ef.sh_array[i];

        else if(!sh_name.compare(".symstr"))
            symstr_h = &ef.sh_array[i];
    }

    ef.symtab.resize(symtab_h->sh_size / symtab_h->sh_entsize);
    file.seekg(symtab_h->sh_offset);
    file.read((char*) &ef.symtab[0], symtab_h->sh_size);

    clear_stream(file);

    file.seekg(symstr_h->sh_offset);
    ef.sym_names.reserve(symstr_h->sh_size);
    file.read(&ef.sym_names[0], symstr_h->sh_size);
}

/*
*
* function opening given ELF file and loading its header and section headers into ef object
*
*/

void ElfParser::load_elf(std::string file_name){
    Elf64_Shdr shstrtab;
    std::fstream f;            

    std::cout << "LIB PARSING - ARM: " << file_name << std::endl; 

    f.open(file_name, std::ios::in | std::ios::binary );
    f.read((char*) &ef.elf_header, sizeof(Elf64_Ehdr));

    unsigned long size = ef.elf_header.e_shentsize * ef.elf_header.e_shnum;
    ef.sh_array = new Elf64_Shdr[ef.elf_header.e_shnum];

    f.seekg(ef.elf_header.e_shoff);
    f.read((char*) ef.sh_array, size);
    clear_stream(f);

    shstrtab = ef.sh_array[ef.elf_header.e_shstrndx];

    f.seekg(shstrtab.sh_offset);
    std::vector<char> shstr_arr(shstrtab.sh_size);
    f.read(&shstr_arr[0], shstrtab.sh_size);

    load_symbols(f, shstr_arr);    
    f.close();
}

/*
* function finding symbol address in ELF file symbol section
*
*/

unsigned long ElfParser::get_addr(std::string sym_name){
    unsigned long addr = -1;

    for(auto &symbol : ef.symtab){
        std::string name = string_from_vector(ef.sym_names, symbol.st_name);

        if(name.find(sym_name) != std::string::npos){
            addr = symbol.st_value;
        }
    }

    return addr;
}