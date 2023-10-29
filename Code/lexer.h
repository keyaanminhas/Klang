#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <iostream>
#include <utility>



typedef std::vector<std::pair<std::string, std::string>> TOKEN_TYPE;

// LATER UPDATES SHALL INCLUDE 
// - STRUCTS
// - CONSTANTS
// - FUNCTIONS
// - PROCEDURES

const std::vector<std::string> KEYWORDS{"INTEGER", "REAL", "STRING", "BOOLEAN", "CHAR", "DECLARE", "ARRAY", "OF", "INPUT", "OUTPUT", "IF", "THEN", "ELSE", "ENDIF", "CASE", "OTHERWISE", "ENDCASE", "FOR", "TO", "ENDFOR", "REPEAT", "UNTIL", "ENDWHILE", "WHILE"};
const std::vector<std::string> OPERATORS{":","+", "-", "=", "<--", "<", ">", "<=", ">=", "<>", "AND", "OR", "NOT"};

const std::vector<std::string>::size_type KEYWORDS_SIZE = KEYWORDS.size();
const std::vector<std::string>::size_type OPERATORS_SIZE = OPERATORS.size();




std::string tokenizer(TOKEN_TYPE& TOKEN_LIST, const std::string& RAW_CODE);
bool is_keyword(const std::string& word);
bool is_operator(const std::string& word);

#endif