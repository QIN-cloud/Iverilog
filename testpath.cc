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

TestGen::TestGen(PDesign* pde, Design* de)
{
	pdesign_ = pde;
	design_ = de;
}

TestGen::~TestGen()
{
}

void TestGen::gen_smt(Module* module_, string file, ofstream& o1, ofstream& o2, ofstream& o3)
{
	this->module_ = module_;

	initilize(file);
	
	gen_body(o1);
	
	gen_head(o2);
	
	gen_tail(o3);

}

void TestGen::initilize(string file)
{
	get_stats();

	get_vartab();

	read_testpath(file);

	arrange_testpath();

}

void TestGen::get_stats()
{
	NetProcTop* proc = design_->find_process();
	while(proc)
	{
		NetStats proc_stats;
		proc->gen_stats(&proc_stats);
		stats_[pdesign_->get_modules()[proc->scope()->basename()]][proc->get_id()] = proc_stats;
		proc = design_->find_next_process(proc);
	}
}

void TestGen::get_vartab()
{
	module_->build_vartab(design_);
	set<RefVar*> vars = module_->get_vartable();
	for(RefVar* var : vars)
	{
		vartabs_[module_][var->name] = var;
	}

	module_->parse_assigns();
	module_->sort_assigns();
	module_->parse_wires(vartabs_[module_]);
}

void TestGen::read_testpath(string file)
{
	ifstream in(file.c_str());
	string temp_path("temp.txt");
	ofstream temp_out(temp_path);
	if(!in) 
		error("cannot open input file", file.c_str());

	string head;
	unsigned cycle, nodeid, procid;

	while(!in.eof())
	{

		in >> head;

		if(head.empty())  
			break;

		if(head == "cycle:")
			in >> cycle;

		else
		{

			TestPath* tp = new TestPath;
			string name_s = head.substr(0, head.length());
			char* name_c = new char[head.length()];
			strcpy(name_c, name_s.c_str());
			const char* const_name = name_c;

            tp->md = pdesign_->get_modules()[perm_string(name_c)];
			Module_Cfgs* cfgs = tp->md->get_cfg();

			in >> procid;
			Cfg* cfg = (*(cfgs->cfgs))[procid];
			tp->cfg = cfg;

			while(in >> nodeid)
			{
				
				TPNode* node = new TPNode;
				temp_out << (*(cfg->root))[nodeid]->lineno << " ";
				node->nodeid = nodeid;
				tp->nodes = svector<TPNode*>(tp->nodes, node);
				if(in.get() == 10) break;

			}

			temp_out << endl;
			testpath_[cycle][tp->md].push_back(tp);
			
		}
	}
	in.close();
	temp_out.close();
}

void TestGen::arrange_testpath()
{
	for(map<int, map<Module*, list<TestPath*> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
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
						for(int idx2 = 0; idx2 < node->dsuc.count(); ++idx2)
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

void TestGen::select_sync_proc(set<TestPath*> procs, int cycle)
{
	list<TestPath*>::iterator pos = testpath_[cycle][module_].begin();
	for(pos; pos != testpath_[cycle][module_].end(); pos++)
	{
		if((*pos)->cfg->sync)
			procs.insert(*pos);
	}
}

void TestGen::select_combine_proc(int cycle, list<TestPath*>& procs, set<string>& refs, map<TestPath*, unsigned>& excute_procs)
{
	list<TestPath*>::iterator pos = testpath_[cycle][module_].begin();
	for(pos; pos != testpath_[cycle][module_].end(); pos++)
	{
		if(!((*pos)->cfg->sync) && excute_procs.find(*pos) == excute_procs.end())
		{
			for(string ref : refs)
			{
				if((*pos)->cfg->refs.find(ref) != (*pos)->cfg->refs.end())
				{
					procs.push_back(*pos);
					break;
				}
			}
		}
	}
}

void TestGen::gen_body(ofstream& o)
{
	//sign for exit generating smt of process
	unsigned excute_num = 0;
	map<TestPath*, unsigned> excute_procs;
	set<string> refs;

	//for each time cycle
	for(map<int, map<Module*, list<TestPath*> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		refs.clear();

		//initialize the space and time number for variables, especially for input type. 
	    map<string, RefVar*>::iterator vpos = vartabs_[module_].begin();
		for(vpos; vpos != vartabs_[module_].end(); vpos++)
		{
			vpos->second->space =  vpos->second->ptype == "ASSIGN" ? -1 : 0;
			if(vpos->second->ptype == "PINPUT" || vpos->second->ptype == "PINOUT")
				vpos->second->time = pos->first;
		}

		//select all the synchronous process.
		set<TestPath*> clk_procs;
		select_sync_proc(clk_procs, pos->first);

		//first of all excute the assign statements.
		module_->gen_assign_smt(refs, true, o, vartabs_[module_], smttabs_[module_]);

		//secondly excute the synchronous processes at the same time.
		for(TestPath* clk_proc : clk_procs)
		{
			gen_proc_smt(clk_proc, o);
			excute_procs[clk_proc] = ++excute_num;
			add_refs(clk_proc, refs);
		}
		update_refs(refs);

		//excute triggered combination process.
		list<TestPath*> combine_procs;
		while(!combine_procs.empty())
		{
			module_->gen_assign_smt(refs, false, o, vartabs_[module_], smttabs_[module_]);
			select_combine_proc(pos->first, combine_procs, refs, excute_procs);
			refs.clear();
			TestPath* combine_proc = combine_procs.front();
			combine_procs.pop_front();
			gen_proc_smt(combine_proc, o);
			excute_procs[combine_proc] = ++excute_num;
			add_refs(combine_proc, refs);
		}

	}
}

void TestGen::add_refs(TestPath* tp, set<string>& refs)
{
	for(string ref : tp->cfg->refs)
	{
		refs.insert(ref);
	}
}

void TestGen::update_refs(set<string>& refs)
{
	for(string ref : refs)
	{
		vartabs_[module_][ref]->space += 1;
	}
}

void TestGen::gen_proc_smt(TestPath* tp, ofstream& o)
{
	//for all the nodes in current test path
	for(unsigned idx = 0; idx < tp->nodes.count(); ++idx)
	{
		//find the condition node
		if(tp->nodes[idx]->node->type.find("ISCONTROL") == 0)
		{
			for(NetProc* stat : stats_[module_][tp->cfg->id][tp->nodes[idx]->node->lineno])
			{
				stat->dump_smt(o, vartabs_[module_], smttabs_[module_], tp->nodes[idx]->cond, tp->nodes[idx]->itemindex, module_);
			}
		}
		else // for non-control node
		{
			for(NetProc* smt_stat : stats_[module_][tp->cfg->id][tp->nodes[idx]->node->lineno])
			{
				smt_stat->dump_smt(o, vartabs_[module_], smttabs_[module_], module_);
			}
			break;
		}
	}
}

void TestGen::gen_head(ofstream& o)
{
	for(SmtVar* var : smttabs_[module_])
	{
		if(var->temp_flag)
			o << "(declare-const " << var->smtname << " Int)" << endl;
		else
			o << "(declare-const " << var->smtname << " (_ BitVec " << var->width << "))" << endl;
	}
}

void TestGen::gen_tail(ofstream& o)
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
void int_to_bv(ostringstream& expr, int width, ostringstream& target)
{
	target << "((_ int2bv " << width << ")" << expr.str() << ")";
}

/*
* Convert a BitVec to a BitVec for changing the width of the BitVec.
* The format is ( (_ int2bv width ) ( (_ bv2int ) BitVec ) ).
*/
void bv_int_bv(ostringstream& expr, int width, ostringstream& target)
{
	target << "((_ int2bv " << width << ")" << "((_ bv2int)" << expr.str() << "))";
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
void bv_compare_zero(ostringstream& expr, string op, int width, ostringstream& target)
{
	target << "(" << op << " " << expr.str() << " " << "#b";
	for(int i = 0; i < width; i++){target << "0";}
	target << ")";
}