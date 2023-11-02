#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "parser_tree.h"


/*
DECLARE <IDENTIFIER> : DATATYPE/KEYWORD

INPUT <IDENTIFIER>
OUTPUT <IDENTIFIER> / <EXPRESSION> , <IDENTIFIER> / <EXPRESSION> 

IF <EXPRESSION> THEN
<STATEMENTS> / <EXPRESSIONS>

ELSE
<STATEMENTS> / <EXPRESSIONS>
ENDIF

CASE OF <IDENTIFIER>

{{{
OTHERWISE
ENDCASE
}}}
future updates

FOR IDENTIFIER = <NUM> TO <NUM>
<STATEMENTS> / <EXPRESSIONS>
ENDFOR

REPEAT
<STATEMENTS> / <EXPRESSIONS>
UNTIL <CONDITION>

WHILE <CONDITION>
<STATEMENTS> / <EXPRESSIONS>
ENDWHILE
*/



Tree::Tree(TOKEN_TYPE TOKENS){
    std::vector<std::tuple<int, std::string, std::string> *> vec1;
    TOKEN_TYPE::size_type TOKEN_SIZE= TOKENS.size(); 
    int current = 1;
    for (int i = 0; i != TOKEN_SIZE; i++){
        if (std::get<0>(TOKENS[i]) != current){
            m_lines.push_back(vec1);
            vec1.clear();
            current = std::get<0>(TOKENS[i]);
        }

        std::tuple<int, std::string, std::string>* word = new std::tuple<int, std::string, std::string>(TOKENS[i]);
        // std::cout << std::get<0>(*ex)<< std::get<1>(*ex)<< std::get<2>(*ex) << std::endl;
        vec1.push_back(word);
    }
    m_lines.push_back(vec1);
}

Tree::~Tree(){
    for (int i = 0; i != m_lines.size() ; i++){
        for (int x = 0; x != m_lines[i].size(); x++)
        {
            delete m_lines[i][x];
        }
    }
}


void Tree::lines_display(){
    for(int i = 0; i != m_lines.size() ; i++){
        for (int x = 0; x != m_lines[i].size(); x++)
        {
            std::cout << std::get<0>(*m_lines[i][x]) << std::get<1>(*m_lines[i][x]) << std::get<2>(*m_lines[i][x]);
        }
        std::cout << std::endl;
    }
}


void Tree::create(){
    for (int i = 0; i!= m_lines.size(); i++){
        
    }
}