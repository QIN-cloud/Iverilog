#include "testpath.h"
#include "PDesign.h"
#include "Module.h"
#include "PExpr.h"
#include "Statement.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <string.h>

void error(const char* p, const char* p2 = " ")
{
	cerr << p << ' ' << p2 << endl;
	exit(1);
}

bool operator == (const RefVar&left, const RefVar&right)
{
	if((left.name == right.name) && (left.time == right.time) && (left.space == right.space) )
		return true;
	else
		return false;
}

TestGen::TestGen(PDesign* pde, Design* de, Module* md)
{
	pdesign_ = pde;
	design_ = de;
	module_ = md;
}

TestGen::~TestGen()
{
}

void TestGen::gen_smt(string file, ostream& o1, ostream& o2, ostream& o3)
{
	module_->set_design(design_);

	cout << "Initializing..." << endl;

	initilize(file);

	cout << "Genenrate smt assign lines..." << endl;

	gen_body(o2);

	cout << "Generate smt define lines..." << endl;

	gen_head(o1);

	cout << "Generate smt get-value lines..." << endl;

	gen_tail(o3);

	cout << "Generator had excuted succefully..." << endl;

}

void TestGen::initilize(string file)
{
	cout << "Search the statements in netlist..." << endl;

	get_stats();

	cout << "Build the var table for module " << module_->pscope_name() << "..." << endl;

	get_vartab();

	cout << "Parsing the nodes file..." << endl;

	read_testpath(file);

	cout << "Arrange the paths for conditional statements..." << endl;

	arrange_testpath();

}

void TestGen::get_stats()
{
	NetProcTop* proc = design_->find_process();
	while(proc)
	{
		cout << proc->get_id() << endl;
		NetStats proc_stats;
		proc->gen_stats(&proc_stats);
		stats_[pdesign_->get_modules()[proc->scope()->basename()]][proc->get_id()] = proc_stats;
		proc = design_->find_next_process(proc);
	}
	/*
	for(auto i : stats_)
	{
		cout << i.first->pscope_name() << endl;
		for(auto j : i.second)
		{
			cout << "ID : " << j.first << endl;
			for(auto k : j.second)
			{
				cout << k.first << " " << k.second.size() << endl;
			}
		}
	}
	*/
}

void TestGen::get_vartab()
{
	module_->initialize(false, false, false, false, true);

	vartabs_[module_] = module_->vartab_;
}

void TestGen::read_testpath(string file)
{
	module_->tran_nodes();
	
	ifstream in(file.c_str());
	if(!in) 
		error("cannot open input file", file.c_str());

	string head;
	unsigned cycle, procid, pathid;

	while(!in.eof())
	{
		in >> head;

		if(head.empty())  
			break;

		if(head == "cycle:")
		{	
			in >> cycle;
			procid = 0;
		}

		else
		{
			Module_Cfgs* cfgs = module_->get_cfg();
			string text;
			while(in >> text && text != "end")
			{
				TestPath* tp = new TestPath;
				tp->md = module_;
				tp->cfg = (*(cfgs->cfgs))[procid];
				pathid = atoi(text.c_str());
				for(unsigned nodeid : module_->nodes_[tp->cfg->id][pathid])
				{
					TPNode* node = new TPNode;
					node->nodeid = nodeid;
					tp->nodes = svector<TPNode*>(tp->nodes, node);
				}
				testpath_[cycle][tp->md].push_back(tp);
				procid++;
			}
		}
	}
	in.close();
}

void TestGen::arrange_testpath()
{
	for(map<unsigned, map<Module*, list<TestPath*> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		map<Module*, list<TestPath*> >::iterator mpos;
		for(mpos = pos->second.begin(); mpos != pos->second.end(); ++mpos)
		{
			//iterate over the cfg to assign refs and defs and control direction to TestPath*
			for(list<TestPath*>::iterator lpos = mpos->second.begin(); lpos != mpos->second.end(); ++lpos)
			{
				for(unsigned idx = 0; idx < (*lpos)->nodes.count(); ++idx)
				{
					Cfg_Node* node = (*(((*lpos)->cfg)->root))[(*lpos)->nodes[idx]->nodeid];
					(*lpos)->nodes[idx]->node = node;
					(*lpos)->nodes[idx]->condid = -1;
					if(node->type.find("ISCONTROL") == 0)
					{
						(*lpos)->nodes[idx]->condid = node->condid;
						for(unsigned idx2 = 0; idx2 < node->dsuc.count(); ++idx2)
						{
							if((idx + 1) < (*lpos)->nodes.count())
							{
								if(node->dsuc[idx2]->index == (*lpos)->nodes[idx + 1]->nodeid)
								{
									if(node->dsuc[idx2]->caseitem.size() < 1)
									{
										(*lpos)->nodes[idx]->usewhich = false;
										(*lpos)->nodes[idx]->cond = node->dsuc[idx2]->cond;
										(*lpos)->nodes[idx]->itemindex = -1;
									}
									else
									{
										(*lpos)->nodes[idx]->usewhich = true;
										(*lpos)->nodes[idx]->itemindex = node->dsuc[idx2]->caseitemidx;
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void TestGen::gen_body(ostream& o)
{
	set<string> refs;
	unordered_map<string, bool> changed;

	//For each time cycle.
	for(map<unsigned, map<Module*, list<TestPath*> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		o << endl;

		refs.clear();

		//Initialize the space and time number for variables, especially for input type. 
	    map<string, RefVar*>::iterator vpos = vartabs_[module_].begin();
		for(vpos; vpos != vartabs_[module_].end(); vpos++)
		{
			if(vpos->second->ptype == "PINPUT" || vpos->second->ptype == "PINOUT")
			{	
				vpos->second->time = pos->first;
				vpos->second->record = false;
			}
		}

		//First step, excute all the assign statements.
		module_->gen_assign_smt(refs, true, o, vartabs_[module_], smttabs_[module_], pos->first);

		//Second step, excute the sychorous processes then update the space variables.
		list<Cfg*>::iterator sp = module_->sync_cfgs_.begin();
		for(sp; sp != module_->sync_cfgs_.end(); sp++)
		{
			list<TestPath*>::iterator tpp = testpath_[pos->first][module_].begin();
			for(tpp; tpp != testpath_[pos->first][module_].end(); tpp++)
			{
				if((*tpp)->cfg == (*sp))
				{
					cout << ">>>sync process " << (*sp)->id << endl;
					gen_proc_smt(*tpp, o, refs, pos->first);
				}
			}
		}
		update_refs(refs, pos->first);

		//Third step, excute the influenced assign statements.
		module_->gen_assign_smt(refs, false, o, vartabs_[module_], smttabs_[module_], pos->first);
		update_changed(refs, changed);

		//Fourth step, excute the combine process according to the changed vars.
		list<Cfg*>::iterator cp = module_->combine_cfgs_.begin();
		for(cp; cp != module_->combine_cfgs_.end(); cp++)
		{
			list<TestPath*>::iterator tpp = testpath_[pos->first][module_].begin();
			for(tpp; tpp != testpath_[pos->first][module_].end(); tpp++)
			{
				if((*tpp)->cfg == (*cp))
				{
					cout << ">>>combine process " << (*cp)->id << endl;
					gen_proc_smt(*tpp, o, refs, pos->first);

					list<Cfg*>::iterator temp = cp;
					if(++temp != module_->combine_cfgs_.end())
					{
						module_->gen_assign_smt(refs, false, o, vartabs_[module_], smttabs_[module_], pos->first);
						update_changed(refs, changed);
					}
					break;
				}
			}
		}
		changed.clear();
	}
}

void TestGen::update_refs(set<string>& refs, unsigned cur_time)
{
	for(string ref : refs)
	{
		RefVar* var = vartabs_[module_][ref];

		var->space = var->time == cur_time ? var->space + 1 : 1;
		var->time = cur_time;
	}
}

void TestGen::update_changed(set<string>& refs, unordered_map<string, bool>& changed)
{
	for(string ref : refs)
	{
		changed[ref] = true;
	}
	refs.clear();
}

bool TestGen::excute_process(unordered_map<string, bool>& changed, TestPath* tp)
{
	for(string ref : tp->cfg->refs)
	{
		if(changed.find(ref) != changed.end())
		{
			return true;
		}
	}

	return false;
}

void TestGen::gen_proc_smt(TestPath* tp, ostream& o, set<string>& refs, unsigned cur_time)
{
	//for all the nodes in current test path
	for(unsigned idx = 0; idx < tp->nodes.count(); ++idx)
	{
		//find the condition node
		if(tp->nodes[idx]->node->type.find("ISCONTROL") == 0)
		{
			for(NetProc* stat : stats_[module_][tp->cfg->id][tp->nodes[idx]->node->lineno])
			{
				stat->dump_smt(o, vartabs_[module_], smttabs_[module_], tp->nodes[idx]->cond, tp->nodes[idx]->itemindex, module_, refs);
			}
		}
		else // for non-control node
		{
			for(NetProc* smt_stat : stats_[module_][tp->cfg->id][tp->nodes[idx]->node->lineno])
			{
				smt_stat->dump_smt(o, vartabs_[module_], smttabs_[module_], module_, refs, cur_time);
			}
		}
	}

	o << endl;

}

void TestGen::gen_head(ostream& o)
{
	set<string> defines;
	for(SmtVar* var : smttabs_[module_])
	{
		if(defines.find(var->smtname) == defines.end())
		{
			if(var->temp_flag)
				o << "(declare-const " << var->smtname << " Int)" << endl;
			else
				o << "(declare-const " << var->smtname << " (_ BitVec " << var->width << "))" << endl;
			defines.insert(var->smtname);
		}
	}
}

void TestGen::gen_tail(ostream& o)
{
	o << "(check-sat)" << endl;
	for(SmtVar* var : smttabs_[module_])
	{
		if(var->type == "PINPUT" || var->type == "PINOUT")
			o << "(get-value(" << var->smtname << "))" << endl;
	}
}

bool operator == (const SmtVar&left, const SmtVar&right)
{
	if(left.smtname == right.smtname)
		return true;
	else
		return false;
}

bool operator <= (const SmtVar&left, const SmtVar&right)
{
	if(left.smtname <= right.smtname)
		return true;
	else
		return false;
}

bool operator <  (const SmtVar&left, const SmtVar&right)
{
	if(left.smtname < right.smtname)
		return true;
	else
		return false;
}

/* 
* Convert a BitVec to a Int.
* The format is ( (_ bv2int) BitVec ).
*/
void bv_to_int(ostringstream& expr, ostringstream& target)
{
	target << "((_ bv2int)" << expr.str() << ")";
}

/* 
* Convert a Int to a BitVec.
* Request a width of BitVec.
* The format is ( (_ int2bv width) Int ).
*/
void int_to_bv(ostringstream& expr, unsigned width, ostringstream& target)
{
	target << "((_ int2bv " << width << ")" << expr.str() << ")";
}

/*
* Convert a BitVec to a BitVec for changing the width of the BitVec.
* The format is ( (_ int2bv width ) ( (_ bv2int ) BitVec ) ).
*/
void bv_int_bv(ostringstream& expr, unsigned width, ostringstream& target)
{
	target << "((_ int2bv " << width << ")" << "((_ bv2int)" << expr.str() << "))";
}

/*
* Convert a Bool expression to a BitVec.
* First convert Bool to Int type, then Int to BitVec.
*/
void bool_to_bv(ostringstream& expr, ostringstream& target)
{
	ostringstream i_expr;
	i_expr << "(ite " << expr.str() << " 1 0 )";
	int_to_bv(i_expr, 1, target);  
}

/*
* Bit or part selection for a BitVec.
* The format is ( (_ extract msi lsi ) BitVec ).
*/
void extract(ostringstream& expr, int msi, int lsi, ostringstream& target)
{
	target << "((_ extract " << msi << " " << lsi << ")" << expr.str() << ")" ;
}

/*
* Compare a BitVec with #b0 which need to expand the number of "0" bits.
*/
void bv_compare_zero(ostringstream& expr, string op, unsigned width, ostringstream& target)
{
	target << "(" << op << " " << expr.str() << " " << "#b";
	for(int i = 0; i < width; i++){target << "0";}
	target << ")";
}

/*
* Compare a Int number with 0 for a bool result.
*/
void int_compare_zero(ostringstream& expr, string op, ostringstream& target)
{
	target << "(" << op << " " << expr.str() << " " << "0";
}

void RefVar::dump(ostream& o)
{
	o << ptype << " " << name << "[" << lsb << ":" << msb << "]" << endl;
}
