#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "Scanner.h"

struct parseData {
    string data;
    bool check;
    set<int> errors;
};

class parser
{
public:
    bool error;
    parser();
    node* program (vector<Token>);
    std::string draw(node*);
private:
    int count;
    string file;
    void drawTree(node*);
    node* stmt_sequence (vector<Token>);
    node* statement (vector<Token>);
    node* if_stmt (vector<Token>);
    node* repeat_stmt (vector<Token>);
    node* assign_stmt (vector<Token>);
    node* read_stmt (vector<Token>);
    node* write_stmt (vector<Token>);
    node* exp (vector<Token>);
    node* comparison_op (vector<Token>);
    node* simple_exp(vector<Token>);
    node* addop(vector<Token>);
    node* term(vector<Token>);
    node* mulop(vector<Token>);
    node* factor(vector<Token>);
};

bool checkTokens(string);
parseData parseTokens(string);
//void drawParseTree(string);

#endif
