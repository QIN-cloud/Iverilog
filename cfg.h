#ifndef __CFG_H
#define __CFG_H

#include "svector.h"
#include <string>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

using namespace std;

class PExpr; 
class NetProcTop;
class CoverBitVecArray;

/*Directsucc is the information pointing to one of the next CFG nodes.
It can be devided into the different types according to the current node type.*/
typedef struct directsucc{

	/*Next node id.*/
	int index;
	/*Using in condit nodes other than case nodes.
	Be true if next node is the conditions established branch.
	Be false on the contrary.*/
	bool cond;
	/*Using in case nodes.
	Represents the case conditional expression corresponding to the next node.*/
	set<PExpr*> caseitem;
	/*Using in case nodes.
	Indicates that the next noder correspond to the branch statement of the case.*/
	unsigned caseitemidx;

}directsucc;

/*A node created by a valid Verilog statement.*/
typedef struct Cfg_Node
{
	int lineno;						//Line number.
	svector<directsucc*> dsuc;		//The collection of branch nodes for the next path.
	set<int, less<int> >infls;		//Statements in the scope and next path.		
	set<string> defs, refs;			//Variables using as def or ref.
	string type;					//Type of this node.
	string assign_type;				//Type of assign.
	svector<PExpr*> expr;			//Conditional expression.
	int condid;						//Represents the id of this conditional node in process.
	int exprid;						//-1, not used.
	bool visited;					//false, not used.
}Cfg_Node;

std::ostream& operator << (std::ostream&, const Cfg_Node&);


/*Control Flow Graph represent all paths that a program will traverse during execution.
It represents the possible flow direction of all basic blocks in a process in the form
of a praph, and can also reflect the real-time execution process.*/
typedef struct Cfg{
	svector<Cfg_Node*>* root;          	//All cfgnodes order by cfgnode line number.
	map<unsigned, int> lineno_index;	//Mapping between line number and cfgnode id in the CFG.
	set<string> defs, refs; 			//Variables using as defs and refs in this process.
	unsigned lineno;					//Line number of the first line in CFG's process.
	unsigned id;						//Locatiion of CFG's process in module.  
	bool sync;							//1 for clock driven, 0 for combination driven;
	bool always;                        //1 for always, 0 for others.
	NetProcTop* proc;
    unordered_map<unsigned, unsigned> pp_line;
	map<CoverBitVecArray, unsigned> pp_path;
    unsigned path_num;
    unsigned min_lineno;
    unsigned max_lineno;
}Cfg;

/*A collection of CFG in a module.*/
typedef struct Module_Cfgs
{
	svector<Cfg*>* cfgs;
}Module_Cfgs;

#endif