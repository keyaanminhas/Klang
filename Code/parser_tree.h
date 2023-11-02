#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>




typedef std::vector<std::tuple<int, std::string, std::string>> TOKEN_TYPE;


struct node{
    std::tuple<int, std::string, std::string> *dataptr;
    node *leftptr;
    node *rightptr;
    
};

struct base_node{
    std::vector<std::tuple<int, std::string, std::string> *> * root;
    node *leftptr;
    node *rightptr;

};

class Tree{
    private:
    std::vector<std::vector<std::tuple<int, std::string, std::string> *>> m_lines;
    std::vector<base_node *> base_nodes;
    public:
    Tree(TOKEN_TYPE TOKENS);
    void create();
    void lines_display();
    void tree_display();
    ~Tree();


};

#endif