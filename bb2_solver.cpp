#include <iostream>
#include <fstream>
#include <cstdlib>
#include "node.h"

using namespace std;
typedef unsigned char field[6][5]; 
static int clicks = 0;
static int last_clicks = 0;
static bool global_exit = false;

struct Point {
	Point():x(0),y(0) {}
	Point(int _x, int _y):x(_x),y(_y) {}
	int x, y;
};

bool isZeros(field *fld) {
	for(int i=0; i<6; ++i)
		for(int j=0; j<5; ++j)
			if ((*fld)[i][j] != '0') return false;
	return true;
}

bool isCorrect(field *fld) {
	for(int i=0; i<6; ++i)
		for(int j=0; j<5; ++j)
			if ((*fld)[i][j] >= '5') return false;
	return true;
}

void inField(field *fld) {
	char t;
	do {
		for (int i=0;i<6;i++)
			for (int j=0;j<5;j++)
				cin >> (*fld)[i][j];
	} while (!isCorrect(fld));
}

void outField(field *fld) {
	for (int i=0;i<6;i++) {
		for (int j=0;j<5;j++) 
			cout << (*fld)[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

void copyFieldTo(field *dest, field *src) {
	//memcpy?
	for(int i=0; i<6; ++i)
		for(int j=0; j<5; ++j)
			(*dest)[i][j] = (*src)[i][j];
}

Point isHaveFives(field *fld) {
	for(int i=0;i<6;++i)
		for(int j=0;j<5;++j)
			if ((*fld)[i][j] >= '5') return Point(i,j);
	return Point(-1,-1);
}

void processField(field *fld) {
	Point tmp;
	while (true) {
		tmp = isHaveFives(fld);
		if (tmp.x <0 || tmp.y < 0) break;
		(*fld)[tmp.x][tmp.y] = '0';
		int i=tmp.x-1,j=tmp.y-1;
		for(;i>=0; --i)
			//if cell isn't zero or five which means empty or must-be-processed-already then increment
			if (i >= 0 && (*fld)[i][tmp.y] != '0' && (*fld)[i][tmp.y] != '5') {
				(*fld)[i][tmp.y] += 1;
				break;
			}
		for(;j>=0;--j)
			if (j >= 0 && (*fld)[tmp.x][j] != '0' && (*fld)[tmp.x][j] != '5') {
				(*fld)[tmp.x][j] += 1;
				break;
			}
		i=tmp.x+1; j=tmp.y+1;
		for(;i<6;++i) 
			if (i < 6 && (*fld)[i][tmp.y] != '0' && (*fld)[i][tmp.y] != '5') {
				(*fld)[i][tmp.y] += 1;
				break;
			}
		for(;j<5;++j)
			if (j < 5 && (*fld)[tmp.x][j] != '0' && (*fld)[tmp.x][j] != '5') {
				(*fld)[tmp.x][j] += 1;
				break;
			}
	}
}

void clickBall(field *fld, int x, int y) {
	if ((*fld)[x][y] != '0') (*fld)[x][y] += 1; //clicking
	else return;
	processField(fld);
}

field* copyField(field *fld) {
	//new[]?
	field *ptr = (field*) malloc(5*6);
	copyFieldTo(ptr,fld);
	return ptr;
}

bool isEqual(field *fld1, field *fld2) {
	for (int i=0;i<6;++i) {
		for (int j=0;j<5;++j) {
			if ((*fld1)[i][j] != (*fld2)[i][j]) return false;
		}
	}
	return true;
}

void depth(node<field*> *cur_node, node<Point> *cur_coord) {
	field *ptr = 0;
	node<field*> *proc_node = 0;
       	node<Point> *point_node = 0;
	clicks -= 1;
	for (int i=0;i<6;++i) {
		for (int j=0;j<5;++j) {
			ptr = copyField(cur_node->get_data());
			clickBall(ptr,i,j);
			proc_node = new node<field*>(ptr);
			point_node = new node<Point>(Point(i,j));
			if (isEqual(ptr,cur_node->get_data())) { 
				delete ptr;
				delete proc_node;
				delete point_node;
				continue;
			} else {
				cur_node->append_child(proc_node);
				cur_coord->append_child(point_node);
			}
			if (clicks == 0) {
				if (isZeros(ptr)) {
					global_exit = true;
					break;
				}
			} else depth(proc_node,point_node);
			if (global_exit) break;
		}
		if (global_exit) break;
	}
	if (global_exit) cout << (last_clicks-clicks) << " click: " <<
				 (point_node->get_data().x + 1) << " " <<
				 (point_node->get_data().y + 1) << endl;
	clicks += 1;
}

#ifdef GRAPH
void outGraph(ofstream &graph, node<field*> *root, node<Point> *rootc) {
	vector<node<field*>*>::iterator itrf = root->begin();
	vector<node<Point>*>::iterator itrp = rootc->begin();
	field *cur = root->get_data();
	graph << root->get_name() << "[label=\"";
	for(int i=0; i<6; ++i) {
		for(int j=0; j<5; ++j)
			graph << (*cur)[i][j] << ",";
		graph << "\\n";
	}
	graph << "\"];\n";
	if (isZeros(root->get_data())) {
		graph << root->get_name() << "[color=red, style=bold];\n";
	}
	for(; itrf != root->end(); ++itrf, ++itrp) {
		graph << root->get_name() << "->" << (*itrf)->get_name();
		graph << "[label=\"(" << ((*itrp)->get_data().x+1);
		graph << ";" << ((*itrp)->get_data().y+1) << ")\"];\n";
		outGraph(graph,(*itrf),(*itrp));
	}
}
#endif //GRAPH

int main(int argc, char **argv) {
	char wait;
	node<field*> root;
	node<Point> rootc;
	field *init = (field*) malloc(5*6);
	cout << "enter initial values:" << endl;
	cout << "field:" << endl;
	inField(init);
	cout << "clicks:";
	cin >> clicks;
	last_clicks = clicks;
	global_exit = false;
	cout << endl;
	root.set_data(init); //head
	rootc.set_data(Point(0,0));
	depth(&root,&rootc);
	#ifdef GRAPH
	ofstream graph;
	graph.open("graph.txt");
	graph << "digraph g {\n";
	graph << "graph[splines=true];\n";
	outGraph(graph,&root,&rootc);
	graph << "\n}";
	graph.close();
	#endif //GRAPH
	cout << "enter any symbol:";
	cin >> wait;
	return 0;
}

