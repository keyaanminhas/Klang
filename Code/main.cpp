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
    std::cout << working_code;
    tokenizer(TOKEN_LIST, working_code);
    Tree main_tree(TOKEN_LIST);
    main_tree.create();
    // main_tree.lines_display();

    return 0;
}


int main(){
    std::ifstream code_file("code.k");
    std::string working_code;
    std::vector<std::string> code_lines;
    if (code_file.is_open()){
        char letter;
        while (code_file) {
            letter = code_file.get();
            if (letter != '\n') working_code += letter;
            else{
                code_lines.push_back(working_code);
                working_code = "";
            }
        }
    }
    code_lines.push_back(working_code);
    for (int i = 0; i != code_lines.size(); i++){
        std::cout << code_lines[i] << std::endl;
        tokenizer2(TOKEN_LIST, code_lines[i], i+1);}

    std::cout << "cmon";
    TOKEN_LIST.erase(TOKEN_LIST.end());
    std::cout << "cmon2";
    Tree main_tree(TOKEN_LIST);
    main_tree.create();
    main_tree.lines_display();
    return 0;
}