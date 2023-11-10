#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>

#include "parser_tree.h"


/*
INPUT <IDENTIFIER>
OUTPUT <IDENTIFIER> / <EXPRESSION> , <IDENTIFIER> / <EXPRESSION> 

DECLARE <IDENTIFIER> : DATATYPE/KEYWORD

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
        delete m_base_nodes[i];
        for (int x = 0; x != m_lines[i].size(); x++)
        {
            delete m_lines[i][x];
        }
    }
}


void Tree::lines_display(){
    std::cout << std::endl;
    for(int i = 0; i != m_lines.size() ; i++){
        for (int x = 0; x != m_lines[i].size(); x++)
        {
            std::cout << std::get<0>(*m_lines[i][x]) << std::get<1>(*m_lines[i][x]) <<  std::get<2>(*m_lines[i][x]) << "   ";
        }
        std::cout << std::endl;
    }
}


void Tree::create(){
    for (int i = 0; i!= m_lines.size(); i++){
        base_node* current_base = new base_node;
        current_base->root = &m_lines[i];
        m_base_nodes.push_back(current_base);
        _recurse_base_node(current_base);
    }
    // std::cout << m_IF_COUNT;
}

void Tree::vec_split(std::vector<std::tuple<int, std::string, std::string> *>* arr, int till,std::vector<std::tuple<int, std::string, std::string> *>* result1, std::vector<std::tuple<int, std::string, std::string> *>* result2){ 
    for (int i = 0; i != arr->size(); i++){
        if (i>= till){
            result2->push_back((*arr)[i]);
        }
        else{
            result1->push_back((*arr)[i]);   
        }
    }
}

void Tree::_recurse_node(node* current, std::vector<std::tuple<int, std::string, std::string> *>* data){
    // std::cout << "RECURSED" << std::endl;
    for (int i = 0; i != data->size(); i ++){
        std::cout << std::get<2>(*(*data)[i]);//<< "   " << (std::get<2>(*(*data)[i]).size());
    }
    std::cout << std::endl;
    if ((*data).size() == 1){
        current->dataptr = (*data)[0];
        if (std::get<2>(*(current->dataptr)) == "ENDIF"){m_IF_COUNT--;std::cout << "reduced";}
        // std::cout << std::get<2>(*(current->dataptr)) << std::endl;
    }
    else{
        if (std::get<2>(*(*data)[0]) == "INPUT"){

            if (std::get<1>(*(*data)[1]) != "ID") {
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'INPUT' expected an Identifier!\n");
                exit(0);
            }
            else{
                current->leftptr = new node;
                current->rightptr = new node;
                std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
                std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
                vec_split(data, 1, result1, result2);
                _recurse_node(current->leftptr, result1);
                _recurse_node(current->rightptr, result2);
            }
        }
        else if (std::get<2>(*(*data)[0]) == "OUTPUT"){
            for (int i = 1; i!= (*data).size(); i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'OUTPUT' doesnt take Keywords!\n");
                    exit(0);
                }
            }
            current->leftptr = new node;
            current->rightptr = new node;
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);
        }

        else if (std::get<2>(*(*data)[0]) == "DECLARE"){
            if (std::get<1>(*(*data)[(*data).size()-1]) != "KW"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'DECLARE' requires a DataType!\n");
                exit(0);
            }
            if (std::get<2>(*(*data)[(*data).size()-2]) != ":"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'DECLARE' expects ':' before DataType!\n");
                exit(0);
      
            }
            bool comma = false;
            for (int i = 1; i!= (*data).size()-2; i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'DECLARE' doesnt take Keywords!\n");
                    exit(0);
                }

                if (comma == false){
                    if (std::get<1>(*(*data)[i]) != "ID"){
                        std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'DECLARE' expected an Identifier!\n");
                        exit(0);
                    }
                    else{
                        comma = true;
                    }
                }
                else if (comma == true){
                    if (std::get<2>(*(*data)[i]) != ","){
                        std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'DECLARE' expected a comma as a separator!\n");
                        exit(0);
                    }
                    else{
                        comma = false;
                    } 
                }

            }
            current->leftptr = new node;
            current->rightptr = new node;
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);
        }
        else if (std::get<2>(*(*data)[0]) == "IF") 
        {
            if (std::get<2>(*(*data)[data->size() - 1]) != "THEN"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'IF' expected an expression and keyword 'THEN'!\n");
                exit(0);              
            }
            for (int i = 1; i!= (*data).size()-1; i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'IF' got a Keyword instead of an Expression!\n");
                    exit(0);
                }
            }
            m_IF_COUNT ++;
            current->leftptr = new node;
            current->rightptr = new node;
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            result2->erase(result2->end());
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);
        }
    }

}
void Tree::_recurse_base_node(base_node* current){
    (*current).start = new node;
    Tree::_recurse_node(current->start, current->root);
}


