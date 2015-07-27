#include "SQLParser.h"
#include "bison_parser.h"
#include "flex_lexer.h"
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <fstream>



namespace hsql {
std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &ob)
{
    stream << "std::vector<std::string>: " << std::endl;
    for (size_t i = 0, size = ob.size(); i < size; ++i) {
        stream << i << ": " << ob[i] << std::endl;
    }
    return stream;
}

std::vector<std::string> split(const std::string &s, const std::string &delims = " ")
{
    std::vector<std::string> res;
    std::string cur_str;
    for (size_t i = 0, size = s.length(); i < size; ++i) { 
        if (delims.find(s[i]) != delims.npos) {
            if (cur_str.length()) res.push_back(cur_str);
            cur_str.clear();
        } else {
            cur_str += s[i];
        }
    }
    if (cur_str.length()) res.push_back(cur_str);
    return res;
}

std::vector<std::string> read_key_words()
{
    std::vector<std::string> key_words;
    std::ifstream stream("./src/parser/sql_keywords.txt");
    if (!stream.is_open()) {
        std::cout << "file with kewords didn't open" << std::endl;
        return key_words;
    }
    std::string line;
    while (!stream.eof()) {
        getline(stream, line);
        if (line.find("//") != line.npos) {
            continue;
        }
        auto words = split(line);
        if (words.size() <= 0) continue;
        key_words.push_back(split(line)[0]);
    }
    return key_words;
}

std::string ReplaceSQLTokens(const char *text)
{
    if (text == NULL) return std::string();
    std::string res(text);

    std::vector<std::string> key_words;
    std::ifstream stream("./src/parser/sql_keywords.txt");
    if (!stream.is_open()) {
        std::cout << "file with kewords didn't open" << std::endl;
        return res;
    }
    std::string line;
    while (!stream.eof()) {
        getline(stream, line);
        if (line.find("//") != line.npos) {
            continue;
        }
        auto words = split(line);
        if (words.size() <= 0) continue;
        key_words.push_back(split(line)[0]);
    }
    std::cout << key_words << std::endl;
    return res;
}

SQLParser::SQLParser() {
	fprintf(stderr, "SQLParser only has static methods atm! Do not initialize!\n");
}


SQLStatementList* SQLParser::parseSQLString(const char *text) {
    SQLStatementList* result = NULL;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (hsql_lex_init(&scanner)) {
        // couldn't initialize
        fprintf(stderr, "[Error] SQLParser: Error when initializing lexer!\n");
        return NULL;
    }


    state = hsql__scan_string(text, scanner);

    if (hsql_parse(&result, scanner)) {
        // Returns an error stmt object
        return result;
    }

    hsql__delete_buffer(state, scanner);

    hsql_lex_destroy(scanner);

    return result;
}

SQLStatementList* SQLParser::parseSQLString(const std::string& text) {
    return parseSQLString(text.c_str());
}

    
} // namespace hsql