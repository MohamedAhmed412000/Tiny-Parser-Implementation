#ifndef NODE_H
#define NODE_H

#include "headers.h"

class node
{
public:
	node(std::string, std::string, char);
	void addChild(node*);
	void addSibling(node*);
	void draw(node*);
	void setNum(int in){num = in;}
	int Num(){return num;}
	char shape();
	string type();
	string value();
	node* sibling();
	node* parent();
	vector<node*> children();

private:
	char shp;
	int num;
	std::string typ, val;
    node *r_sib, *p;
	vector<node*> c;
};

#endif
