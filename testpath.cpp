#include "testpath.h"
#include "PDesign.h"
#include "Module.h"
#include "PExpr.h"
#include "Statement.h"
#include "DDModel.h"
#include <fstream>
#include <math.h>

void error(const char* p, const char* p2 = " ")
{
	cerr << p << ' ' << p2 << endl;
	exit(1);
}

TestGen::TestGen(PDesign* pde, Design* de)
{
	pdesign_ = pde;
	design_ = de;
}

TestGen::~TestGen()
{
}

void TestGen::initilize()
{
	map<string, Module*> modules = pdesign_->get_modules();
	for(map<string, Module*>::const_iterator pos = modules.begin(); pos != modules.end(); ++pos)
	{
		vartab_[pos->first] = pos->second->get_vartable();
		cetab_[pos->first] = pos->second->get_cetable();
		cfgs_[pos->first] = pos->second->get_cfg();
	}
	clp_vars_.clear();
	used_var_.clear();
}

void TestGen::gen_ref_def()
{
	for(map<int, map<string, list<testpath> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		map<string, list<testpath> >::iterator mpos;
		for(mpos = pos->second.begin(); mpos != pos->second.end(); ++mpos)
		{
			for(list<testpath>::iterator lpos = mpos->second.begin(); lpos != mpos->second.end(); ++lpos)
			{
				(*lpos).preproc = -1;
				for(unsigned idx = 0; idx < (*lpos).nodes.count(); ++idx)
				{
					if((*lpos).nodes[idx].type.find("ISCONTROL") == 0)
					{
						for(set<string>::const_iterator spos = (*lpos).nodes[idx].refs.begin(); 
						spos != (*lpos).nodes[idx].refs.end(); ++spos)
						{
							(*lpos).refs.insert((*spos));
						}
					}
					for(set<string>::const_iterator spos2 = (*lpos).nodes[idx].defs.begin(); 
					spos2 != (*lpos).nodes[idx].defs.end(); ++spos2)
					{
						(*lpos).defs.insert((*spos2));
					}
				}
			}
		}
	}
}

void TestGen::arrange_testpath()
{
	map<string, Module*> modules = pdesign_->get_modules();
	for(map<int, map<string, list<testpath> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		map<string, list<testpath> >::iterator mpos;
		for(mpos = pos->second.begin(); mpos != pos->second.end(); ++mpos)
		{
			map<string, Module*>::iterator mpos2 = modules.find(mpos->first);
			if(mpos2 != modules.end())
			{
				//get the cfg from the found module
				Module_Cfgs* cfg = mpos2->second->get_cfg();
				//iterate over the cfg to assign refs and defs and control direction to testpath
				for(list<testpath>::iterator lpos = mpos->second.begin(); lpos != mpos->second.end(); ++lpos)
				{
					for(unsigned idx = 0; idx < cfg->cfgs->count(); ++idx)
					{
						if((*lpos).procid == (*(cfg->cfgs))[idx]->id)
						{
							(*lpos).sync = (*(cfg->cfgs))[idx]->sync;
							for(unsigned idx2 = 0; idx2 < (*lpos).nodes.count(); ++idx2)
							{
								(*lpos).nodes[idx2].type = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->type;
								(*lpos).nodes[idx2].defs = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->defs;
								(*lpos).nodes[idx2].refs = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->refs;
								(*lpos).nodes[idx2].condid = -1;
								(*lpos).nodes[idx2].lineno = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->lineno;
								if((*lpos).nodes[idx2].type.find("ISCONTROL") == 0)
								{
									(*lpos).nodes[idx2].condid = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->condid;
									for(int idx3 = 0; idx3 < (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc.count(); ++idx3)
									{
										if((idx2 + 1) < (*lpos).nodes.count())
										{
											if((*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc[idx3]->index == (*lpos).nodes[idx2 + 1].nodeid)
											{
												if((*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc[idx3]->caseitem.size() < 1)
												{
													(*lpos).nodes[idx2].usewhich = false;
													(*lpos).nodes[idx2].cond = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc[idx3]->cond;
													(*lpos).nodes[idx2].itemindex = -1;
												}
												else
												{
													(*lpos).nodes[idx2].usewhich = true;
													(*lpos).nodes[idx2].caseitem = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc[idx3]->caseitem;
													(*lpos).nodes[idx2].itemindex = (*(*(cfg->cfgs))[idx]->root)[(*lpos).nodes[idx2].nodeid]->dsuc[idx3]->caseitemidx;
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
			else
				error("Illegle Module name in testpath file", mpos->first.c_str());
		}
	}
}

void TestGen::sort_testpath()
{
	for(map<int, map<string, list<testpath> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		for(map<string, list<testpath> >::iterator mpos = pos->second.begin(); mpos != pos->second.end(); ++mpos)
		{
			for(list<testpath>::iterator lpos1 = mpos->second.begin(); lpos1 != mpos->second.end(); ++lpos1)
			{
				int i = (*lpos1).procid;
				if((*lpos1).sync) continue;
				for(list<testpath>::iterator lpos2 = mpos->second.begin(); lpos2 != mpos->second.end(); ++lpos2)
				{
					int j = (*lpos2).procid;
					if(i != j)
					{
						bool found = false;
						for(set<string>::const_iterator spos = (*lpos1).refs.begin(); spos != (*lpos1).refs.end(); ++spos)
						{
							if((*lpos2).defs.find(*spos) != (*lpos2).defs.end())
							{
								found = true;
								break;
							}
						}
						//lpos1's ref in the lpos2's def, so lpos2 proceed lpos1;
						if(found)
							(*lpos1).preproc = (*lpos2).procid;
					}
				}
			}
		}
	}
}

void TestGen::read_testpath(string file)
{
	ifstream in(file.c_str());
	if(!in) 
		error("cannot open input file", file.c_str());
	map<string, list<testpath> > mtps;
	testpath tp;
	string head;
	tpnode node;
	int cycle, nodeid;

	while(!in.eof())
	{
		in >> head;
		if(head == "cycle:")
		{
			if(mtps.size() > 0)
			{
				testpath_[cycle] = mtps;
				mtps.clear();
			}
			in >> cycle;
		}
		else
		{
			tp.modulename = head.substr(0, head.length()-1);
			in >> tp.procid;
			in >> head;
			while(in >> nodeid) 
			{
				node.nodeid = nodeid;
				tp.nodes = svector<tpnode>(tp.nodes, node);
				if(in.get() == 10) break;
			}
			tp.gened = false;
			mtps[tp.modulename].push_back(tp);
			tp.nodes = svector<tpnode>();
		}
	}

	if(mtps.size() > 0)
	{
		testpath_[cycle] = mtps;
		mtps.clear();
	}
	
	in.close();
}

void TestGen::build_testpath(string file)
{
	//read testpath from file and build testpath_;
	read_testpath(file);

	//get information for testpath_ node from design_
	arrange_testpath();

	//generate refs and defs for each test path
	gen_ref_def();

	//arrange the order for test pathes in each cycle belonged to the same module;
	sort_testpath();
}

ostream& TestGen::dump(ostream& o)
{
	o << "DUMP TEST PATHES:" << endl;
	for(map<int, map<string, list<testpath> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		o << "cycle: " << pos->first << endl;
		for(map<string, list<testpath> >::iterator lpos = pos->second.begin(); lpos != pos->second.end(); ++lpos)
		{
			for(list<testpath>::iterator lpos2 = lpos->second.begin(); lpos2 != lpos->second.end(); ++lpos2)
			{
				o << lpos->first << ": "	<< (*lpos2).procid << " : ";
				for(unsigned spos = 0; spos < (*lpos2).nodes.count(); ++spos)
				{
					o << (*lpos2).nodes[spos].nodeid << "(" << (*lpos2).nodes[spos].type;
					if((*lpos2).nodes[spos].type.find("ISCONTROL") == 0)
					{
						if((*lpos2).nodes[spos].usewhich) // case statement
						{
							for(set<PExpr*>::iterator cidx = (*lpos2).nodes[spos].caseitem.begin(); cidx != (*lpos2).nodes[spos].caseitem.end(); ++cidx)
								(*cidx)->dump(o);
						}
						else
						{
							o << ", " << (*lpos2).nodes[spos].cond << ")";
						}
					}
					else
					o << ")";
				}
				o << " refs = {";
				for(set<string>::const_iterator spos2 = (*lpos2).refs.begin(); spos2 != (*lpos2).refs.end(); ++spos2)
				{
					o << *spos2 << " ";
				}
				o << "}, defs ={";
				for(spos2 = (*lpos2).defs.begin(); spos2 != (*lpos2).defs.end(); ++spos2)
				{
					o << *spos2 << " ";
				}
				o << "}, preproc = " << (*lpos2).preproc << endl;
			}
		}
	}
	return o;
}

void TestGen::dump_cond_clp(ofstream& out, cond_time_expr ce, const string modname)
{
	//first locate the cetab by modname;
	map<string, list<cond_expr*> >::const_iterator modpos = cetab_.find(modname);
	map<string, set<Var*> >::const_iterator vmodpos = vartab_.find(modname);
	ostringstream expr;
	if((modpos != cetab_.end()) && (vmodpos != vartab_.end()))
	{
		//second find the cond_expr struct by procid and condid;
		for(list<cond_expr*>::const_iterator lpos = modpos->second.begin(); lpos != modpos->second.end(); ++lpos)
		{
			//call the design's dump_clp to dump the cond_expr;
			if(((*lpos)->procid == ce.procid) && ((*lpos)->condid == ce.condid))
			{
				for(set<Var*>::const_iterator vpos = vmodpos->second.begin(); vpos != vmodpos->second.end(); ++vpos)
				{
					for(set<RefVar*>::const_iterator rvpos = ce.refs.begin(); rvpos != ce.refs.end(); ++rvpos)
					{
						if((*vpos)->name == (*rvpos)->name)
						{
							(*rvpos)->width = (*vpos)->width;
							(*rvpos)->msb = (*vpos)->msb;
							(*rvpos)->lsb = (*vpos)->lsb;
							if(((*vpos)->ptype == NetNet::PINPUT) || ((*vpos)->ptype == NetNet::PINOUT))
								(*rvpos)->ptype = "INPUT";
							else
								(*rvpos)->ptype = "NOINPUT";
							(*rvpos)->assigntype = (*vpos)->assigntype;
							break;
						}
					}
				}
				NetProcTop* cur = design_->find_process();
				while(cur)
				{
					if((cur->scope()->name() == modname) && (cur->get_id() == ce.procid))
					{
						cur->dump_cond_clp(out, (*lpos)->lineno, ce.refs, clp_vars_, expr, ce.result, ce.itemindex, temp_idx);
						temp_idx += 1;
						break;
					}
					cur = design_->find_next_process(cur);
				}
				break;
			}
		}
	}
}

void TestGen::gen_clp_testpath(int cycle, string modname, list<testpath>& tps, const set<int>& cur, ofstream& out)
{
	//for all the test path
	//find the variable table for current module
	map<string, set<Var*> >::iterator vpos = vartab_.find(modname);
	for(list<testpath>::iterator pos = tps.begin(); pos != tps.end(); ++pos)
	{
		//for all the test path in this generate group
		for(set<int>::const_iterator spos = cur.begin(); spos != cur.end(); ++spos)
		{
			//find the test path for generation
			if(((*pos).procid == *spos))// && ((*pos).gened == false))
			{
				//for all the nodes in current test path
				for(unsigned idx = 0; idx < (*pos).nodes.count(); ++idx)
				{
					//find the condition node
					if((*pos).nodes[idx].type.find("ISCONTROL") == 0)
					{
						//change the condexpr table for these changed variable of current condexpr;
						//generate a condition expression using the condid,
						//then get the time and space number from var table
						//at last insert it into the ce_time_table;
						
						map<string, list<cond_expr*> >::const_iterator cepos = cetab_.find(modname);
						if(cepos != cetab_.end())
						{
							cond_time_expr ce2;//= new cond_time_expr;
							for(list<cond_expr*>::const_iterator lcepos = cepos->second.begin(); lcepos != cepos->second.end(); ++lcepos)
							{
								//find the cond expression by condid
								if(((*lcepos)->condid == (*pos).nodes[idx].condid) && ((*lcepos)->procid == (*pos).procid))
								{
									ce2.refs.clear();
									ce2.procid = (*lcepos)->procid;
									ce2.condid = (*lcepos)->condid;
									ce2.refs.clear();
									for(set<RefVar*>::const_iterator refvarpos = (*lcepos)->refs.begin(); refvarpos != (*lcepos)->refs.end(); ++refvarpos)
									{
										RefVar* rv = new RefVar;
										rv->name = (*refvarpos)->name;
										rv->time = (*refvarpos)->time;
										rv->space = (*refvarpos)->space;
										ce2.refs.insert(rv);
									}
									ce2.result = (*pos).nodes[idx].cond;
									ce2.itemindex = (*pos).nodes[idx].itemindex;

									//for all the refs of the condition expression, assign new time and space number
									for(set<Var*>::const_iterator rvpos = vpos->second.begin(); rvpos != vpos->second.end(); ++rvpos)
									{
										for(set<RefVar*>::const_iterator rvpos2 = ce2.refs.begin(); rvpos2 != ce2.refs.end(); ++rvpos2)
										{
											if((*rvpos)->name == (*rvpos2)->name)
											{
												(*rvpos2)->time = (*rvpos)->time;
												//if((*rvpos)->assigntype)
												(*rvpos2)->space = (*rvpos)->space;
											}
										}
									}
								}
							}
							dump_cond_clp(out, ce2, modname);
						}
					}
					else // for non-control node
					{
						for(set<string>::const_iterator dpos = (*pos).nodes[idx].defs.begin(); dpos != (*pos).nodes[idx].defs.end(); ++dpos)
						{
							set<RefVar*> refs, lrefs;
							refs.clear();
							lrefs.clear();
							RefVar* rv2;
							for(set<Var*>::iterator varspos = vpos->second.begin(); varspos != vpos->second.end(); ++varspos)
							{
								if(*dpos == (*varspos)->name)
								{
									(*varspos)->time = cycle;
									(*varspos)->space = (*varspos)->space + 1;
									rv2 = new RefVar;
									rv2->name = (*varspos)->name;
									rv2->time = (*varspos)->time;
									rv2->space = (*varspos)->space;
									rv2->width = (*varspos)->width;
									rv2->lsb = (*varspos)->lsb;
									rv2->msb = (*varspos)->msb;
									rv2->ptype = (*varspos)->ptype;
									rv2->assigntype = (*varspos)->assigntype;
									lrefs.insert(rv2);
									used_var_.insert(rv2);
									break;
								}
							}
							for(varspos = vpos->second.begin(); varspos != vpos->second.end(); ++varspos)
							{
								for(set<string>::iterator rpos = (*pos).nodes[idx].refs.begin(); rpos != (*pos).nodes[idx].refs.end(); ++rpos)
								{
									//cerr << *rpos << endl;
									if((*rpos) == (*varspos)->name)
									{
										RefVar* rv = new RefVar;
										rv->name = (*varspos)->name;
										rv->time = (*varspos)->time;
										if(rv->name == rv2->name)
											rv->space = (*varspos)->space - 1;
										else
											rv->space = (*varspos)->space;
										rv->width = (*varspos)->width;
										rv->lsb = (*varspos)->lsb;
										rv->msb = (*varspos)->msb;
										rv->ptype = (*varspos)->ptype == NetNet::PINPUT ? "INPUT" : "NOINPUT";
										rv->assigntype = (*varspos)->assigntype;
										refs.insert(rv);
										break;
									}
								}
							}
							ostringstream expr;
							NetProcTop* cur = design_->find_process();
							while(cur)
							{
								if((cur->scope()->name() == modname) && (cur->get_id() == (*pos).procid))
								{
									cur->dump_expr_clp(out, (*pos).nodes[idx].lineno, refs, lrefs, clp_vars_, expr, 0, -1, temp_idx);
									temp_idx += 1;
									break;
								}
								cur = design_->find_next_process(cur);
							}
							break;
						}
					}
				}
			}
		}
	}
}

void TestGen::gen_clp_body(ofstream& out)
{
	//for each cycle
	for(map<int, map<string, list<testpath> > >::iterator pos = testpath_.begin(); pos != testpath_.end(); ++pos)
	{
		//first we must initial the variables's time and space number
		for(map<string, set<Var*> >::iterator vpos = vartab_.begin(); vpos != vartab_.end(); ++vpos)
		{
			for(set<Var*>::iterator vspos = vpos->second.begin(); vspos != vpos->second.end(); ++vspos)
			{
				(*vspos)->space = 0;
				//(*vspos)->change_count = 0;
				if(((*vspos)->ptype == NetNet::PINPUT) || ((*vspos)->ptype == NetNet::PINOUT))
					(*vspos)->time = pos->first;
			}
		}
		//for each module
		for(map<string, list<testpath> >::iterator mpos = pos->second.begin(); mpos != pos->second.end(); ++mpos)
		{
			set<int> procs, procs2;
			int i = 100000;
			for(list<testpath>::iterator sortpos = mpos->second.begin(); sortpos != mpos->second.end(); ++sortpos)
			{
				if((*sortpos).preproc < i)
					i = (*sortpos).preproc;
			}
			procs.insert(i);
			while(procs.size() > 0)
			{
				procs2.clear();
				for(set<int>::const_iterator spos = procs.begin(); spos != procs.end(); ++spos)
				{
					for(list<testpath>::iterator lpos = mpos->second.begin(); lpos != mpos->second.end(); ++lpos)
					{
						if(((*lpos).preproc == *spos))// && ((*lpos).gened == false))
						{
							int i;
							i = (*lpos).procid;
							procs2.insert(i);
							//(*lpos).gened = true;
						}
					}
				}
				gen_clp_testpath(pos->first, mpos->first, mpos->second, procs2, out);
				procs.clear();
				for(spos = procs2.begin(); spos != procs2.end(); ++spos)
					procs.insert(*spos);
			}
		}
	}
}

void TestGen::dump_vartab()
{
	map<string, set<Var*> >::iterator modpos;
	for(modpos = vartab_.begin(); modpos !=  vartab_.end(); ++modpos)
	{
		cout << "Module name : " << modpos->first << endl;
		for(set<Var*>::iterator vpos = modpos->second.begin(); vpos != modpos->second.end(); ++vpos)
		{
			cout << "Variable : (" << (*vpos)->name << ", " << (*vpos)->time << ", " << (*vpos)->space << ")" << endl;
		}
	}
}

void TestGen::gen_clp(ofstream& out)
{
	ifstream head("head.pl");
	if(!head) 
		error("cannot open input file", "head.pl");

	ifstream tail("tail.pl");
	if(!tail) 
		error("cannot open input file", "tail.pl");

	ofstream output("body.pl");

	temp_idx = 0;
	gen_clp_body(output);
	
	output.close();
	ifstream body("body.pl");

	char ch;

	while(head.get(ch)) 
		out.put(ch);

	out.put('\n');
	out.put('\n');
	out.put('\n');

	dump_clpvar_define(out);

	while(body.get(ch)) 
		out.put(ch);

	while(tail.get(ch)) 
		out.put(ch);

	dump_clp_initialize(out);
	
	head.close();
	tail.close();
	body.close();
	out.close();
}

bool operator == (const clp_var&left, const clp_var&right)
{
	if(left.clpname == right.clpname)
		return true;
	else
		return false;
}

bool operator <= (const clp_var&left, const clp_var&right)
{
	if(left.clpname <= right.clpname)
		return true;
	else
		return false;
}

bool operator <  (const clp_var&left, const clp_var&right)
{
	if(left.clpname < right.clpname)
		return true;
	else
		return false;
}

ofstream& TestGen::dump_clpvar_define(ofstream& out)
{
	svector<clp_var> clpvars, oclpvars;
	for(set<clp_var>::const_iterator clppos = clp_vars_.begin(); clppos != clp_vars_.end(); ++clppos)
	{
		if((*clppos).type == "INPUT")
			clpvars = svector<clp_var>(clpvars, *clppos);
		if((*clppos).type == "OUTPUT")
			oclpvars = svector<clp_var>(oclpvars, *clppos);
	}

	if(clpvars.count() > 0)
	{
		out << "INPUTS = [";
		out << clpvars[0].clpname;
		for(unsigned idx = 1; idx < clpvars.count(); ++idx)
		{
			out << ", ";
			out << clpvars[idx].clpname;
		}
		out << "]," << endl;
	}
	
	if(oclpvars.count() > 0)
	{
		out << "OUTPUTS = [";
		out << oclpvars[0].clpname;
		for(unsigned oidx = 1; oidx < oclpvars.count(); ++oidx)
		{
			out << ", ";
			out << oclpvars[oidx].clpname;
		}
		out << "]," << endl;
	}

	unsigned lsb, msb;
	lsb = 0;
	for(clppos = clp_vars_.begin(); clppos != clp_vars_.end(); ++clppos)
	{
		msb = pow(2, (*clppos).width) - 1;
		out << "fd_domain(" << (*clppos).clpname << " , " << lsb << " , " << msb << ")," << endl;
	}

	return out;
}

ofstream& TestGen::dump_clp_initialize(ofstream& out)
{
	unsigned out_count = 0;
	out << endl;
	for(set<clp_var>::const_iterator clppos = clp_vars_.begin(); clppos != clp_vars_.end(); ++clppos)
	{
		if((*clppos).type == "OUTPUT")
			out_count += 1;
	}
	
	if(out_count > 0)
	{
		out << ":-initialization(q([";
		out << "1";
		for(unsigned idx = 1; idx < out_count; ++idx)
		{
			out << ", 1";
		}
		out << "]))." << endl;
	}
	return out;
}