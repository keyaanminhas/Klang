#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "lexer.h"


TOKEN_TYPE TOKEN_LIST;

std::string tokenizer(TOKEN_TYPE& TOKEN_LIST, const std::string& RAW_CODE){
    std::string NEXT;
    std::string CURRENT;
    int LINE_NUMBER;
    std::string report;
    std::string::size_type code_size = RAW_CODE.size();
    int i = 0;
    int j = 0;
    while (i != code_size && j <= code_size){
        while (RAW_CODE[i] != ' ' && i!= code_size){
            // std::cout << i << " " << j << std::endl;
            // if (((i + 1) != code_size) && (RAW_CODE[i] == '\\') && RAW_CODE[i+1] == 'n'){
            //     break;
            // }
            i++;
        }
        CURRENT = RAW_CODE.substr(j, i-j);
        int linechange = CURRENT.find(R"(\n)");
        if ( linechange != std::string::npos){
            NEXT = CURRENT.substr(linechange + 2, CURRENT.size() - linechange);
            CURRENT = CURRENT.substr(0, linechange); 
            std::cout << CURRENT << is_keyword(CURRENT) << NEXT << is_keyword(NEXT);
        }
        else{
            std::cout << CURRENT << is_keyword(CURRENT);
        }

        ++i;
        j = i;
    }

    return report;
};

bool is_keyword(const std::string& word){
    for(int i = 0; i != KEYWORDS_SIZE; i++){
        // std::cout << KEYWORDS[i] << std::endl;
        if (word == KEYWORDS[i]){
            return true;
        }
    }
    return false;
};

bool is_operator(const std::string& word){
    for(int i = 0; i != OPERATORS_SIZE; i++){
        std::cout << OPERATORS[i] << std::endl;
        if (word == OPERATORS[i]){
            return true;
        }
    }
    return false;
};


int main(){
    std::string CODE = R"(DECLARE NUM : INTEGER\nDECLARE NAME : STRING\nNUM = 3\nNAME <-- "Keyaan")";
    tokenizer(TOKEN_LIST, CODE);
}