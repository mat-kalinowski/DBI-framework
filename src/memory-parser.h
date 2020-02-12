
/*
*
* Class containing proc/<pid>/maps parsing utilities
*
*/

/*struct mem_line {
    std::string name;
    unsigned long beg;
    unsigned long end;
};

class MemoryParser {
    public:
        void MemoryParser();
        int load_pid(pid_t pid);
        void get_stack();
        void get_libc();
        void get_name(std::name lib_name);

    private:
        pid_t pid;
        std::vector<mem_line*> mem_mapping;
        ~MemoryParser();

}

int MemoryParser::load_pid(pid_t pid){
    std::ostringstream os;    
    std::string line;
    mem_line* mapping;

    os << "/proc" << pid_num << "/maps";
    std::ifstream maps(os.str());
    mem_mapping.clear();

	while(std::getline(maps, line)) {
        size_t name_pos = line.find_last_of(' ');

        std::string name = line.substr(name_pos, std::string::npos);
        std::string beg = line.substr(0,12);
		std::string end = line.substr(13,25);

        mapping = new mem_line(stoul(beg, NULL, 16),stoul(end, NULL, 16), name);
	}

    if(mem_mapping.size == 0){
        return 1;
    }

    return 0;
}*/