#include <string>
#include <vector>
#include <tuple>
#include <fstream>

#include "lexer.h"
#include "parser_tree.h"



TOKEN_TYPE TOKEN_LIST;


int main2(){
    std::ifstream code_file ("code.k");
    std::string working_code;
    if ( code_file.is_open() ) {
        char letter;
        while (code_file) {
        letter = code_file.get();
        if (letter != '\n') working_code += letter;
        else working_code+= "\\n";
        }
    }
    tokenizer(TOKEN_LIST, working_code);
    Tree main_tree(TOKEN_LIST);
    main_tree.create();
    // main_tree.lines_display();

    return 0;
}


int main(){
    std::ifstream code_file("code.k");
    std::vector<std::string> code_lines;
    if (!code_file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1; // Return an error code
    }

    std::string line;
    while (std::getline(code_file, line)) {
        // Process each line as needed
        code_lines.push_back(line);
        
    }

    // Close the file
    code_file.close();


    for (int i = 0; i != code_lines.size();i++){
        if (code_lines[i] != ""){
            tokenizer2(TOKEN_LIST, code_lines[i], i+1);
        } 
    }
    Tree main_tree(TOKEN_LIST);
    // main_tree.lines_display();
    main_tree.create();
    return 0;
}