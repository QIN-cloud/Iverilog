#ifndef __NCfg_Node_H
#define __NCfg_Node_H

#include "CfgNode.h"
#include <string>

using namespace std;

class NcfgNode
{
public:
	NcfgNode();
	~NcfgNode();
	
};

class ProgramNode
{
public:
	ProgramNode();
	~ProgramNode();

	unsigned listNo;
	unsigned get_listNo(){ return listNo; }
	void set_listNo(unsigned Noin){ listNo = Noin; }
	
	unsigned lineNo;
	unsigned get_lineNo(){ return lineNo; }
	void set_lineNo(unsigned Noin){ lineNo = Noin; }

	enum shape_t {ellipse, box};
	enum fontsize_t {12};
	enum color_t {green, blue};

	struct Node_label
	{
		string filename;
		unsigned lineNo;
	};
}

class 