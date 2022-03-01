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
class Module;
struct RefVar;
struct TestPath;
struct SmtVar;
struct Cfg;
struct Cfg_Node;

typedef map<int, set<NetProc*> > NetStats;
typedef map<string, RefVar*> VarTable;
typedef set<SmtVar*> SVTable;


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
	int index;
	int width;
	int msb, lsb;
	string type;
	bool temp_flag;
}SmtVar;

typedef struct RefVar{
	string name;
	int space, time;
	unsigned width;
	int lsb, msb;
	string ptype;
}RefVar;

extern void bv_to_int(ostringstream& expr, ostringstream& target);
extern void int_to_bv(ostringstream& expr, int width, ostringstream& target);
extern void bv_int_bv(ostringstream& expr, int width, ostringstream& target);
extern void extract(ostringstream& expr, int msi, int lsi, ostringstream& target);
extern void bv_compare_zero(ostringstream& expr, string op, int width, ostringstream& target);

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
		TestGen(PDesign*, Design*);
		~TestGen();
		void gen_smt(Module* module_, string file, ofstream& o1, ofstream& o2, ofstream& o3);  /* Generate SMT-LIB2 for a module. */
	private:
		Design*      design_;
		PDesign*     pdesign_;
		Module*      module_;
		map<Module*, SVTable>                     smttabs_;              	/* Variables builded in smt generator. */
		map<Module*, VarTable>                    vartabs_;              	/* Variables defined in every module. */
		map<Module*, map<int, NetStats> >         stats_;                	/* Netlist statements located in evert module. */
		map<int, map<Module*, list<TestPath*> > > testpath_;             	/* Record time and paths for every module.*/

	private:
		void initilize(string file);                                      	/* Collect statements and variables for a module */
		void get_stats();                                                 	/* Get netlist statements. */
		void get_vartab();                                                	/* Get var table for a module. */
		void get_refs_defs();                                             	/* Get refs and defs for a process. */
		void read_testpath(string file);                                  	/* Collect testpath information from a file. */
		void arrange_testpath();                                          	/* Build node for testpath. */
 
		void gen_body(ofstream& o);                                       	/* Generate assert lines. */
		void gen_head(ofstream& o);                                       	/* Generate define lines. */
		void gen_tail(ofstream& o);                                       	/* Generate get-value lines. */
		
		void select_sync_proc(set<TestPath*> procs, int cycle);				/* Select synchorous processes. */
		void select_combine_proc(int cycle, list<TestPath*>& procs,         /* Select combine processes. */
		set<string>& refs, map<TestPath*, unsigned>& excute_procs);         
		void add_refs(TestPath* tp, set<string>& refs);                     /* Add refs after excute a process. */
		void update_refs(set<string>& refs);                                /* Update the space after excuting a synchorous process. */

		void gen_proc_smt(TestPath* tp, ofstream& o);  	                    /* Generate smt for a process. */

};

#endif