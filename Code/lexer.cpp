#include <string>
#include <iostream>
#include <vector>
#include "parser_tree.h"
#include <tuple>
#include "lexer.h"


TOKEN_TYPE TOKEN_LIST;

std::string tokenizer(TOKEN_TYPE& TOKEN_LIST, const std::string& RAW_CODE){
    std::string NEXT;
    std::string CURRENT;
    int LINE_NUMBER = 1;
    std::string report;
    std::string::size_type code_size = RAW_CODE.size();
    bool string_toggle = false;
    int i = 0;
    int j = 0;
    while (i != code_size && j <= code_size){
        while (RAW_CODE[i] != ' ' || string_toggle == true && i!= code_size ){
            if (RAW_CODE[i] == '"'){
                string_toggle = !string_toggle;
            }
            i++;
        }
        CURRENT = RAW_CODE.substr(j, i-j);
        int linechange = CURRENT.find(R"(\n)");
        if ( linechange != std::string::npos){
            
            NEXT = CURRENT.substr(linechange + 2, CURRENT.size() - linechange);
            CURRENT = CURRENT.substr(0, linechange); 

            categorize(TOKEN_LIST, CURRENT, LINE_NUMBER);
            LINE_NUMBER ++;
            categorize(TOKEN_LIST, NEXT, LINE_NUMBER);
        }
        else{
            categorize(TOKEN_LIST, CURRENT, LINE_NUMBER);
        }

        ++i;
        j = i;
    }

    return report;
};

bool is_keyword(const std::string& word){
    for(int i = 0; i != KEYWORDS_SIZE; i++){
        if (word == KEYWORDS[i]){
            return true;
        }
    }
    return false;
};

bool is_operator(const std::string& word){
    for(int i = 0; i != OPERATORS_SIZE; i++){
        if (word == OPERATORS[i]){
            return true;
        }
    }
    return false;
};


bool is_number(const std::string& word)
{
    std::string::const_iterator it = word.begin();
    while (it != word.end() && (std::isdigit(*it) || *it == '.')) ++it;
    return !word.empty() && it == word.end();
}

void categorize(TOKEN_TYPE& TOKEN_LIST, const std::string& CURRENT, const int& LINE_NUMBER){
    if (is_keyword(CURRENT)){
        TOKEN_LIST.push_back(std::tuple<int, std::string, std::string> {LINE_NUMBER, "KW", CURRENT});
    }
    else if(is_operator(CURRENT)){
        TOKEN_LIST.push_back(std::tuple<int, std::string, std::string> {LINE_NUMBER, "OP", CURRENT});
    }
    else if (CURRENT[0] == '"' && CURRENT[CURRENT.size()-1] == '"'){
        TOKEN_LIST.push_back(std::tuple<int, std::string, std::string> (LINE_NUMBER, "STR", CURRENT));
    }
    else if (is_number(CURRENT)){
        TOKEN_LIST.push_back(std::tuple<int, std::string, std::string> (LINE_NUMBER, "NUM", CURRENT));
    }
    else{
        TOKEN_LIST.push_back(std::tuple<int, std::string, std::string> (LINE_NUMBER, "ID", CURRENT));
    }
}


int main(){
    std::string CODE = R"(DECLARE NUM : INTEGER\nDECLARE NAME : STRING\nNUM = 3.2\nNAME <-- "Keyaan Minhas")";
    tokenizer(TOKEN_LIST, CODE);
    Tree main_tree(TOKEN_LIST);
    main_tree.lines_display();

}