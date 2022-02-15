#ifndef __VCDVAR_H
#define __VCDVAR_H

#include <map>
#include <set>
#include <vector>
#include "verinum.h"

class vcd_var{
public:
	vcd_var();
	string name_;
	bool little_endia;
	unsigned width;
	long lsb, msb;
	verinum pre_val_;
	verinum cur_val_;
public:
	void toggle01();
	void toggle10();
	void initial_toggle_stat();
	void toggle_report(ostream& report);
private:
	map<int, bool> toggle01_stat;
	map<int, bool> toggle10_stat;
};

typedef struct covered_tran{
	verinum from;
	verinum to;
}covered_tran;

class fsm_stat{
public:
	fsm_stat();
	void add_covered_state(verinum state);
	void add_covered_tran(covered_tran ct);
	void report_fsm(ostream& report);
private:
	vector<verinum> covered_state;
	vector<covered_tran> covered_trans;
};

class stmt_stat{
public:
	stmt_stat();
	void add_stmt(set<unsigned, less<unsigned> > stmt);
	void report_stmt(ostream& report);
private:
	set<unsigned, less<unsigned> > covered_stmts;
};

class path_stat{
public:
	path_stat();
	void add_path(set<unsigned, less<unsigned> > path);
	void report_path(ostream& report);
private:
	vector<set<unsigned, less<unsigned> > > covered_paths;
	bool equal(set<unsigned, less<unsigned> > p1, set<unsigned, less<unsigned> > p2);
};

#endif