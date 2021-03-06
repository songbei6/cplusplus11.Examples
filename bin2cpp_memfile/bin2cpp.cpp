#include<iostream>
#include<string>
#include<fstream>
#include<cctype>

std::string convertFilename(const std::string &input) {
    std::string fpath=input;
    ssize_t pos = fpath.rfind("/");
    std::string text;
    if(pos != std::string::npos)
        text = fpath.substr(pos+1, fpath.length()-pos);
    else
        text = input;
    
    std::string temp;
    for(unsigned int i = 0; i < text.length(); ++i) {
        if(text[i] == '.' || temp[i] == ' ')
            temp += '_';
        else if(isalpha(text[i]) || isdigit(text[i]))
            temp += text[i];
    }
    return temp;
}

int main(int argc, char **argv) {
    if(argc != 4) {
        std::cerr << "Error requires two arguments.\n" << argv[0] << " binarysource outputfile lang\nlang is either c or cpp\n";
        exit(EXIT_FAILURE);
    }
    std::string arg_x = argv[3];
    
    if(arg_x != "c" && arg_x != "cpp") {
        std::cerr << "Invalid language selection please use either c or cpp\n";
        exit(EXIT_FAILURE);
    }
    
    std::fstream file;
    file.open(argv[1], std::ios::in | std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 0;
    }
    std::string filename=argv[2];
    filename += ".h";
    std::fstream outfile;
    outfile.open(filename, std::ios::out);
    if(!outfile.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        file.close();
        return 0;
    }
    std::string filen = convertFilename(argv[1]);
    outfile << "#ifndef " << filen << "__H\n";
    outfile << "#define " << filen << "__H\n\n";
    if(arg_x == "cpp")
    outfile << "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";
    outfile << "extern unsigned char " << filen << "[];\n";
    outfile << "extern unsigned long " << filen << "_length;\n\n";
    if(arg_x == "cpp")
    outfile << "#ifdef __cplusplus\n}\n#endif\n";
    outfile << "\n";
    outfile << "\n\n#endif\n";
    std::string filename_;
    filename_= argv[2];
    filename_+= ".";
    filename_+= arg_x;
    outfile.close();
    outfile.open(filename_, std::ios::out);
    if(!outfile.is_open()) {
        std::cerr << "could not open file: " << filename_ << "\n";
        exit(0);
    }
    outfile << "#include\"" << filename << "\"\n\n";
    file.seekg(0, std::ios::end);
    size_t len = file.tellg();
    file.seekg(0, std::ios::beg);
    outfile << "\nunsigned long " << filen << "_length = 0x" << std::hex << len << ";\n\n";
    outfile << "\nunsigned char " << filen << "[] = {\n";
    unsigned long counter = 0;
    
    char buffer[1024];
    while(!file.eof()) {
        file.read((char*)buffer, 1024);
        for(unsigned int i = 0; i < file.gcount(); ++i) {
            outfile << "0x" << std::hex << (unsigned int)(unsigned char)buffer[i] << ", ";
            ++counter;
            if((counter%15) == 0) outfile << "\n";
            
        }
    }
    file.close();
    outfile << "0x0" << "};\n\n";
    outfile.close();
    std::cout << "Array Variable: " << filen << "\n";
    std::cout << "Array Length Variable: " << filen << "_length\n";
    std::cout << "Outputed: " << filename << " and " << filename_ << "\n";
    return 0;
}

