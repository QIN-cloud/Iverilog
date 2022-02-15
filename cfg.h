#ifndef __CFG_H
#define __CFG_H

#include "svector.h"
#include <map>

class PExpr;

typedef struct RefVar{
	string name;
	unsigned space, time;
	int width;
	int lsb, msb;
	string ptype;
	bool assigntype;//0 for blocking, 1 for non-blocking;
}RefVar;

extern bool operator == (const RefVar&left, const RefVar&right);

typedef struct cond_expr{
	unsigned procid;
	unsigned condid;
	unsigned lineno;
	set<RefVar*> refs;
	svector<PExpr*> expr;
}cond_expr;

typedef struct assign_expr{
	int procid;
	int exprid;
	unsigned lineno;
	string assign_type;
	svector<PExpr*> expr;
}assign_expr;

typedef struct directsucc{
	int index;
	bool cond;
	set<PExpr*> caseitem;
	unsigned caseitemidx;
}directsucc;

typedef struct Cfg_Node
{
		int lineno;
		svector<directsucc*> dsuc;
		set<int, less<int> >infls;
		set<string> defs, refs;
		string type;
		string assign_type;
		svector<PExpr*> expr;
		int condid;
		int exprid;
		bool visited;
}Cfg_Node;

std::ostream& operator << (std::ostream&, const Cfg_Node&);

typedef struct Cfg{
		svector<Cfg_Node*>* root;
		map<unsigned, int> lineno_index;
		set<string> defs, refs;
		unsigned id;
		unsigned lineno;
		bool sync;//1 for clock driven, 0 for combination driven;
}Cfg;

typedef struct Module_Cfgs
{
		svector<Cfg*>* cfgs;
}Module_Cfgs;

#endif