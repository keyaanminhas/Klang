#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>




typedef std::vector<std::tuple<int, std::string, std::string>> TOKEN_TYPE;


struct node{
    std::tuple<int, std::string, std::string> *dataptr;
    node *leftptr;
    node *rightptr;
    
};

struct base_node{
    std::vector<std::tuple<int, std::string, std::string> *> * root;
    node *start;
};

class Tree{
    private:
    std::vector<std::vector<std::tuple<int, std::string, std::string> *>> m_lines;
    std::vector<base_node *> m_base_nodes;

    short int m_IF_COUNT = 0;
    short int m_WHILE_COUNT = 0;
    std::vector<std::tuple<std::string, std::string>>m_var_list; // VARNAME, VARTYPE
    void _recurse_node(node* current, std::vector<std::tuple<int, std::string, std::string> *>* data);
    void _recurse_base_node(base_node* current);
    void vec_split(std::vector<std::tuple<int, std::string, std::string> *>* arr, int till, std::vector<std::tuple<int, std::string, std::string> *>*  result1, std::vector<std::tuple<int, std::string, std::string> *>*  result2);
    public:
    std::string m_code_generated="#include <iostream>\n#include <string>\nint main(){";
    Tree(TOKEN_TYPE TOKENS);
    void create();
    void lines_display();
    void tree_display();
    ~Tree();


};

#endif