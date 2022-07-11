#ifndef __VCDVAR_H
#define __VCDVAR_H
#define  BRANCH_TRUE_VALUE 1
#define  BRANCH_FALSE_VALUE 0
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "Module.h"
#include "verinum.h"
#include "CfgNode.h"
#include "PExpr.h"
#include "PGate.h"
#include "StringHeap.h"

class VcdScope;
class VcdVar;

typedef map<perm_string, vector<string> > Filelines;

/* 
* Variables defined in an instantion module scope of VCD file.
* We use this struct to record value-changes and repaly a simulation.
* A variable may have mutiple VcdVars because of the mutiple instantiations.
*/
class VcdVar{
public:
	typedef enum Type{INPUT, OUTPUT, INOUT, SIGNAL};
	VcdVar();
	~VcdVar();
	void dump(ostream& o);
	VcdScope* scope; 				//The instantiated module where this variable is located in.
	string name;      				//Variable name.
	Type type;						//Signal or Port.
	string symbol;					//Symbol in VCD file.
	bool little_endia; 				//True as var[3:0], false as var[0:3].
	unsigned width;   		    	//Bit width.
	long lsb, msb;     				//Least and most size of bits.
	verinum cur_val;  				//Final Value at current time.
	verinum pre_val;   				//Final Value at last change time.
	verinum sim_val;                //Real value in simulation replay.
};

class ToggleRep{
public:
	ToggleRep(VcdVar* var);
	~ToggleRep();
	void add();
	unsigned get_pos_num();
	unsigned get_neg_num();
	void dump_bit(ostream& o);
	void dump_detail(ostream& o);
private:
	VcdVar* var_;
	vector<bool> pos_bits_;
	vector<bool> neg_bits_;
};

class FsmRep{
public:
	FsmRep(VcdVar* var);
	~FsmRep();
	void add();
	unsigned get_state();
	unsigned get_transition();
	void dump_state(ostream& o);
	void dump_transition(ostream& o);
private:
	VcdVar* var_;
	map<verinum, bool> states_;
	map<pair<verinum, verinum> , bool> trans_;
};

class LineRep{
public:
	LineRep(Cfg* cfg, VcdScope* scope);
	~LineRep();
	void initial();
	void add(set<unsigned>& lines);
	unsigned get_total();
	unsigned get_hit();
	void dump(ostream& o);
	void dump_summary(ostream& o);
private:
	Cfg* cfg_;
	Module* module_;
	Filelines* file_;
	unordered_map<unsigned, bool> stats_;
};

class PathRep{
public:
	PathRep(Cfg* cfg, VcdScope* scope);
	~PathRep();
	void add(set<unsigned>& lines);
	unsigned get_total();
	unsigned get_hit();
	void dump(ostream& o);
	void dump_summary(ostream& o);
private:
	Cfg* cfg_;
	VcdScope* scope_;
	Filelines* file_;
	vector<bool> paths_;
};

class BranchTree{
public:
	typedef enum TreeType{TERNARY, IF, CASE, CASEX, CASEZ, BLOCK, EXPRLEAF, STATLEAF};
	BranchTree(Module* md, TreeType type);
	virtual ~BranchTree();
	virtual void dump(ostream& o, VcdScope* scope) = 0;
	virtual void dump_summary(ostream& o, VcdScope* scope) = 0;
	virtual void add(vector<unsigned>& values, VcdScope* scope, unsigned& idx) = 0;
	virtual unsigned get_total() = 0;
	virtual unsigned get_hit(VcdScope* scope) = 0;
public:
	Module* module_;
	TreeType type_;
	map<VcdScope*, bool> cover_;
};

class BrCondit : public BranchTree{
public:
	BrCondit(Module* md, TreeType type);
	virtual ~BrCondit();
	virtual void dump(ostream& o, VcdScope* scope);
	virtual void dump_summary(ostream& o, VcdScope* scope);
	virtual void add(vector<unsigned>& values, VcdScope* scope, unsigned& idx);
	virtual unsigned get_total();
	virtual unsigned get_hit(VcdScope* scope);
public:
	unsigned lineno_;
	PExpr* cond_;
	BranchTree* tru_;
	BranchTree* fal_;
};

class BrCase : public BranchTree{
public:
	BrCase(Module* md, TreeType type);
	virtual ~BrCase();
	virtual void dump(ostream& o, VcdScope* scope);
	virtual void dump_summary(ostream& o, VcdScope* scope);
	virtual void add(vector<unsigned>& values, VcdScope* scope, unsigned& idx);
	virtual unsigned get_total();
	virtual unsigned get_hit(VcdScope* scope);
public:
	unsigned lineno_;
	PExpr* cond_;
	vector<pair<PExpr*, BranchTree*> > items_;
};

class BrBlock : public BranchTree{
public:
	BrBlock(Module* md, TreeType type);
	virtual ~BrBlock();
	virtual void dump(ostream& o, VcdScope* scope);
	virtual void dump_summary(ostream& o, VcdScope* scope);
	virtual void add(vector<unsigned>& values, VcdScope* scope, unsigned& idx);
	virtual unsigned get_total();
	virtual unsigned get_hit(VcdScope* scope);
public:
	list<BranchTree*> stats_;
};

class BrLeaf : public BranchTree{
public:
	BrLeaf(Module* md, TreeType type);
	virtual ~BrLeaf();
	virtual void dump(ostream& o, VcdScope* scope);
	virtual void dump_summary(ostream& o, VcdScope* scope);
	virtual void add(vector<unsigned>& values, VcdScope* scope, unsigned& idx);
	virtual unsigned get_total();
	virtual unsigned get_hit(VcdScope* scope);
public:
	union{
		PExpr* expr_;
		unsigned lineno;
	}node_;
};

class BranchRep{
public:
	BranchRep(BranchTree* tree, VcdScope* scope);
	~BranchRep();
	void add(vector<unsigned>& values);
	unsigned get_hit();
	unsigned get_total();
	void dump(ostream& o);
	void dump_summary(ostream& o);
private:
	BranchTree* tree_;
	VcdScope* scope_;
};

class CondRep{
public:
	CondRep(PExpr* cond, set<PExpr*> items);
	~CondRep();
	enum CondType {SINGEL, OR, AND};
	void add(map<PExpr*, bool>& values);
	void dump_summary(ostream& o);
	void dump(ostream& o);
	unsigned get_total();
	unsigned get_hit();
private:
	PExpr* cond_;
	vector<PExpr*> items_;
	map<unsigned, bool> combs_;
	CondType type_;
};

/*
* ScopeRep stores the coverage report for a instantiated module.
*/

class ScopeRep{
public:
	ScopeRep(bool t, bool f, bool l, bool p, bool b, bool c, VcdScope* scope);
	~ScopeRep();
public:
	void dump(ostream& o);
	void initial();
	void add_tgl_report(set<VcdVar*> vars);
	void add_fsm_report(VcdVar* var);
	void add_line_report(Cfg* cfg, set<unsigned>& lines);
	void add_path_report(Cfg* cfg, set<unsigned>& lines);
	void add_branch_report(unsigned lineno, vector<unsigned>& values);
	void add_cond_report(map<PExpr*, map<PExpr*, bool> >& values);
private:
	void toggle_initial();
	void fsm_initial();
	void line_initial();
	void path_initial();
	void branch_initial();
	void cond_initial();
	void dump_tgl_summary(ostream& o, string type);
	void dump_tgl_detail(ostream& o, string type);
	void dump_tgl_bit(ostream& o, string type);
	void dump_tgl_report(ostream& o);
	void dump_fsm_report(ostream& o);
	void dump_line_summary(ostream& o);
	void dump_line_detail(ostream & o);
	void dump_line_report(ostream & o);
	void dump_path_summary(ostream& o);
	void dump_path_detail(ostream& o);
	void dump_path_report(ostream& o);
	void dump_branch_summary(ostream& o);
	void dump_branch_detail(ostream& o);
	void dump_branch_report(ostream& o);
	void dump_cond_summary(ostream &o);
	void dump_cond_detail(ostream& o);
	void dump_cond_report(ostream& o);
private:
	bool cover_t;
	bool cover_f;
	bool cover_l;
	bool cover_p;
	bool cover_b;
	bool cover_c;
private:
	VcdScope* scope_;
	map<VcdVar*, ToggleRep*> toggle_;
	map<VcdVar*, FsmRep*> fsm_;
	map<unsigned, LineRep*> line_;
	map<unsigned, PathRep*> path_;
	map<unsigned, BranchRep*> branch_;
	map<PExpr*, CondRep*> cond_;
};

extern bool equal(const set<unsigned>& l1, const set<unsigned>& l2);
extern void dump_coverage_line(ostream& o, VcdScope* scope, string type);
extern void dump_depart_line(ostream& o);
extern void dump_line(ostream& o, string s);
extern void dump_line(ostream& o, ostringstream& s);
extern void dump_summary_line(ostream& o, string s1, string s2, string s3, string s4);
extern void dump_summary_line(ostream& o, string head, unsigned total, unsigned covered);
extern void dump_summary_line(ostream& o, string s1, string s2, string s3, string s4, string s5);
extern void dump_summary_line(ostream& o, string head, unsigned lineno, unsigned total, unsigned covered);
extern void dump_toggle_detail(ostream& o, string s1, string s2, string s3, string s4, string s5);
extern void dump_toggle_detail(ostream& o, ostringstream& name, bool posedge, bool negedge, VcdVar::Type type);
extern void dump_item_line(ostream& o, string item);
extern void dump_toggle_bit(ostream& o, string s1, string s2);
extern void dump_toggle_bit(ostream& o, ostringstream& name, ostringstream& bits);
extern void dump_fsm_state(ostream& o, const verinum& v);
extern void dump_fsm_transtate(ostream& o, const verinum& pre_val, const verinum& cur_val);
extern void dump_line_detail(ostream& o, unsigned lineno, bool select, bool hit, string& s);
extern void dump_line_detail(ostream& o, bool select, bool hit, PExpr* expr);
extern void dump_path_line(ostream& o, unsigned lineno);
extern void dump_cond_detail(ostream& o, CondRep::CondType type, unsigned num, unsigned idx, bool covered);
extern void dump_cond_line(ostream& o, unsigned num);
extern void dump_branch_detail(ostream& o, unsigned lineno, string head, bool covered, ostringstream& expr);
extern void dump_branch_line(ostream& o, unsigned lineno);

#endif
