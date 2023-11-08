#include <string>
#include <vector>
#include <tuple>
#include <fstream>

#include "lexer.h"
#include "parser_tree.h"



TOKEN_TYPE TOKEN_LIST;


int main(){
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