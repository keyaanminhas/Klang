#include <string>
#include <iostream>
#include <vector>
#include "parser_tree.h"
#include <tuple>
#include <fstream>
#include "lexer.h"



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




std::string tokenizer2(TOKEN_TYPE& TOKEN_LIST,const std::string& RAW_CODE, const int& line){
    int i = 0;

    bool start_string = false;
    bool start_braces = false;
    bool start_word = false;
    bool start_operator = false;
    bool start_number = false;
    std::string current_word;

    std::vector<std::string> temp_vec;

    while (i <= RAW_CODE.size()){
        // std::cout << RAW_CODE[i];
        if (RAW_CODE[i] == '\n'){i++;continue;}
        if (RAW_CODE[i] == '"'){
            if (current_word != ""){
                if (start_string){
                    current_word+='"';
                    i++;}
                temp_vec.push_back(current_word);
                current_word = "";
            }
            start_word = false;
            start_number = false;
            start_operator = false;
            start_string = !start_string;
        }
        if (start_string == true){
            current_word += RAW_CODE[i];
        }
        else if (RAW_CODE[i] == ' '){
            temp_vec.push_back(current_word);
            current_word = "";
            start_word = false;
            start_number = false;
            start_operator = false;
        }
        else if ((RAW_CODE[i] >= 'A' && RAW_CODE[i] <= 'Z') || (RAW_CODE[i] >= 'a' && RAW_CODE[i] <= 'z')){

            if (start_word == false && start_operator == false && start_number == false){
                start_word = true;
                current_word += RAW_CODE[i];
            }
            else if (start_word == true){
                current_word += RAW_CODE[i];
            }
            else if (start_number == true){
                temp_vec.push_back(current_word);
                current_word = RAW_CODE[i];
                start_number = false;
                start_word = true;
            }
            else if (start_operator == true){
                temp_vec.push_back(current_word);
                current_word = RAW_CODE[i];
                start_operator = false;
                start_word = true;                
            }
            
        }
        else if (isdigit(RAW_CODE[i]) || RAW_CODE[i] == '.'){
            if (start_word == false && start_operator == false && start_number == false){
                start_number = true;
                current_word += RAW_CODE[i];
            }
            else if (start_number == true){
                current_word += RAW_CODE[i];
                
            }
            else if (start_word == true){
                // temp_vec.push_back(current_word);
                current_word += RAW_CODE[i];
                // start_number = true;
                // start_word = false;
            }
            else if (start_operator == true){
                temp_vec.push_back(current_word);
                current_word = RAW_CODE[i];
                start_number = true;
                start_operator = false;                
            }
        }
        else{
            if (start_word == false && start_operator == false && start_number == false){
                start_operator = true;
                current_word += RAW_CODE[i];
            }
            else if (start_operator == true){
                current_word += RAW_CODE[i];
            }
            else if (start_word == true){
                temp_vec.push_back(current_word);
                current_word = RAW_CODE[i];
                start_operator = true;
                start_word = false;
            }
            else if (start_number == true){
                temp_vec.push_back(current_word);
                current_word = RAW_CODE[i];
                start_operator = true;
                start_number = false;                
            }
        }
        i ++;
    }
    temp_vec.push_back(current_word);

    for (int i = 0; i != temp_vec.size()-1; i++){
        if (temp_vec[i] != ""){
            // std::cout << temp_vec[i];
            categorize(TOKEN_LIST, temp_vec[i], line);
        }
    }

    if (temp_vec[temp_vec.size()-1].size() > 1){
        std::string weird_stuff = temp_vec[temp_vec.size()-1].substr(0, temp_vec[temp_vec.size()-1].size()-1);
        categorize(TOKEN_LIST, weird_stuff, line);
    }

    return std::string(" ");
}

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
};
