#include"Make_Route.h"

Route_Nodes::Route_Nodes(void)
{
	node_ptr = 0;
	for(int i = 0; i< MAX_LINE_NUM ;i++)
	{
		node[i].cfg_n = NULL;
		node[i].lineno = 0;
		node[i].pass_no = 0;
		node[i].succ_no = 0;
		node[i].index = -1;
	}
}

Route_Nodes::~Route_Nodes(void)
{

}

int Route_Nodes::get_node_pass(int idx)
{
	int i = 0;
	for(; i < node_ptr ; i++)
		if(node[i].lineno == idx)
			return node[i].pass_no;
	return -1;
}

ostream & Route_Nodes::dump(ostream &o)
{
	int i= 0;
	o << " one path :>>";
	for(; i < node_ptr ; i++)
	{
		o << node[i].lineno << " -> " ;
	}
	o << endl;
	return o;
}

bool Route_Nodes::node_exist(int idx)
{
	int i = 0;
	for(; i < node_ptr ; i++)
		if(node[i].lineno == idx)
			return true;
	return false;
}

bool Route_Nodes::pass_all(int idx)
{
	if(node[idx].succ_no < node[idx].pass_no )
		return true;
	else
		return false;
}

bool Route_Nodes::pass_all(void)
{
	if(node_ptr == 0)
		return false;
	if(node[node_ptr-1].succ_no < node[node_ptr-1].pass_no )
		return true;
	else
		return false;
}

bool Route_Nodes::pop(Route_Node &rn)
{
	if(node_ptr <= 0)
		return false;
	rn.lineno = node[node_ptr - 1].lineno;
	rn.pass_no = node[node_ptr - 1].pass_no;
	rn.succ_no = node[node_ptr - 1].succ_no;
	rn.cfg_n = node[node_ptr - 1].cfg_n;
	rn.index = node[node_ptr - 1].index;
	node_ptr--;
	return true;
}

bool Route_Nodes::push(Route_Node &rn)
{
	if(node_ptr > MAX_LINE_NUM)
		return false;
	node[node_ptr].lineno = rn.lineno;
	node[node_ptr].pass_no = rn.pass_no;
	node[node_ptr].succ_no = rn.succ_no;
	node[node_ptr].cfg_n = rn.cfg_n;
	node[node_ptr].index = rn.index;
	node_ptr++;
	return true;
}

bool Route_Nodes::set_node_pass_no(int lineno,int sn)
{
	int i = 0;
	for(; i < node_ptr ; i++)
		if(node[i].lineno == lineno)
		{
			node[i].pass_no = sn;
			return true;
		}
	return false;
}

void Route_Nodes::output(ofstream& file, unsigned long pathidx, string& out)
{
	int i= 0;
	char tmp[10];
	out = "";
	file << pathidx << " : ";
	for(; i < node_ptr ; i++)
	{
		file << node[i].index << " , " ;
		sprintf(tmp, "%d", node[i].index);
		out = out + tmp + ",";
	}
	file << endl;
}

