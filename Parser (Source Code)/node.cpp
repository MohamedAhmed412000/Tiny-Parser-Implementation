#include "node.h"

node::node(string t, string value = "", char s = 'O'){
	typ = t;
	val = value;
	shp = s;
    p = r_sib = NULL;
}
void node::addChild(node* s){
	if (s != NULL) {
		c.push_back(s);
		s->p = this;
	}
}
void node::addSibling(node* p){
    r_sib = p;
}
char node::shape(){
	return this->shp;
}
string node::type(){
	return this->typ;
}
string node::value(){
	return this->val;
}
node* node::sibling(){
	return r_sib;
}
node* node::parent(){
	return p;
}
vector<node*> node::children(){
	return c;
}



