#include "Parser.h"
#include <set>

unsigned int Parser_index = 0;
set<int> err;

vector<Token> readTokens(string input){
    vector<Token> list;
	Token curr;
    bool type = false, check = false;
	string x = "";
    int count = 0;
    for (unsigned int i = 0; i < input.length(); i++) {
        if(input[i] == ',')
            check = true;
        else if (input[i] == '\n' || input[i] == '\t')
            input[i] = ' ';
    }
    if (check) {
        for (unsigned int i = 0; i < input.length(); i++) {
            if (input[i] == ' ' || input[i] == ',') {
                if (type) {
                    curr.Type = x;
                    list.push_back(curr);
                    count++;
                }
                else {
                    curr.Value = x;
                    count++;
                }
                type = !type;
                x = "";
                while (input[i] == ' ' || input[i] == ',')
                    i++;
                i--;
            }
            else {
                x += input[i];
            }
        }
    }
    return list;
}
void match(string in, vector<Token>list, bool& error){
    if (in == list[Parser_index].Type) {
        // qDebug() << QString::fromStdString(list[Parser_index].Type) << " - " << QString::fromStdString(list[Parser_index].Type);
        Parser_index++;
	}
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("match error" + to_string(Parser_index));
	}
}

parser::parser(){
    count = 0;
    file = "";
    error = false;
}
node* parser::program(vector<Token>list) {
    node* root = stmt_sequence(list);
    if (Parser_index < list.size()) {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("program error" + to_string(Parser_index) + to_string(Parser_index));
    }
    return root;
}
node* parser::stmt_sequence (vector<Token>list) {
    node* c1 = statement(list), *cur = c1;
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    while (x.Type == "SEMICOLON" && Parser_index != list.size()-1) {
        match("SEMICOLON", list, error);
        cur->addSibling(statement(list));
        cur = cur->sibling();
        if (Parser_index < list.size())
            x = list[Parser_index];
        else
            break;
    }
    return c1;
}
std::string parser::draw(node* root){
    file += "digraph structs {\n";
    file += "\tedge [arrowhead = none]\n";
    drawTree(root);
    file += "}";
    return file;
}
void parser::drawTree(node* root){
    string shape = root->shape() == 'O' ? "oval" : "box";
    root->setNum(count);
    file += "\ts" + to_string(count) + " [label=\"" + root->type();
    if (root->value() != "")
        file += "&#92;n(" + root->value() + ")";
    file += "\" shape=\"" + shape + "\"];\n";

    //cout << shape << " : " << root->type() << " " << root->value() << endl;
    vector<node*> ch = root->children();
    if (ch.size() > 0) {
        for (unsigned int i = 0; i < ch.size(); i++) {
            file += "\ts" + to_string(ch[i]->parent()->Num()) + " -> s" + to_string(count+1);
            if (i == 2)
                file += " [style = dashed]";
            file += ";\n";
            count++;
            drawTree(ch[i]);
        }
        if (ch.size() > 1)
        {
            file += "\t{rank = same; ";
            for (unsigned int i = 0; i < ch.size(); i++) {
                file += "s" + to_string(ch[i]->Num());
                if (i != ch.size()-1)
                    file += "->";
            }
            file += " [style = invis];}\n";
            for (unsigned int i = 0; i < ch.size()-1; i++) {
                node* c = ch[i];
                while (c->sibling() != NULL)
                    c = c->sibling();
                file += "\t{rank = same; s" + to_string(c->Num()) + " -> s" + to_string(ch[i+1]->Num()) + " [style = invis];}\n";
            }
        }
    }
    if (root->sibling() != NULL) {
        file += "\ts" + to_string(root->Num()) + " -> s" + to_string(count+1) + ";\n";
        file += "\t{rank=same; s" + to_string(root->Num()) + "; s" + to_string(count+1) + ";}\n";
        count++;
        drawTree(root->sibling());
    }

}
node* parser::statement (vector<Token>list) {
    node* cur = NULL;
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    if (x.Type == "IF")
        cur = if_stmt(list);
    else if (x.Type == "REPEAT")
        cur = repeat_stmt(list);
    else if (x.Type == "IDENTIFIER")
        cur = assign_stmt(list);
    else if (x.Type == "READ")
        cur = read_stmt(list);
    else if (x.Type == "WRITE")
        cur = write_stmt(list);
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("statement error" + to_string(Parser_index));
    }
    return cur;
}
node* parser::if_stmt (vector<Token>list) {
    node *cur = new node("IF", "", 'R');
    match("IF", list, error);
    cur->addChild(exp(list));
    match("THEN", list, error);
    cur->addChild(stmt_sequence(list));
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    if (x.Type == "ELSE") {
        match("ELSE", list, error);
        cur->addChild(stmt_sequence(list));
    }
    match("END", list, error);
    return cur;
}
node* parser::repeat_stmt (vector<Token>list) {
    node *cur = new node("REPEAT", "", 'R');
    match("REPEAT", list, error);
    cur->addChild(stmt_sequence(list));
    match("UNTIL", list, error);
    cur->addChild(exp(list));
    return cur;
}
node* parser::assign_stmt (vector<Token>list) {
    node *cur = new node("ASSIGN", list[Parser_index].Value, 'R');
    match("IDENTIFIER", list, error);
    match("ASSIGN", list, error);
    cur->addChild(exp(list));
    return cur;
}
node* parser::read_stmt (vector<Token>list) {
    match("READ", list, error);
    node *cur = new node("READ", list[Parser_index].Value, 'R');
    match("IDENTIFIER", list, error);
    return cur;
}
node* parser::write_stmt (vector<Token>list) {
    node *cur = new node("WRITE", "", 'R');
    match("WRITE", list, error);
    cur->addChild(exp(list));
    return cur;
}
node* parser::exp (vector<Token>list) {
    node *cur, *c1 = simple_exp(list), *c2;
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    if (x.Type == "LESSTHAN" || x.Type == "EQUAL") {
        cur = comparison_op(list);
        cur->addChild(c1);
        c2 = simple_exp(list);
        cur->addChild(c2);
        c1 = cur;
    }
    return c1;
}
node* parser::comparison_op (vector<Token>list) {
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    node* cur = NULL;
    if (x.Type == "LESSTHAN") {
        cur = new node("OP", "<", 'O');
        match("LESSTHAN", list, error);
    }
    else if (x.Type == "EQUAL") {
        cur = new node("OP", "=", 'O');
        match("EQUAL", list, error);
    }
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("comparison error" + to_string(Parser_index));
    }
    return cur;
}
node* parser::simple_exp(vector<Token>list) {
    node *cur, *c1 = term(list), *c2;
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    while (x.Type == "PLUS" || x.Type == "MINUS") {
        cur = addop(list);
        cur->addChild(c1);
        c2 = term(list);
        cur->addChild(c2);
        c1 = cur;
        if (Parser_index < list.size())
            x = list[Parser_index];
        else
            break;
    }
    return c1;
}
node* parser::addop(vector<Token>list) {
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    node* cur = NULL;
    if (x.Type == "PLUS") {
        cur = new node("OP", "+", 'O');
        match("PLUS", list, error);
    }
    else if (x.Type == "MINUS") {
        cur = new node("OP", "-", 'O');
        match("MINUS", list, error);
    }
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("addop error" + to_string(Parser_index));
    }
    return cur;
}
node* parser::term(vector<Token>list) {
    node *cur, *c1 = factor(list), *c2;
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    while (x.Type == "MULT" || x.Type == "DIV") {
        cur = mulop(list);
        cur->addChild(c1);
        c2 = factor(list);
        cur->addChild(c2);
        c1 = cur;
        if (Parser_index < list.size())
            x = list[Parser_index];
        else
            break;
    }
    return c1;
}
node* parser::mulop(vector<Token>list) {
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    node* cur = NULL;
    if (x.Type == "MULT") {
        cur = new node("OP", "*", 'O');
        match("MULT", list, error);
    }
    else if (x.Type == "DIV") {
        cur = new node("OP", "/", 'O');
        match("DIV", list, error);
    }
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("mulop error" + to_string(Parser_index));
    }
    return cur;
}
node* parser::factor(vector<Token>list) {
    Token x;
    if (Parser_index < list.size())
        x = list[Parser_index];
    node* cur = NULL;
    if (x.Type == "OPENBRACKET") {
        match("OPENBRACKET", list, error);
        cur = exp(list);
        match("CLOSEDBRACKET", list, error);
    }
    else if (x.Type == "NUMBER") {
        cur = new node("Const", x.Value, 'O');
        match("NUMBER", list, error);
    }
    else if (x.Type == "IDENTIFIER") {
        cur = new node("ID", x.Value, 'O');
        match("IDENTIFIER", list, error);
    }
    else {
        error = true;
        err.insert(Parser_index);
        // qDebug() << QString::fromStdString("factor error" + to_string(Parser_index));
    }
    return cur;
}

parseData parseTokens(string tokens) {
    // qDebug() << QString::fromStdString(tokens);
    tokens += "\n";
    vector<Token> list = readTokens(tokens);
    node* root;
    parser p;
    parseData output;
    root = p.program(list);
    output.check = p.error;
    // qDebug() << QString::fromStdString("Finished");
    if (!p.error)
        output.data = p.draw(root);
    else {
        output.data = "";
        output.errors.operator =(err);
    }
    Parser_index = 0;
    err.clear();
    return output;
}
