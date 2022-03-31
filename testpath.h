#ifndef __Testpath_H
#define __Testpath_H

#include <string>
#include <map>
#include <set>
#include <list>
#include <ostream>
#include <fstream>
#include <unordered_map>
#include "cfg.h"
#include "svector.h"
#include "StringHeap.h"

#define SMT_INT 0
#define SMT_BOOL -1
#define SMT_NULL -2
#define SMT_WRONG -3
#define SMT_MAX 64

using namespace std;

class PDesign;
class Design;
class PExpr;
class perm_string;
class NetProc;
class Module;
struct RefVar;
struct TestPath;
struct SmtVar;
struct Cfg;
struct Cfg_Node;

typedef map<int, set<NetProc*> > NetStats;
typedef map<string, RefVar*> VarTable;
typedef set<SmtVar*> SVTable;

static unordered_map<char, const char*> SMT_Vec_Add = {{'+', "bvadd"}, {'-', "bvsub"}};
static unordered_map<char, const char*> SMT_Vec_Div = {{'/', "bvudiv"}, {'%', "bvurem"}};
static unordered_map<char, const char*> SMT_Vec_Bits = {{'^', "bvxor"}, {'|', "bvor"}, {'&', "bvand"}, {'O', "bvnor"}, {'A', "bvnand"}, {'X', "bvxor"}};
static unordered_map<char, const char*> SMT_Vec_Comp = {{'>', "bvugt"}, {'<', "bvult"}, {'L', "bvule"}, {'G', "bvuge"}, {'e', "="}, {'n', "distinct"}};
static unordered_map<char, const char*> SMT_Int_Comp = {{'>', ">"}, {'<', "<"}, {'L', "<="}, {'G', ">="}, {'e', "="}, {'n', "distinct"}};
static unordered_map<char, const char*> SMT_Vec_Logic = {{'a', "bvand"}, {'o', "bvor"}};
static unordered_map<char, const char*> SMT_Bool_Logic = {{'a', "and"}, {'o', "or"}};
static unordered_map<char, const char*> SMT_Vec_Mult = {{'*', "bvmul"}};


typedef struct TPNode{
	bool usewhich;         //false for use cond, true for caseitem;
	bool cond;
	int condid;
	int itemindex;
	unsigned nodeid;
	Cfg_Node* node;
}TPNode;

typedef struct TestPath
{
	Module* md;
	Cfg* cfg;
	svector<TPNode*> nodes;

}TestPath;

typedef struct SmtVar{
	string basename;
	string smtname;
	string parentname;
	unsigned width;
	int msb, lsb;
	string type;
	bool temp_flag;
}SmtVar;

typedef struct RefVar{
	string name;
	unsigned space, time;
	unsigned width;
	int lsb, msb;
	string ptype;
	bool record;
	void dump(ostream& o);
}RefVar;

extern void bv_to_int(ostringstream& expr, ostringstream& target);
extern void int_to_bv(ostringstream& expr, unsigned width, ostringstream& target);
extern void bv_int_bv(ostringstream& expr, unsigned width, ostringstream& target);
extern void bool_to_bv(ostringstream& expr, ostringstream& target);
extern void extract(ostringstream& expr, int msi, int lsi, ostringstream& target);
extern void bv_compare_zero(ostringstream& expr, string op, unsigned width, ostringstream& target);
extern void int_compare_zero(ostringstream& expr, string op, ostringstream& target);

extern bool operator == (const SmtVar&left, const SmtVar&right);
extern bool operator <= (const SmtVar&left, const SmtVar&right);
extern bool operator <  (const SmtVar&left, const SmtVar&right);

inline bool operator > (const SmtVar&left, const SmtVar&right)
{ return right < left; }

inline bool operator >= (const SmtVar&left, const SmtVar&right)
{ return right <= left; }

inline bool operator != (const SmtVar&left, const SmtVar&right)
{ return (left == right)? false : true; }

/*
* TestGen is using for generate the SMT-LIB2 format file in netlist.
*/
class TestGen{
	public:
		TestGen(PDesign*, Design*, Module*);
		~TestGen();
		void gen_smt(string file, ostream& o1, ostream& o2, ostream& o3);  /* Generate SMT-LIB2 for a module. */
	private:
		Design*      design_;
		PDesign*     pdesign_;
		Module*      module_;
		map<Module*, SVTable>                     smttabs_;              	/* Variables builded in smt generator. */
		map<Module*, VarTable>                    vartabs_;              	/* Variables defined in every module. */
		map<Module*, map<unsigned, NetStats> >         stats_;              /* Netlist statements located in evert module. */
		map<unsigned, map<Module*, list<TestPath*> > > testpath_;           /* Record time and paths for every module.*/

	private:
		void initilize(string file);                                      	/* Collect statements and variables for a module */
		void get_stats();                                                 	/* Get netlist statements. */
		void get_vartab();                                                	/* Get var table for a module. */
		void read_testpath(string file);                                  	/* Collect testpath information from a file. */
		void arrange_testpath();                                          	/* Build node for testpath. */
 
		void gen_body(ostream& o);                                       	/* Generate assert lines. */
		void gen_head(ostream& o);                                       	/* Generate define lines. */
		void gen_tail(ostream& o);                                       	/* Generate get-value lines. */
		
		void add_refs(TestPath* tp, set<string>& refs);                     /* Add refs after excute a process. */
		void update_refs(set<string>& refs, unsigned cur_time);             /* Update the space after excuting a synchorous process. */
		void update_changed(set<string>& refs, unordered_map<string, bool>& changed);
		bool excute_process(unordered_map<string, bool>& changed, TestPath* tp);

		void gen_proc_smt(TestPath* tp, ostream& o, set<string>& refs, unsigned cur_time);  	/* Generate smt for a process. */

};

#endif