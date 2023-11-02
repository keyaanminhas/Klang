#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>




typedef std::vector<std::tuple<int, std::string, std::string>> TOKEN_TYPE;


struct node{
    std::tuple<int, std::string, std::string> *dataptr = nullptr;
    node *leftptr = nullptr;
    node *rightptr = nullptr;
    
};

class Tree{
    private:
    std::vector<std::vector<std::tuple<int, std::string, std::string> *>> m_lines;

    public:
    Tree(TOKEN_TYPE TOKENS);
    void create();
    void lines_display();
    void tree_display();
    ~Tree();


};

#endif