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

ELSE IF <EXPRESSION>
ENDIF


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
    std::cout << "\n\n\n\n\n\n";

    std::cout << m_code_generated << std::endl; 

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
        std::cout << std::get<2>(*(*data)[i]) << "   ";// << (std::get<2>(*(*data)[i]).size());
    }
    std::cout << std::endl;
    if ((*data).size() == 1){
        current->dataptr = (*data)[0];
        if (std::get<2>(*(current->dataptr)) == "ENDIF"){m_code_generated += "}\n";m_IF_COUNT--;}
        else if (std::get<2>(*(current->dataptr)) == "ELSE"){m_code_generated += "}\nelse{\n";}
        else if (std::get<2>(*(current->dataptr)) == "NEXT"){m_code_generated += "}\n";}
        else if (std::get<2>(*(current->dataptr)) == "ENDWHILE"){m_code_generated += "}\n";}
        else if (std::get<2>(*(current->dataptr)) == "REPEAT"){m_code_generated += "do{\n";}

        // std::cout << std::get<2>(*(current->dataptr)) << std::endl;
    }
    else{
        if (std::get<2>(*(*data)[0]) == "INPUT"){

            if (std::get<1>(*(*data)[1]) != "ID") {
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'INPUT' expected an Identifier!\n");
                exit(0);
            }
            else{
                bool flag = false;
                for (int i = 0; i!= m_var_list.size(); i++ ){
                    if (std::get<0>(m_var_list[i]) == std::get<2>(*(*data)[1])){
                        flag = true;
                        if (std::get<1>(m_var_list[i]) == "STRING"){m_code_generated += "std::getline(std::cin," + std::string(std::get<0>(m_var_list[i])) + ");\n";}
                        else if (std::get<1>(m_var_list[i]) == "INTEGER" || std::get<1>(m_var_list[i]) == "REAL"){m_code_generated = m_code_generated + "std::cin >> " + std::get<0>(m_var_list[i]) + ";\n";}
                    }
                }
                if (flag == false){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: 'Identifier' was not declared!\n");
                    exit(0);
                }
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
            m_code_generated += "std::cout << ";
            for (int i = 1; i!= (*data).size(); i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'OUTPUT' doesnt take Keywords!\n");
                    exit(0);
                }
                else if (std::get<1>(*(*data)[i]) == "ID"){
                    bool found = false;
                    for (int j = 0; j != m_var_list.size(); j++){
                        if (std::get<0>(m_var_list[j]) == std::get<2>(*(*data)[i])){
                            m_code_generated += std::get<2>(*(*data)[i]);
                            found = true;
                            break;
                        }
                    }
                    if (found == false){
                        std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Variable missing delaration!\n");
                        exit(0);
                    }
                }
                else if (std::get<2>(*(*data)[i]) == ","){
                    m_code_generated = m_code_generated + " << ";

                }
                else{
                    m_code_generated = m_code_generated + std::get<2>(*(*data)[i]);
                }
            }
            m_code_generated = m_code_generated + " << std::endl;\n";
            
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
            

            std::string data_type = std::get<2>(*(*data)[(*data).size()-1]);
            std::cout << data_type << std::endl;
            if (data_type == "INTEGER"){
                m_code_generated += "int ";
            }
            else if (data_type == "STRING"){
                m_code_generated += "std::string ";
            }
            else if (data_type == "REAL"){
                m_code_generated += "float ";
            }
            else if (data_type == "BOOLEAN"){
                m_code_generated += "bool ";
            }
            else if (data_type == "CHAR"){
                m_code_generated += "char ";
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
                        bool found = false;
                        for (int j = 0; j != m_var_list.size(); j++){
                            if (std::get<0>(m_var_list[j]) == std::get<2>(*(*data)[0])){
                                m_code_generated += std::get<2>(*(*data)[0]);
                                found = true;
                                break;
                            }
                        }
                        if (found == true){
                            std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Redelaration not allowed!\n");
                            exit(0);
                        }
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
                m_code_generated += std::get<2>(*(*data)[i]);
                if (std::get<2>(*(*data)[i]) != ","){
                    m_var_list.push_back(std::tuple<std::string, std::string>{std::get<2>(*(*data)[i]), data_type});
                }
            }
            m_code_generated += ";\n";
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
            m_code_generated += "if (";
            for (int i = 1; i!= (*data).size()-1; i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'IF' got a Keyword instead of an Expression!\n");
                    exit(0);
                }
                if (std::get<2>(*(*data)[i]) == "<>"){
                    m_code_generated += "!=";
                }
                else if (std::get<2>(*(*data)[i]) == "="){
                    m_code_generated += "==";
                }
                else{m_code_generated += std::get<2>(*(*data)[i]);}
            }
            m_code_generated += "){\n";
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
        else if (std::get<2>(*(*data)[0]) == "ELSE" && std::get<2>(*(*data)[1]) == "IF" ) 
        {
            if (std::get<2>(*(*data)[data->size() - 1]) != "THEN"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'ELSE IF' expected an expression and Keyword 'THEN'!\n");
                exit(0);              
            }
            m_code_generated += "}\nelse if (";
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            for (int i = 2; i!= (*data).size()-1; i++){
                if (std::get<1>(*(*data)[i]) == "KW"){
                    std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'ELSE IF' got a Keyword instead of an Expression!\n");
                    exit(0);
                }
                else{
                    if (std::get<2>(*(*data)[i]) == "<>"){
                        m_code_generated += "!=";
                    }
                    else if (std::get<2>(*(*data)[i]) == "="){
                        m_code_generated += "==";
                    }
                    else{m_code_generated += std::get<2>(*(*data)[i]);}
                        result1->push_back((*data)[i]);
                }
            }
            m_code_generated += "){\n";
            current->leftptr = new node;
            current->rightptr = new node;

            current->leftptr->dataptr = ((*data)[0]);
            current->rightptr->leftptr = new node;
            current->rightptr->leftptr->dataptr = ((*data)[1]);
            current->rightptr->rightptr = new node;



            _recurse_node(current->rightptr->rightptr, result1);
        }
        else if (std::get<2>(*(*data)[0]) == "ELSE" && std::get<2>(*(*data)[1]) != "IF" ){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'ELSE' did not expect an Expression!\n");
                exit(0);  
        }
        else if (std::get<2>(*(*data)[0]) == "FOR"){
            if (std::get<1>(*(*data)[1]) != "ID"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'FOR' expected an Identifier!\n");
                exit(0);      
            }

            m_var_list.push_back(std::tuple<std::string, std::string>{std::get<2>(*(*data)[1]), "INTEGER"});
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            
            current->leftptr = new node;
            current->rightptr = new node;
            vec_split(data, 1, result1, result2);
            _recurse_node(current->leftptr, result1);

            bool step = false;


            for (int i = 0; i != result2->size() -1; i++){
                if (std::get<2>(*(*result2)[i]) == "STEP"){
                    std::vector<std::tuple<int, std::string, std::string> *> * result3 = new std::vector<std::tuple<int, std::string, std::string> *>;
                    std::vector<std::tuple<int, std::string, std::string> *> * result4 = new std::vector<std::tuple<int, std::string, std::string> *>;
                    vec_split(result2, i, result3, result4);
                    current->rightptr->leftptr = new node;
                    current->rightptr->rightptr = new node;
                    _recurse_node(current->rightptr->leftptr, result3);
                    _recurse_node(current->rightptr->rightptr, result4);

                    step = true;
                    break;
                }
            }
            std::string start = " (";
            std::string end = " (";
            std::string stepstr = " (";
            int i = 3;

            for (; i!= (*data).size()-1; i++){
                if (std::get<2>(*(*data)[i]) == "TO"){
                    break;
                }
                start += std::get<2>(*(*data)[i]) + " ";
            }

            i +=1;
            start += ")";

            if (step == false){
                for (; i!= (*data).size(); i++){
                    end += std::get<2>(*(*data)[i]) + " ";
                }
                end += ")";
                m_code_generated += "for (int " + std::get<2>(*(*data)[1]) + " = " + start + "; " + std::get<2>(*(*data)[1]) + " != " + "(" +  start + ">" + end + " ? " + end + " -1 : " + end + " +1 " + ")" + "; " + std::get<2>(*(*data)[1]) + " += 1){\n";
                _recurse_node(current-> rightptr, result2);
            }
            else{
                for (; i!= (*data).size()-1; i++){
                    if (std::get<2>(*(*data)[i]) == "STEP"){
                        break;
                    }
                    end += std::get<2>(*(*data)[i]) + " ";
                }
                i+=1;
                end += ")";
                for (; i!= (*data).size(); i++){
                    stepstr += std::get<2>(*(*data)[i]) + " ";
                }
                stepstr += ")";
                m_code_generated += "for (int " + std::get<2>(*(*data)[1]) + " = " + start + "; " + std::get<2>(*(*data)[1]) + " != " + "(" +  start + ">" + end + " ? " + end + " -1 : " + end + " +1 " + ")" + "; " + std::get<2>(*(*data)[1]) + " += " + stepstr + "){\n";

            }
            // std::cout << "start "<< start << "\n" << "stop " << end << "\n" << "step " << stepstr << "\n" << std::endl;
        }
        else if (std::get<2>(*(*data)[0]) == "STEP"){
            current->rightptr = new node;
            current->leftptr = new node;

            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);

        }        
        else if (std::get<2>(*(*data)[0]) == "WHILE"){

            if (std::get<2>(*(*data)[(*data).size()-1]) != "DO"){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Keyword 'WHILE' expected a KEYWORD 'DO'!\n");
                exit(0);      
            }

            m_code_generated += "while ( ";
            for (int i = 1; i != (*data).size()-1; i++){
                if (std::get<2>(*(*data)[i]) == "<>"){
                    m_code_generated += "!= ";
                }
                else if (std::get<2>(*(*data)[i]) == "="){
                    m_code_generated += "== ";
                }
                else{m_code_generated += std::get<2>(*(*data)[i]) + " ";}
            }

            m_code_generated += "){\n";


            current->rightptr = new node;
            current->leftptr = new node;

            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            result2->erase(result2->end());
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);
        }
        else if (std::get<2>(*(*data)[0]) == "UNTIL"){

            m_code_generated += "} while ( ";
            for (int i = 1; i != (*data).size(); i++){
                if (std::get<2>(*(*data)[i]) == "<>"){
                    m_code_generated += "!= ";
                }
                else if (std::get<2>(*(*data)[i]) == "="){
                    m_code_generated += "== ";
                }
                else{m_code_generated += std::get<2>(*(*data)[i]) + " ";}
            }
            m_code_generated += ");\n";

            current->rightptr = new node;
            current->leftptr = new node;

            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 1, result1, result2);
            _recurse_node(current->leftptr, result1);
            _recurse_node(current->rightptr, result2);
        }
        else if ((std::get<1>(*(*data)[0]) == "ID") && (std::get<2>(*(*data)[1]) == "<--")){
            bool found = false;
            for (int j = 0; j != m_var_list.size(); j++){
                if (std::get<0>(m_var_list[j]) == std::get<2>(*(*data)[0])){
                    m_code_generated += std::get<2>(*(*data)[0]) + " = ";
                    found = true;
                    break;
                }
            }
            if (found == false){
                std::cout << std::string("ERROR -Line[") << std::to_string(std::get<0>(*(*data)[0])) << std::string("]: Variable missing delaration!\n");
                exit(0);
            }
            for (int i = 2; i != (*data).size(); i++){
                if (std::get<2>(*(*data)[i]) == "<>"){
                    m_code_generated += "!= ";
                }
                else if (std::get<2>(*(*data)[i]) == "="){
                    m_code_generated += "== ";
                }
                else{m_code_generated += std::get<2>(*(*data)[i]) + " ";}
            }
            m_code_generated += ";\n";
            current->leftptr = new node;
            current->rightptr = new node;
            current->leftptr->dataptr = (*data)[0];
            current->rightptr->leftptr = new node;
            current->rightptr->leftptr->dataptr = (*data)[1];
            current->rightptr->rightptr = new node;
            std::vector<std::tuple<int, std::string, std::string> *> * result1 = new std::vector<std::tuple<int, std::string, std::string> *>;
            std::vector<std::tuple<int, std::string, std::string> *> * result2 = new std::vector<std::tuple<int, std::string, std::string> *>;
            vec_split(data, 2, result1, result2);            
            _recurse_node(current->rightptr->rightptr, result2);
        }
    }

}
void Tree::_recurse_base_node(base_node* current){
    (*current).start = new node;
    Tree::_recurse_node(current->start, current->root);
}

// void Tree::tree_display(){
// }