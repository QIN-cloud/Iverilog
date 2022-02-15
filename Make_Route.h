#ifndef __Make_Route_H
#define __Make_Route_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#define MAX_LINE_NUM 150

typedef struct Cfg_Node;

struct Route_Node
{
	unsigned lineno;
	int index;
	int succ_no;			//后继路径条数
	int pass_no;			//已经走过的路径条数
	Cfg_Node* cfg_n;
};

class Route_Nodes
{
public:

	Route_Node node[MAX_LINE_NUM];
	int node_ptr;

	bool push(Route_Node&);
	bool pop(Route_Node&);

	bool node_exist(int);
	bool set_node_pass_no(int,int);

	bool pass_all(void);		//检查栈顶节点的所有后继都被遍历
	bool pass_all(int);			//检查栈中某一节点的所有后继都被遍历
	int get_node_pass(int);

	ostream& dump(ostream& o);

	void output(ofstream&, unsigned long, string&);
	
	Route_Nodes(void);
	~Route_Nodes(void);



};





#endif