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
	int succ_no;			//���·������
	int pass_no;			//�Ѿ��߹���·������
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

	bool pass_all(void);		//���ջ���ڵ�����к�̶�������
	bool pass_all(int);			//���ջ��ĳһ�ڵ�����к�̶�������
	int get_node_pass(int);

	ostream& dump(ostream& o);

	void output(ofstream&, unsigned long, string&);
	
	Route_Nodes(void);
	~Route_Nodes(void);



};





#endif