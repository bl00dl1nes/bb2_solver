#include <iostream>
#include <fstream>
#include <cstdlib>
#include "node.h"
#include "field.h"
#include <queue>
#include <cmath>
#include <windows.h>

struct Point {
	Point():x(0),y(0) {}
	Point(uint8_t _x, uint8_t _y):x(_x),y(_y) {}
	uint8_t x, y;
};

using namespace std;
static int clicks = 0;
static int last_clicks = 0;
static bool global_exit = false;
static queue<Point> queuePoints;
static long fieldSize = 0, pointSize = 0;

void inputField(Field *fld) 
{
	do 
	{
		cout << "enter field: " << endl;
		for (int i=0;i<MAX_X;i++)
			for (int j=0;j<MAX_Y;j++)
			{
				uint8_t k;
				cin >> k;
				fld->setValue(i, j, k - '0');
			}
	} while (!fld->isCorrect());
}

void outputField(Field *fld) 
{
	for (int i=0;i<MAX_X;i++) 
	{
		for (int j=0;j<MAX_Y;j++) 
			cout << fld->getValue(i, j) << " ";
		cout << endl;
	}
	cout << endl;
}

void processField(Field *fld)
{
	uint8_t x, y;
	while (!queuePoints.empty()) 
	{
		x = queuePoints.front().x;
		y = queuePoints.front().y;
		queuePoints.pop();
		for (uint8_t i = x - 1; i < MAX_X; --i)
			//if cell isn't zero or five which means empty or must-be-processed-already then increment
			if (fld->getValue(i, y) != 0 && fld->getValue(i, y) != 5) 
			{
				fld->incValue(i, y);
				if (fld->getValue(i, y) == 5)
					queuePoints.push(Point(i, y));
			}
		for (uint8_t j = y - 1; j < MAX_Y; --j)
			if (fld->getValue(x, j) != 0 && fld->getValue(x, j) != 5)
			{
				fld->incValue(x, j);
				if (fld->getValue(x, j) == 5)
					queuePoints.push(Point(x, j));
				break;
			}
		for (uint8_t i = x + 1; i < MAX_X; ++i)
			if (fld->getValue(i, y) != 0 && fld->getValue(i, y) != 5) 
			{
				fld->incValue(i, y);
				if (fld->getValue(i, y) == 5)
					queuePoints.push(Point(i, y));
				break;
			}
		for(uint8_t j = y + 1; j < MAX_Y; ++j)
			if (fld->getValue(x, j) != 0 && fld->getValue(x, j) != 5) 
			{
				fld->incValue(x, j);
				if (fld->getValue(x, j) == 5)
					queuePoints.push(Point(x, j));
				break;
			}
	}
}

bool clickBall(Field *fld, int x, int y)
{
	if (fld->getValue(x, y) != 0) 
	{
		fld->incValue(x, y); //clicking
		if (fld->getValue(x, y) == 5)
			queuePoints.push(Point(x, y));
		processField(fld);
		return true;
	}
	else 
		return false;
}

void depth(node<Field*> *cur_node, node<Point> *cur_coord) 
{
	Field *ptr;
	node<Field*> *proc_node = 0;
 	node<Point> *point_node = 0;
	clicks -= 1;
	for (int i=0;i<MAX_X;++i) {
		for (int j=0;j<MAX_Y;++j) {
			ptr = new Field;
			*ptr = *(cur_node->get_data());
			if (!clickBall(ptr,i,j))
			{			
				delete ptr;
				continue;
			} else {
				proc_node = new node<Field*>(ptr);
				point_node = new node<Point>(Point(i,j));
				fieldSize += sizeof(Field);
				pointSize += sizeof(Point);
				cur_node->append_child(proc_node);
				cur_coord->append_child(point_node);
			}
			if (clicks == 0) 
			{
				if (ptr->isZeros()) 
				{
					global_exit = true;
					break;
				}
			} else 
				depth(proc_node,point_node);
			if (global_exit) break;
		}
		if (global_exit) break;
	}
	if (global_exit) cout << (last_clicks-clicks) << " click: " <<
				 (point_node->get_data().x + 1) << " " <<
				 (point_node->get_data().y + 1) << endl;
	clicks += 1;
}

void outGraph(ofstream &graph, node<Field*> *root, node<Point> *rootc) {
	vector<node<Field*>*>::iterator itrf = root->begin();
	vector<node<Point>*>::iterator itrp = rootc->begin();
	Field *cur = root->get_data();
	graph << root->get_name() << "[label=\"";
	for(int i=0; i<MAX_X; ++i) {
		for(int j=0; j<MAX_Y; ++j)
			graph << (char)(cur->getValue(i, j) + '0') << ",";
		graph << "\\n";
	}
	graph << "\"];\n";
	if (root->get_data()->isZeros()) {
		graph << root->get_name() << "[color=red, style=bold];\n";
	}
	for(; itrf != root->end(); ++itrf, ++itrp) {
		graph << root->get_name() << "->" << (*itrf)->get_name();
		graph << "[label=\"(" << ((*itrp)->get_data().x+1);
		graph << ";" << ((*itrp)->get_data().y+1) << ")\"];\n";
		outGraph(graph,(*itrf),(*itrp));
	}
}

int main(int argc, char **argv) {
	char wait;
	DWORD startTick;
	node<Field*> root;
	node<Point> rootc;
	Field *init = new Field;
	cout << "enter initial values:" << endl;
	inputField(init);
	cout << "clicks:";
	cin >> clicks;
	last_clicks = clicks;
	global_exit = false;
	cout << endl;
	root.set_data(init); //head
	rootc.set_data(Point(0,0));
	startTick = GetTickCount();
	depth(&root,&rootc);
	cout << "processing time: " << (GetTickCount() - startTick)/1000 << " sec" << endl;
	// cout << "fieldSize = " << fieldSize << endl << "pointSize = " << pointSize << endl;
	if (argc >= 2)
	{
		cout << "writing graph.txt" << endl;
		ofstream graph;
		graph.open("graph.txt");
		graph << "digraph g {\n";
		graph << "graph[splines=true];\n";
		outGraph(graph,&root,&rootc);
		graph << "\n}";
		graph.close();
	}
	cout << "enter any integer:";
	cin >> wait;
	return 0;
}

