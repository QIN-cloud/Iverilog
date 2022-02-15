#ifndef __VCDVAR_H
#define __VCDVAR_H

#include <map>
#include <set>
#include <vector>
#include "Module.h"
#include "verinum.h"
#include "CfgNode.h"
#include "PExpr.h"

class VcdScope;
class CoverNum;

/* 
* Collect statistics for coverage functions.
* For example, a data set{1,2,3,4} and a statistic collector{1,1,2}.
* Total states  {1,2,3,4} -> total  = 4;
* Active states {1,1,2}   -> active = 3;
* Hit states    {1,2}     -> hit    = 2;
* Miss states   {3,4}     -> miss   = 2;
*/
class CoverNum{
public:
	CoverNum():total_(0), active_(0), hit_(0), miss_(0){};
	~CoverNum();
	void add(const CoverNum* num);
	void dump(ostream& o, string s);                    
	unsigned total_;					
	unsigned active_;
	unsigned hit_;
	unsigned miss_;
};

/* 
* Variables defined in an instantion module scope of VCD file.
* We use this struct to record value-changes and repaly a simulation.
* A variable may have mutiple VcdVars because of the mutiple instantiations.
*/
class VcdVar{
public:
	VcdVar(){};
	void dump(ostream& o);
	VcdScope* scope; 				//The instantiated module where this variable is located in.
	string name;      				//Variable name.
	bool little_endia; 				//True as var[3:0], false as var[0:3].
	unsigned width;   		    	//Bit width.
	long lsb, msb;     				//Least and most size of bits.
	verinum cur_val;  				//Value at current time.
	verinum pre_val;   				//Value at last time.
};

/*
* This structure represents a toggle analysis report of a variable.
* Note that the results of mutiple instantiations of the same variable
* will eventually be merged.
*/
class VarToggle{
public:
	VarToggle(const VcdVar* var);
	~VarToggle();
	void add(const VcdVar* var);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	string name_;
	unsigned width_;
	long lsb_;
	long msb_;
	bool little_endia_;
	CoverNum* num_;
	map<long, unsigned> toggle01_; 						//Results of bit index which has toggle "0" to "1".
	map<long, unsigned> toggle10_; 						//Results of bit index which has toggle "1" to "0".
};

/*
* Represent the toggle report for a module.
*/
class ToggleRep{
public:
	ToggleRep();
	~ToggleRep();
	void add(const VcdVar* var);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	CoverNum* num_;
	map<string, VarToggle*> vars_;
};

/*
* Represent the FSM analysis report for a selected variable.
* Including the variable reached states and transitions.
*/
class FsmRep{
public:
	//Record the states of a variable.
	typedef struct StateRep{
	public:
		StateRep():num_(0){};
		unsigned num_;
		vector<pair<unsigned, verinum> > states_;
	}CoverState;
	//Record the transtations of a variable.
	typedef struct TranRep{
		TranRep() : num_(0){};
		unsigned num_;
		vector<pair<unsigned, pair<verinum, verinum> > > trans_;
	}CoverTran;
public:
	FsmRep();
	~FsmRep();
	void add(const VcdVar* var);
	void dump(ostream& o);
private:
	map<string, CoverState*>  var_states_;   //The format is {var name , {states num , {state , number}}}
	map<string, CoverTran*> var_trans_;      //The format is {var name , {total num , {tran , number}}}
};

/*
* Represent the statement analysis report for a module.
* This report is based on the cfg, the output is the 
* number of executions of each row of statements.
*/
class StatRep{
public:
	StatRep(Module_Cfgs* cfgs);
	~StatRep();
	void add(const set<unsigned>& path);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	CoverNum* num_;
	map<unsigned, unsigned> stmts_; 				//Count the covered times of each line of statement.
	map<unsigned, Cfg_Node*> nodes_;			    //Using for output the type of lines.
};

class ProcessRep{
public:
	ProcessRep(Module* mod, Cfg* cfg);
	~ProcessRep();
	void add(unsigned time, const set<unsigned>& path);
	void dump_time_paths(ostream& o);
	void dump_cover_paths(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	bool equal(const set<unsigned>& p1, const set<unsigned>& p2);
	Module* module_;
	Cfg* cfg_;
	CoverNum* num_;
	map<unsigned, vector<unsigned> > time_paths_;  //The format is {clk time, {paths address(get from Module)}}.
	map<unsigned, unsigned> paths_;                //The format is {path, cover number}.
};

/*
* Represent the path analysis report for a module.
*/
class PathRep{
public:
	PathRep(Module* mod);
	~PathRep();
	void add(unsigned time, Cfg* cfg, const set<unsigned>& path);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	CoverNum* num_;
	Module* module_;
	map<Cfg*, ProcessRep*> procs_;
};

/*
* Represent the branch analysis report for a control node.
*/
class ExprBranch{
public:
	ExprBranch(Cfg* cfg, Cfg_Node* node, map<unsigned, ExprBranch*>& branchs);
	~ExprBranch();
	void add(unsigned lineno);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	CoverNum* num_;
	Cfg_Node* node_;									//Control node.
	map<unsigned, unsigned> branch_;					//The format is {lineno of branch, hit times}.
};

/*
* Represent the branch analysis report for a module.
*/
class BranchRep{
public:
	BranchRep(Module_Cfgs* cfgs);
	~BranchRep();
	void add(const set<unsigned>& path);
	void dump(ostream& o);
	inline CoverNum* get_num(){return num_;}
private:
	CoverNum* num_;
	map<unsigned, ExprBranch*> locations_;		      //The format is {lineno , branch location}.
	vector<ExprBranch*> branchs_;
};

/*
* Record the value of different items in a condit expression.
* For example , a expression (a[7:0]+b[7:0])/2.
* a[7:0] and b[7:0] are two items and their value is recorded.
*/
struct CondValue{
public:
	CondValue(const vector<pair<PExpr*, verinum> >& items);
	~CondValue();
	void dump(ostream& o);
	vector<pair<PExpr*, verinum> > items_;
	unsigned num_;
};
extern bool operator == (const vector<pair<PExpr*, verinum> >& v1, const vector<pair<PExpr*, verinum> >& v2);

/*
* Represent the combine analysis report for a cond expression of control cfgnode.
*/
class ExprCombine{	
public:
	ExprCombine(Cfg_Node* node);
	~ExprCombine();
	void add(verinum value, const map<PExpr*, verinum>& items);
	void dump(ostream& o);
private:
	Cfg_Node* node_;
	unsigned num_;
	map<verinum, pair<unsigned, vector<CondValue*> > > cond_items_; 	//The format is {{value}, vector{times, items}}.
};

/*
* Represent the combine analysis for a module.
*/
class CombineRep{
public:
	CombineRep(Module_Cfgs* cfgs);
	~CombineRep();
	void add(Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items);
	void dump(ostream& o);
private:
	map<Cfg_Node*, ExprCombine*> exprs_;					   		   //All of combines in this module.
};

class BasicRep{
public:
	BasicRep();
	~BasicRep();
	CoverNum* toggle_num_;
	CoverNum* stat_num_;
	CoverNum* path_num_;
	CoverNum* branch_num_;
};

/*
* Store selected coverage analysis for one module.
*/
class ModuleRep : public BasicRep{
public:
	ModuleRep(Module* mod);
	~ModuleRep();

	void add_toggle(const VcdVar* var);
	void add_fsm(const VcdVar* var);
	void add_stat(const set<unsigned, less<unsigned> >& paths);
	void add_path(const set<unsigned, less<unsigned> >& paths, unsigned time, Cfg* cfg);
	void add_branch(const set<unsigned, less<unsigned> >& paths);
	void add_combine(Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items);

	void dump_toggle(ostream& o);
	void dump_fsm(ostream& o);
	void dump_stat(ostream& o);
	void dump_path(ostream& o);
	void dump_branch(ostream& o);
	void dump_combine(ostream& o);

private:
	Module* module_;
	ToggleRep* toggle_;
	FsmRep* fsm_;
	StatRep* statement_;
	BranchRep* branch_;
	PathRep* path_;
	CombineRep* combine_;
};

/*
* This class collects the coverage analysis of all modules.
*/
class FinalRep : public BasicRep{
public:
	FinalRep(bool t, bool f, bool s, bool p, bool b, bool c);
	~FinalRep();

public:
	void add_toggle(Module* mod, const VcdVar* var);
	void add_fsm(Module* mod_, const VcdVar* var);
	void add_stat(Module* mod_, const set<unsigned, less<unsigned> >& paths);
	void add_path(Module* mod, const set<unsigned, less<unsigned> >& paths, unsigned time, Cfg* cfg);
	void add_branch(Module* mod, const set<unsigned, less<unsigned> >& paths);
	void add_combine(Module* mod, Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items);
	void dump(ostream& o);

private:
	void check(Module* mod);
	void dump_toggle(ostream& o);
	void dump_fsm(ostream& o);
	void dump_stat(ostream& o);
	void dump_path(ostream& o);
	void dump_branch(ostream& o);
	void dump_combine(ostream& o);
	void dump_summary(ostream& o);

private:
	bool toggle_flag_;
	bool fsm_flag_;
	bool stat_flag_;
	bool path_flag_;
	bool branch_flag_;
	bool combine_flag_;

private:
	map<Module*, ModuleRep*> reports_;
};
#endif
