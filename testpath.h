#ifndef __Testpath_H
#define __Testpath_H

#include <string>
#include <map>
#include <set>
#include <list>
#include <ostream>
#include <fstream>
#include "svector.h"

using namespace std;

class PDesign;
class Design;
class dd_var;
class PExpr;
struct Var;
struct RefVar;
struct cond_expr;
struct Module_Cfgs;
struct assign_expr;

typedef struct cond_time_expr{
	unsigned procid;
	unsigned condid;
	set<RefVar*> refs;
	bool result;
	int itemindex;
}cond_time_expr;

typedef struct ct_result{
	bool result;
	int itemindex;
}ct_result;

typedef struct tpnode{
	int nodeid;
	string type;
	bool usewhich;//false for use cond, true for caseitem;
	set<string> refs, defs;
	bool cond;
	int condid;
	set<PExpr*> caseitem;
	int itemindex;
	int lineno;
}tpnode;

typedef struct testpath
{
	string modulename;
	int procid;
	int preproc;
	set<string> refs, defs;
	svector<tpnode> nodes;
	bool gened;
	bool sync;//1 for clock driven, 0 for combination driven
}testpath;

typedef struct clp_var{
	string basename;
	string clpname;
	string parentname;
	int index;
	int width;
	int msb, lsb;
	string type;
}clp_var;


extern bool operator == (const clp_var&left, const clp_var&right);
extern bool operator <= (const clp_var&left, const clp_var&right);
extern bool operator <  (const clp_var&left, const clp_var&right);

inline bool operator > (const clp_var&left, const clp_var&right)
{ return right < left; }

inline bool operator >= (const clp_var&left, const clp_var&right)
{ return right <= left; }

inline bool operator != (const clp_var&left, const clp_var&right)
{ return (left == right)? false : true; }

class TestGen{
	public:
		TestGen(PDesign*, Design*);
		~TestGen();

		void build_testpath(string);
		void initilize();

		void gen_clp(ofstream&out);

		ostream& dump(ostream&);

	private:
		PDesign* pdesign_;
		Design* design_;
		map<int, map<string, list<testpath> > > testpath_; //cycle, <modulename, testpath>
		map<string, set<Var*> > vartab_;
		map<string, list<cond_expr*> > cetab_;
		map<string, Module_Cfgs*> cfgs_;

		set<RefVar*> used_var_;
		set<clp_var> clp_vars_;
		unsigned temp_idx;
		ofstream output;

	private:
		void read_testpath(string);
		void arrange_testpath();
		void gen_ref_def();
		void sort_testpath();

		void gen_clp_body(ofstream& o);
		void gen_clp_testpath(int, string, list<testpath>&, const set<int>&, ofstream&);
		void dump_vartab();
		void dump_cond_clp(ofstream&, cond_time_expr, const string);
		ofstream& dump_clpvar_define(ofstream& out);
		ofstream& dump_clp_initialize(ofstream& out);
};

#endif