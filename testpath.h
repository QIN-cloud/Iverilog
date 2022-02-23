#ifndef __Testpath_H
#define __Testpath_H
#define SMT_INT 0
#define SMT_BOOL -1
#define SMT_NULL -2
#define SMT_WRONG -3
#define SMT_MAX 64
#include <string>
#include <map>
#include <set>
#include <list>
#include <ostream>
#include <fstream>
#include <unordered_map>
#include "svector.h"

using namespace std;

static unordered_map<char, const char*> bv_arith = {{'+', "bvadd"}, {'-', "bvsub"}, {'*', "bvmul"}, {'/', "bvudiv"}, {'%', "bvurem"}};
static unordered_map<char, const char*> bv_logic = {{'^', "bvxor"}, {'|', "bvor"}, {'&', "bvand"}, {'O', "bvnor"}, {'A', "bvnand"}, {'X', "bvxor"}};
static unordered_map<char, const char*> logic_unary = {{'^', "xor"}, {'|', "or"}, {'&', "and"}, {'O', "or"}, {'A', "and"}, {'X', "xor"}};
static unordered_map<char, const char*> logic_binary = {{'o', "or"}, {'a', "and"}};
static unordered_map<char, const char*> compare = {{'e', "="}, {'E', "==="}, {'n', "distinct"}, {'N', "!=="}};
static unordered_map<char, const char*> bv_compare = {{'>', "bvugt"}, {'<', "bvult"}, {'L', "bvule"}, {'G', "bvuge"}};
static unordered_map<char, const char*> int_compare = {{'>', ">"}, {'<', "<"}, {'L', "<="}, {'G', ">="}};
static unordered_map<char, const char*> others = {{'p', "**"}, {'l', "<<"}, {'r', ">>"}, {'R', ">>>"}, {'m', "min"}, {'M', "max"}};

class PDesign;
class Design;
class PExpr;
class perm_string;
class NetProc;
struct Var;
struct RefVar;
struct cond_expr;
struct Module_Cfgs;

typedef map<int, set<NetProc*> > smt_stats;

typedef struct cond_time_expr{
	unsigned procid;
	unsigned condid;
	set<RefVar*> refs;
	bool result;
	int itemindex;
}cond_time_expr;

typedef struct cond_expr{
	unsigned procid;
	unsigned condid;
	unsigned lineno;
	set<RefVar*> refs;
	svector<PExpr*> expr;
}cond_expr;

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
	bool sync;				//true for clock driven, false for combination driven
}testpath;

typedef struct smt_var{
	string basename;
	string smtname;
	string parentname;
	int index;
	int width;
	int msb, lsb;
	string type;
	bool temp_flag;
}smt_var;

typedef struct RefVar{
	string name;
	unsigned space, time;
	int width;
	int lsb, msb;
	string ptype;
	bool assigntype;//0 for blocking, 1 for non-blocking;
}RefVar;

typedef struct Var{
	string name;
	unsigned width;
	int lsb, msb;
	unsigned space, time;
	string ptype;
	int varidx;
	bool assigntype;
	bool changed;
	int change_count;
}Var;

extern bool operator == (const smt_var&left, const smt_var&right);
extern bool operator <= (const smt_var&left, const smt_var&right);
extern bool operator <  (const smt_var&left, const smt_var&right);

inline bool operator > (const smt_var&left, const smt_var&right)
{ return right < left; }

inline bool operator >= (const smt_var&left, const smt_var&right)
{ return right <= left; }

inline bool operator != (const smt_var&left, const smt_var&right)
{ return (left == right)? false : true; }

/*
* TestGen is using for generate the SMT-LIB2 format file in netlist.
*/
class TestGen{
	public:
		TestGen(PDesign*, Design*);
		~TestGen();
		void gen_smt(string, ofstream&, ofstream&, ofstream&);
		ostream& dump(ostream&);
	private:
		Design*      design_;
		PDesign*     pdesign_;
		set<smt_var> smt_vars_;
		map<perm_string, set<Var*> >                 vartab_;
		map<perm_string, Module_Cfgs*>               cfgs_;
		map<perm_string, list<cond_expr*> >          cetab_;
		map<perm_string, map<int, smt_stats> >       stats_;
		map<int, map<perm_string, list<testpath> > > testpath_;

	private:
		void initilize();
		void build_testpath(string);
		void read_testpath(string);
		void arrange_testpath();
		void gen_ref_def();
		void sort_testpath();
		void gen_stats();
		void gen_smt_testpath(int, perm_string, list<testpath>&, const set<int>&, ofstream&);
		void gen_body(ofstream&);
		void gen_head(ofstream&);
		void gen_tail(ofstream&);
		void dump_vartab();
		void dump_cond_smt(ofstream&, cond_time_expr, const perm_string);
};

#endif