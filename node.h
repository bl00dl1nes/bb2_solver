#ifndef NODE_H
#define NODE_H
#include <vector>
#include <cstdio>
#include <string>

using namespace std;
static int count = 0;

template<typename T>
class node {
	public:
		node();
		node(const node<T>&);
		node(const T&);
	
		void set_data(const T&);
		T& get_data();
		void set_parent(node<T>*);
		node<T>* get_parent();
		const char* get_name();
		
		void append_child(const T&);
		void append_child(node<T> *d);
		typename vector<node<T>*>::iterator begin();
		typename vector<node<T>*>::iterator end();
		node<T>* operator[](int i);
		
	private:
		void make_name();
		string c;
		bool made;
		vector<node<T>*> children;
		node<T> *parent;
		T data;
};

template<typename T>
node<T>::node():parent(0),made(false) { count++; make_name(); }

template<typename T>
node<T>::node(const node<T> &d):parent(0),made(false) {
	for (typename vector<node<T>*>::iterator itr=d.begin();itr != d.end();++itr) 
		children.append_child(*itr);
	data = d.get_data();
	parent = d.get_parent();
	count++;
	make_name();
}

template<typename T>
node<T>::node(const T& d):parent(0),made(false) {
	data = d;
	count++;
	make_name();
}

template<typename T>
void node<T>::make_name() {
	char tmp[20];
	sprintf(tmp,"%c%i",'a',count);
	c = tmp;
}

template<typename T>
void node<T>::set_data(const T& d) {
	data = d;
}

template<typename T>
T& node<T>::get_data() {
	return data;
}

template<typename T>
void node<T>::set_parent(node<T> *p) {
	parent = p;
}

template<typename T>
node<T>* node<T>::get_parent() {
	return parent;
}

template<typename T>
const char* node<T>::get_name() {
	return c.c_str();
}

template<typename T>
void node<T>::append_child(const T& d) {
	node<T> *c = new node<T>(d);
	c->set_parent(this);
	children.push_back(c);
}

template<typename T>
void node<T>::append_child(node<T> *c) {
	c->set_parent(this);
	children.push_back(c);
}

template<typename T>
typename vector<node<T>*>::iterator node<T>::begin() {
	return children.begin();
}

template<typename T>
typename vector<node<T>*>::iterator node<T>::end() {
	return children.end();
}

template<typename T>
node<T>* node<T>::operator[](int i) {
	return children[i];
}


#endif //NODE_H
