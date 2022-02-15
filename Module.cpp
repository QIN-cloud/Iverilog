#include "config.h"
#include <windows.h>
#include "Module.h"
#include "PGate.h"
#include "PWire.h"
#include "Statement.h"
#include "netlist.h"
#include <assert.h>
#include "DDModel.h"
#include "testpath.h"
#include "Make_Route.h"
#include <time.h>

/* n is a permallocated string. */
Module::Module(const char*n)
: name_(strdup(n))
{
}

Module::~Module()
{
      free(name_);
}

void Module::add_gate(PGate*gate)
{
      gates_.push_back(gate);
}

void Module::add_task(const string&name, PTask*task)
{
      tasks_[name] = task;
}

void Module::add_function(const string &name, PFunction *func)
{
      funcs_[name] = func;
}

PWire* Module::add_wire(PWire*wire)
{
      PWire*&ep = wires_[wire->path()];
      if (ep) return ep;

      assert(ep == 0);
      ep = wire;
      return wire;
}

void Module::add_behavior(PProcess*b)
{
      behaviors_.push_back(b);
}

unsigned Module::port_count() const
{
      return ports.count();
}

/*
 * Return the array of PEIdent object that are at this port of the
 * module. If the port is internally unconnected, return an empty
 * array. 
 */
const svector<PEIdent*>& Module::get_port(unsigned idx) const
{
      assert(idx < ports.count());
      static svector<PEIdent*> zero;

      if (ports[idx])
	    return ports[idx]->expr;
      else
	    return zero;
}

string& Module::get_port_name(unsigned idx)
{
	assert(idx < ports.count());
	static string tmps("");
	if(ports[idx])
	  return ports[idx]->name;
	else
	  return tmps;
}

unsigned Module::find_port(const string&name) const
{
      assert(name != "");
      for (unsigned idx = 0 ;  idx < ports.count() ;  idx += 1) {
	    if (ports[idx] == 0) {
		    /* It is possible to have undeclared ports. These
		       are ports that are skipped in the declaration,
		       for example like so: module foo(x ,, y); The
		       port between x and y is unnamed and thus
		       inaccessible to binding by name. */
		  continue;
	    }
	    assert(ports[idx]);
	    if (ports[idx]->name == name)
		  return idx;
      }

      return ports.count();
}


PWire* Module::get_wire(const hname_t&name) const
{
      map<hname_t,PWire*>::const_iterator obj = wires_.find(name);
      if (obj == wires_.end())
	    return 0;
      else
	    return (*obj).second;
}

PTask* Module::get_task(const string&name)
{
	map<string, PTask*>::const_iterator obj = tasks_.find(name);
	if(obj==tasks_.end())
		return 0;
	else
		return (*obj).second;
}

PGate* Module::get_gate(const string&name)
{
      for (list<PGate*>::iterator cur = gates_.begin()
		 ; cur != gates_.end()
		 ; cur ++ ) {

	    if ((*cur)->get_name() == name)
		  return *cur;
      }

      return 0;
}

const map<hname_t,PWire*>& Module::get_wires() const
{
      return wires_;
}

const list<PGate*>& Module::get_gates() const
{
      return gates_;
}

const list<PProcess*>& Module::get_behaviors() const
{
      return behaviors_;
}

void Module::adjust_decl()
{
	//int i;
	map<hname_t,PWire*>::iterator obj;
	for(obj = wires_.begin(); obj != wires_.end();)
	{
		if((*obj).second->path().component_count() > 1){
			map<string,PTask*>::iterator t = tasks_.find(string((*obj).second->path().peek_name((*obj).second->path().component_count()-2)));
			map<string, PFunction*>::iterator f = funcs_.find(string((*obj).second->path().peek_name((*obj).second->path().component_count()-2)));
			if(t != tasks_.end()){
				(*t).second->add_decl((*obj).second);
			}
			else if(f != funcs_.end()){
				(*f).second->add_decl((*obj).second);
			}
			wires_.erase(obj++);
		}
		else
			++obj;
	}
}

set<string>& Module::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	list<PProcess*>::const_iterator ppos;
	map<string,PTask*>::const_iterator tpos;
    map<string,PFunction*>::const_iterator fpos;

	for(ppos = behaviors_.begin(); ppos != behaviors_.end(); ++ppos)
		for(pos = (*ppos)->get_funcs().begin(); pos != (*ppos)->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	for(tpos = tasks_.begin(); tpos != tasks_.end(); ++tpos)
		for(pos = (*tpos).second->get_funcs().begin(); pos != (*tpos).second->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	for(fpos = funcs_.begin(); fpos != funcs_.end(); ++fpos)
		for(pos = (*fpos).second->get_funcs().begin(); pos != (*fpos).second->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	return *tmp;
}

set<string>& Module::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	list<PGate*>::const_iterator ppos;
	set<string> tmp1;

	for(ppos = gates_.begin(); ppos != gates_.end(); ++ppos)
	{
		tmp1 = (*ppos)->get_modname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

void Module::traversal_proc()
{
	unsigned count = 1;
	for(list<PProcess*>::const_iterator pos = behaviors_.begin(); pos != behaviors_.end(); ++pos)
	{
		(*pos)->set_id(count);
		count = count + 1;
	}
}

void Module::build_cfgs()
{
	cfg_ = mn_->build_cfgs();
}

ostream& Module::dump_cfg(ostream& o)
{
	for(unsigned y = 0; y < cfg_->cfgs->count(); ++y)
	{
		o << "Process ID: " << (*(cfg_->cfgs))[y]->id << endl;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[y]->root->count(); ++z)
		{
			o << "Index = " << z <<", ";
			o << *(*((*(cfg_->cfgs))[y]->root))[z];
			o << endl;
		}
	}

	return o;
}

void Module::build_vartable(Design* d)
{
	NetScope* scope;
	Var* var;
	string s = name_;
	list<NetScope*> scopes = d->find_root_scopes();
	for(list<NetScope*>::const_iterator pos = scopes.begin();
		pos != scopes.end(); ++pos)
		{
			if(s == (*pos)->basename())
			{
				scope = *pos;
				break;
			}
		}
	int count = 0;
	for(map<hname_t,PWire*>::const_iterator spos = wires_.begin(); spos != wires_.end(); ++spos)
	{
		string o = spos->first.peek_name(spos->first.component_count() - 1);
		var = scope->BuildVar(o, count);
		if(var) vartab_.insert(var);
		count += 1;
	}
}

ostream& Module::dump_vartable(ostream& o)
{
	o << "DUMP Vars Table:" << endl;
	for(set<Var*>::const_iterator sv = vartab_.begin(); sv != vartab_.end(); ++sv)
		o << (*sv)->varidx << ":" << (*sv)->name << ":" << (*sv)->width << ":" << (*sv)->msb << ":" << (*sv)->lsb << ":" << (*sv)->time << ":" << (*sv)->space << ":" << (*sv)->ptype << endl;
	return o;
}

void Module::build_cetable()
{
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		unsigned curprocid = (*(cfg_->cfgs))[idx]->id;
		unsigned count = 0;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[idx]->root->count(); ++z)
		{
			if((*((*(cfg_->cfgs))[idx]->root))[z]->type.find("ISCONTROL") == 0)
			{
				cond_expr* ce = new cond_expr;
				ce->procid = curprocid;
				ce->condid = count;
				ce->lineno = (*((*(cfg_->cfgs))[idx]->root))[z]->lineno;
				(*((*(cfg_->cfgs))[idx]->root))[z]->condid = count;
				svector<string> sset;
				for(unsigned x = 0;	x < (*((*(cfg_->cfgs))[idx]->root))[z]->expr.count(); ++x)
				{
					sset = svector<string>(sset, (*((*(cfg_->cfgs))[idx]->root))[z]->expr[x]->get_vars());
				}
				for(unsigned y = 0; y < sset.count(); ++y)
				{
					RefVar* rv = new RefVar;
					rv->name = sset[y];
					rv->time = 0;
					rv->space = 0;
					ce->refs.insert(rv);
				}
				ce->expr = (*((*(cfg_->cfgs))[idx]->root))[z]->expr;
				cetab_.push_back(ce);
				count = count + 1;
			}
		}
	}
}

ostream& Module::dump_cetable(ostream& o)
{
	for(list<cond_expr*>::const_iterator pos = cetab_.begin(); pos != cetab_.end(); ++pos)
	{
		o << "Process ID = " << (*pos)->procid << " , Condition ID = " << (*pos)->condid;
		o << " , expression = {";
		for(unsigned idx = 0; idx < (*pos)->expr.count(); ++idx)
		{
			(*pos)->expr[idx]->dump(o);
			o << ", ";
		}
		o << "} , Lineno = " << (*pos)->lineno << ", Refs = (name:time:space){";
		for(set<RefVar*>::const_iterator rpos = (*pos)->refs.begin(); rpos != (*pos)->refs.end(); ++rpos)
		{
			o << (*rpos)->name << ":" << (*rpos)->time << ":" << (*rpos)->space << ", ";
		}
		o << "}" << endl;
	}
	return o;
}

void Module::adjust_assigntype()
{
	//for each cfg
	for(unsigned cfgidx = 0; cfgidx < cfg_->cfgs->count(); ++cfgidx)
	{
		//for each node in cfg
		for(unsigned nidx = 0; nidx < (*(cfg_->cfgs))[cfgidx]->root->count(); ++nidx)
		{
			set<string> tmp = (*(*(cfg_->cfgs))[cfgidx]->root)[nidx]->defs;
			string stmp = (*(*(cfg_->cfgs))[cfgidx]->root)[nidx]->assign_type;
			//for each defs in node
			for(set<string>::const_iterator dpos = tmp.begin(); dpos != tmp.end(); ++dpos)
			{
				//for each var in vartab;
				for(set<Var*>::const_iterator vpos = vartab_.begin(); vpos != vartab_.end(); ++vpos)
				{
					if(((*vpos)->name == *dpos) && ((*vpos)->ptype != NetNet::PINPUT))
					{
						if(stmp == "NOCONTROL.BLOCKING")
							(*vpos)->assigntype = 0;
						if(stmp == "NOCONTROL.NONBLOCKING")
							(*vpos)->assigntype = 1;
						break;
					}
				}
			}
		}
	}
}

void Module::build_ddmodel()
{
	for(set<Var*>::const_iterator pos = vartab_.begin(); pos != vartab_.end(); ++pos)
	{
		if((*pos)->ptype != NetNet::PINPUT)
		{
			for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
			{
				bool find = false;
				unsigned index;
				for(set<string>::const_iterator dpos = (*(cfg_->cfgs))[idx]->defs.begin(); dpos != (*(cfg_->cfgs))[idx]->defs.end(); ++dpos)
				{
					if((*pos)->name == *dpos)
					{
						find = true;
						index = idx;
						break;
					}
				}
				if(find)
				{
					if((*(cfg_->cfgs))[idx]->root->count() > 2)
					{
						dd_var * var = new dd_var;
						//build ddmodel for var
						for(unsigned didx = 0; didx < (*(cfg_->cfgs))[idx]->root->count(); ++didx)
						{
							(*(*(cfg_->cfgs))[idx]->root)[didx]->visited = false;
						}
						var->set_name((*pos)->name);
						var->add_dd_node("",(*pos)->varidx,(*(cfg_->cfgs))[idx]->id,0,false,(*pos)->varidx);
						if((*(*(cfg_->cfgs))[idx]->root)[1]->type == "ISCONTROL.EVENT")
							build_dd_model("1", var, (*(cfg_->cfgs))[idx]->id, idx, (*(*(cfg_->cfgs))[idx]->root)[2]);
						else
							build_dd_model("1", var, (*(cfg_->cfgs))[idx]->id, idx, (*(*(cfg_->cfgs))[idx]->root)[1]);
						ddmodel_.push_back(var);
						break;
					}
				}
			}
		}
	}
}

void Module::build_dd_model(string TF, dd_var * ddn, int cfgno, int cfgidx, Cfg_Node* n)
{
	unsigned type = -1;
	if(n->type == "ISCONTROL.LOOP")
		type = 0;
	if((n->type == "ISCONTROL.EVENT") || (n->type == "ISCONTROL.CONDITION"))
		type = 1;
	if((n->type == "ISCONTROL.CASE")||(n->type == "ISCONTROL.CASEX")||(n->type == "ISCONTROL.CASEZ"))
		type = 2;
	if((n->type == "NOCONTROL") || (n->type == "STARTNODE") || (n->type == "ASSIGN"))
		type = 3;
	//if(n->type == "ENDNODE") return;


	unsigned dsidx;
	Cfg_Node* T, *F;
	unsigned Td, Fd;

	//assert((type != 0)&&(type!=1)&&(type!=2)&&(type!=3));
	//cout << "At Line:" << n->lineno << ", Type is "<< n->type << endl;
	switch(type)
	{
	default:
		//cerr << "Fatal Errors in build_dd_model variable 'type' with unknown value" << endl;
		//exit(-1);
		return;
	case 0:	//"ISCONTROL.LOOP"
			/*
			如果 该节点的 走过标记 为 真 则 返回（return）
			将该节点加入dd模型
			将该节点的走过标记置为 真
			假设该节点的2个后继分别为 T 和F（T代表真分支F代表假分支）
			对T：
			build_dd_model(TF + "1",ddn, T)
			
			  对F：
			  build_dd_model(TF + "0",ddn, F)
			  
		*/
		if(n->visited)return;
		ddn->add_dd_node(TF.c_str(),n->condid,cfgno,0,true,n->lineno);
		n->visited = true;
		for(dsidx = 0; dsidx < n->dsuc.count(); ++dsidx)
		{
			if(n->dsuc[dsidx]->cond)
				Td = n->dsuc[dsidx]->index;
			else
				Fd = n->dsuc[dsidx]->index;
		}
		T = (*(*(cfg_->cfgs))[cfgidx]->root)[Td];
		F = (*(*(cfg_->cfgs))[cfgidx]->root)[Fd];
		build_dd_model(TF + "1",ddn, cfgno, cfgidx, T);
		build_dd_model(TF + "0",ddn, cfgno, cfgidx, F);
		break;
	case 1: //"ISCONTROL.EVENT", "ISCONTROL.CONDITION"
			/*
			将该节点加入dd模型
			将该节点的走过标记置为 真
			假设该节点的2个后继分别为 T 和F（T代表真分支F代表假分支）
			对T：
			build_dd_model(TF + "1",ddn, T)
			
			  对F：
			  build_dd_model(TF + "0",ddn, F)
			  
			*/
		ddn->add_dd_node(TF.c_str(),n->condid,cfgno,0,true,n->lineno);
		n->visited = true;
		for(dsidx = 0; dsidx < n->dsuc.count(); ++dsidx)
		{
			if(n->dsuc[dsidx]->cond)
				Td = n->dsuc[dsidx]->index;
			else
				Fd = n->dsuc[dsidx]->index;
		}
		T = (*(*(cfg_->cfgs))[cfgidx]->root)[Td];
		F = (*(*(cfg_->cfgs))[cfgidx]->root)[Fd];
		build_dd_model(TF + "1",ddn, cfgno, cfgidx, T);
		build_dd_model(TF + "0",ddn, cfgno, cfgidx, F);
		break;
	case 2: //"ISCONTROL.CASE"
			/*
			将该节点加入dd模型
			将该节点的走过标记置为 真
			对b[i]：
			将b[i]用函数add_dd_case_node加入dd模型
			build_dd_model(TF + "(b[i]的表达式编号)",ddn, b[i])
			*/
		ddn->add_dd_case_head_node(TF.c_str(),n->condid,cfgno,0,n->lineno);
		n->visited = true;
		for(dsidx = 0; dsidx < n->dsuc.count(); ++dsidx)
		{
			char* s = new char[3];
			sprintf(s, "%d", n->dsuc[dsidx]->caseitemidx);
			ddn->add_dd_case_node(TF.c_str(),n->dsuc[dsidx]->caseitemidx,cfgno,0,n->dsuc[dsidx]->caseitemidx);//n->lineno);
			build_dd_model(TF + "(" + s + ")",ddn, cfgno, cfgidx, (*(*(cfg_->cfgs))[cfgidx]->root)[n->dsuc[dsidx]->index]);
		}
		break;
	case 3: //"NOCONTROL", "STARTNODE"
			/*
			temp = 该节点
			当节点 temp的类型为 "NOCONTROL"  且 temp 的 走过标记 为 假 时
			{
			如果 变量 var 属于 该节点的 def 则　将该节点加入到dd模型中
			t = temp
			temp = temp 的下一个节点
			如果 temp 为空则 返回
			｝
			如果temp节点的类型为 "ISCONTROL.LOOP"  且 t 是temp的 control中的元素 则 返回
			如果 temp的走过标记为 真 时
			则 将 指针合并！！并 返回
			否则 build_dd_model(TF ,ddn, temp)
			*/
		Cfg_Node *temp, *t;
		temp = n;
		string varname;
		bool find = false;
		while((temp->type == "NOCONTROL"))// && (temp->visited == false))
		{
			for(set<Var*>::const_iterator pos = vartab_.begin(); pos != vartab_.end(); ++pos)
			{
				if((*pos)->varidx == ddn->var_head_ptr->con_index)
					varname = (*pos)->name;
			}
			find  = false;
			for(set<string>::const_iterator spos = temp->defs.begin(); spos != temp->defs.end(); ++spos)
			{
				if(varname == *spos)
				{
					find = true;
					break;
				}
			}
			if(find)
			{
				ddn->add_dd_node(TF.c_str(),temp->exprid,cfgno,0,false,temp->lineno);
			}
			t = temp;
			if(temp->dsuc.count() > 0)
				temp = (*(*(cfg_->cfgs))[cfgidx]->root)[temp->dsuc[0]->index];
			if(temp->type == "ENDNODE")
				return;
		}
		find = false;
		if(temp->type == "ISCONTROL.LOOP")
		{
			for(set<int, less<int> >::const_iterator ipos = temp->infls.begin(); ipos != temp->infls.end(); ++ipos)
				if(t->lineno == (*ipos))
				{
					find = true;
					break;
				}
			if(find)
				return;
		}
		if(temp->visited)
		{
			//指针合并
			ddn->combine_2_link(ddn->find_dd_node_from_cons(temp->condid),TF.c_str());
			return;
		}
		else
		{
			build_dd_model(TF.c_str(),ddn, cfgno, cfgidx, temp);
		}

		break;
	}
}

ostream& Module::dump_ddmodel(ostream& o)
{
	for(list<dd_var*>::iterator lpos = ddmodel_.begin(); lpos != ddmodel_.end(); ++lpos)
	{
		(*lpos)->dump(o);
	}
	
	return o;
}

void Module::test(ostream& o)
{
	for(unsigned idx = 1; idx <100; ++idx)
	{
		for(set<Var*>::iterator vpos = vartab_.begin(); vpos != vartab_.end(); ++vpos)
		{
			(*vpos)->time = idx;
			(*vpos)->space = idx;
		}
		dump_vartable(o);
		dump_cetable(o);
	}
}

void Module::set_var_space(set<string>& du)
{
	for(set<string>::iterator dupos = du.begin(); dupos != du.end(); ++dupos)
	{
		//find the variable
		for(set<Var*>::iterator vspos = vartab_.begin(); vspos != vartab_.end(); ++vspos)
		{
			if((*vspos)->name == (*dupos))
			{
				if((*vspos)->ptype != NetNet::PINPUT)
					(*vspos)->space = (*vspos)->space + 1;
			}
		}
	}
}

void Module::set_var_time(int t)
{
	//first we must initial the variables's time and space number
	for(set<Var*>::iterator vpos = vartab_.begin(); vpos != vartab_.end(); ++vpos)
	{
		(*vpos)->time = t;
		(*vpos)->space = 0;
	}	
}

cond_time_expr Module::gen_ce(int procid, int condid)
{
	cond_time_expr cte;

	cout << endl << endl << "Before" << endl;
	dump_cetable(cout);

	for(list<cond_expr*>::const_iterator lcepos = cetab_.begin(); lcepos != cetab_.end(); ++lcepos)
	{
		//find the cond expression by condid
		if(((*lcepos)->condid == condid) && ((*lcepos)->procid == procid))
		{
			cte.procid = (*lcepos)->procid;
			cte.condid = (*lcepos)->condid;
			cte.refs.clear();
			for(set<RefVar*>::const_iterator vpos = (*lcepos)->refs.begin(); vpos != (*lcepos)->refs.end(); ++vpos)
			{
				RefVar* rv = new RefVar;
				rv->name = (*vpos)->name;
				rv->time = (*vpos)->time;
				rv->space = (*vpos)->space;
				cte.refs.insert(rv);
			}
			
			//for all the refs of the condition expression, assign new time and space number
			for(set<Var*>::const_iterator rvpos = vartab_.begin(); rvpos != vartab_.end(); ++rvpos)
			{
				for(set<RefVar*>::iterator rvpos2 = cte.refs.begin(); rvpos2 != cte.refs.end(); ++rvpos2)
				{
					if((*rvpos)->name == (*rvpos2)->name)
					{
						(*rvpos2)->time = (*rvpos)->time;
						(*rvpos2)->space = (*rvpos)->space;
					}
				}
			}
		}
	}

	cout << endl << endl << "After" << endl;
	dump_cetable(cout);
	
	return cte;
}

void Module::build_exprtable()
{
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		unsigned curprocid = (*(cfg_->cfgs))[idx]->id;
		unsigned count = 0;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[idx]->root->count(); ++z)
		{
			if(((*((*(cfg_->cfgs))[idx]->root))[z]->type == "NOCONTROL") && ((*((*(cfg_->cfgs))[idx]->root))[z]->expr.count() > 0))
			{
				assign_expr* ce = new assign_expr;
				ce->procid = curprocid;
				ce->exprid = count;
				ce->lineno = (*((*(cfg_->cfgs))[idx]->root))[z]->lineno;
				ce->assign_type = (*((*(cfg_->cfgs))[idx]->root))[z]->assign_type;
				(*((*(cfg_->cfgs))[idx]->root))[z]->exprid = count;
				ce->expr = (*((*(cfg_->cfgs))[idx]->root))[z]->expr;
				aetab_.push_back(ce);
				count = count + 1;
			}
		}
	}
}

ostream& Module::dump_exprtable(ostream& o)
{
	for(list<assign_expr*>::iterator pos = aetab_.begin(); pos != aetab_.end(); ++pos)
	{
		o << "Process ID : " << (*pos)->procid;
		o << " Assign Expression ID : " << (*pos)->exprid;
		o << " Line No. : " << (*pos)->lineno << " ";
		o << "Assign Type is : " << (*pos)->assign_type << " ";
		o << "Expression = {";
		for(unsigned idx = 0; idx < (*pos)->expr.count(); ++idx)
		{
			(*pos)->expr[idx]->dump(o);
			o << ", ";
		}
		o << "}" << endl;
	}
	return o;
}

void Module::report_path_stmt(int type, ostream& report)
{
	if(type == 0)
	{
		report << "Path Coverage Report" << endl;
		report << "---------------------------------------------------" << endl;
		for(map<unsigned, path_stat>::iterator mpos = paths.begin(); mpos != paths.end(); ++mpos)
		{
			report << "Process at Line : " << mpos->first << endl;
			mpos->second.report_path(report);
		}
	}

	if(type == 1)
	{
		report << "Statement Coverage Report" << endl;
		report << "---------------------------------------------------" << endl;
		for(map<unsigned, stmt_stat>::iterator mpos = statements.begin(); mpos != statements.end(); ++mpos)
		{
			report << "Process at Line : " << mpos->first << endl;
			mpos->second.report_stmt(report);
		}
	}
}

void Module::pathcover(Design* des, const char* vcd_file, ostream& report, int type/*0 for path, 1 for statement*/)
{
	FILE* vcd_handle;        /* Pointer to opened VCD file */
	
	if( (vcd_handle = fopen( vcd_file, "r" )) != NULL ) 
	{
		enter_scope = false;
		scope_count = 0;
		found_in_vcd = false;
		vcd_parse_def( vcd_handle );
		dump_vcd_vars();
		cur_sim_time = 0;
		
		if(type==0)
			paths.clear();
		if(type==1)
			statements.clear();
		report << "Module name:" << name_ << endl;
		vcd_parse_sim( des, vcd_handle, report, type);
		sim_timestep(des, report, type);
		report_path_stmt(type, report);
	} 
	else 
	{
		cout << "Unable to open specified VCD file" << endl;
		exit( 1 );
	}
}

void Module::togglecover(const char* vcd_file, ostream& report)
{
	FILE* vcd_handle;        /* Pointer to opened VCD file */
	
	if( (vcd_handle = fopen( vcd_file, "r" )) != NULL ) 
	{
		enter_scope = false;
		scope_count = 0;
		found_in_vcd = false;
		vcd_parse_def( vcd_handle );
		dump_vcd_vars();
		vcd_parse_toggle(vcd_handle);
		toggle_timestep();
		report_toggle(report);
	}
}

void Module::fsmcover(const char* vcd_file, string var, ostream& report)
{
	FILE* vcd_handle;        /* Pointer to opened VCD file */
	
	if( (vcd_handle = fopen( vcd_file, "r" )) != NULL ) 
	{
		enter_scope = false;
		scope_count = 0;
		found_in_vcd = false;
		
		int sptr1=0, sptr2;
		
		fsm_stat fs;
		
		//取输入参数中要监控的状态寄存器信号名。
		string stmp(var);
		while((sptr2=stmp.find(",", sptr1 ))!=-1)
		{
			char* tmp = new char(sizeof(stmp.substr(sptr1,sptr2-sptr1).c_str()));
			strcpy(tmp, stmp.substr(sptr1,sptr2-sptr1).c_str());
			fsm[tmp] = fs;
			sptr1=sptr2+1;
		}
		//the last variable
		if(sptr2==-1)
		{
			char* tmp = new char(sizeof(stmp.substr(sptr1,stmp.length()-sptr1).c_str()));
			strcpy(tmp, stmp.substr(sptr1,stmp.length()-sptr1).c_str());
			fsm[tmp] = fs;
		}

		vcd_parse_def( vcd_handle );
		dump_vcd_vars();
		vcd_parse_fsm(vcd_handle);
		fsm_timestep();
		report_fsm(report);
	}
}

void Module::vcd_parse_fsm(FILE* vcd)
{
	char token[4100];         /* Current token from VCD file       */
	int  chars_read;          /* Number of characters scanned in   */
	
	//cur_sim_time = -1;
	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ) 
	{
		assert( chars_read <= 4100 );
		if( token[0] == '$' ) 
		{
			/* Currently ignore all simulation keywords */
		} 
		else if( (token[0] == 'b') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_vector( vcd, (token + 1) );
		} 
		else if( (token[0] == 'r') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_ignore( vcd );
		} 
		else if( token[0] == '#' ) 
		{
			if( cur_sim_time >= 0 ) {
				fsm_timestep();
				for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
					vpos->second.pre_val_ = vpos->second.cur_val_;
			}
			cur_sim_time = atol( token + 1 );
		} 
		else if( (token[0] == '0') ||
			(token[0] == '1') ||
			(token[0] == 'x') ||
			(token[0] == 'X') ||
			(token[0] == 'z') ||
			(token[0] == 'Z') ) 
		{
			set_symbol_char( token + 1, token[0] );
		} 
		else 
		{
			cerr << "Badly placed token " << token << endl;
			exit( 1 );
		}
	}
}

void Module::fsm_timestep()
{
	for(map<string, fsm_stat>::iterator fpos = fsm.begin(); fpos != fsm.end(); ++fpos)
	{
		for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
		{
			if(fpos->first == vpos->first)
			{
				fpos->second.add_covered_state(vpos->second.cur_val_);
				covered_tran ct;
				ct.from = vpos->second.pre_val_;
				ct.to = vpos->second.cur_val_;
				fpos->second.add_covered_tran(ct);
				break;
			}
		}
	}
}

void Module::report_fsm(ostream& report)
{
	for(map<string, fsm_stat>::iterator fpos = fsm.begin(); fpos != fsm.end(); ++fpos)
	{
		report << "State Register Variable: " << fpos->first << endl;
		fpos->second.report_fsm(report);
	}
}

void Module::report_toggle(ostream& report)
{
	for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
	{
		vpos->second.toggle_report(report);
	}
}

void Module::toggle_timestep()
{
	for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
	{
		vpos->second.toggle01();
		vpos->second.toggle10();
	}
}

void Module::vcd_parse_toggle(FILE* vcd)
{
	char token[4100];         /* Current token from VCD file       */
	int  chars_read;          /* Number of characters scanned in   */
	
	//cur_sim_time = -1;
	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ) 
	{
		assert( chars_read <= 4100 );
		if( token[0] == '$' ) 
		{
			/* Currently ignore all simulation keywords */
		} 
		else if( (token[0] == 'b') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_vector( vcd, (token + 1) );
		} 
		else if( (token[0] == 'r') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_ignore( vcd );
		} 
		else if( token[0] == '#' ) 
		{
			if( cur_sim_time >= 0 ) {
				toggle_timestep();
				for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
					vpos->second.pre_val_ = vpos->second.cur_val_;
			}
			cur_sim_time = atol( token + 1 );
		} 
		else if( (token[0] == '0') ||
			(token[0] == '1') ||
			(token[0] == 'x') ||
			(token[0] == 'X') ||
			(token[0] == 'z') ||
			(token[0] == 'Z') ) 
		{
			set_symbol_char( token + 1, token[0] );
		} 
		else 
		{
			cerr << "Badly placed token " << token << endl;
			exit( 1 );
		}
	}
}

void Module::dump_vcd_vars()
{
	for(map<string, vcd_var>::const_iterator pos = vcd_vars_.begin(); pos != vcd_vars_.end(); ++pos)
	{
		cout << "Symbol in VCD file is : " << pos->first << ", Variable name is : " << pos->second.name_;
		cout << ", Prevoius value is : " << pos->second.pre_val_ << ", Current value is : " << pos->second.cur_val_  << endl;
	}
}

void Module::vcd_parse_def_ignore( FILE* vcd ) 
{
	bool end_seen = false;  /* If set to true, $end keyword was seen */
	char token[256];        /* String value of current token         */
	int  tokval;            /* Set to number of tokens found         */
	int  chars_read;        /* Number of characters scanned in       */
	
	while( !end_seen && ((tokval = fscanf( vcd, "%s%n", token, &chars_read )) == 1) ) 
	{
		assert( chars_read <= 256 );
		if( strncmp( "$end", token, 4 ) == 0 ) {
			end_seen = true;
		}
	}
	assert( end_seen );
}

void Module::vcd_parse_def_var( FILE* vcd ) 
{
	char type[256];        /* Variable type                   */
	int  size;             /* Bit width of specified variable */
	char id_code[256];     /* Unique variable identifier_code */
	char ref[256];         /* Name of variable in design      */
	char tmp[15];          /* Temporary string holder         */
	int  msb = -1;         /* Most significant bit            */
	int  lsb = -1;         /* Least significant bit           */
	unsigned wid_;
	long lsb_, msb_;
	//char t1, t2;
	
	if( fscanf( vcd, "%s %d %s %s %s", type, &size, id_code, ref, tmp ) == 5 )
	{
		/* Make sure that we have not exceeded array boundaries */
		assert( strlen( type )    <= 256 );
		assert( strlen( ref )     <= 256 );
		assert( strlen( tmp )     <= 15  );
		assert( strlen( id_code ) <= 256 );
		
		if( strncmp( "$end", tmp, 4 ) != 0 ) 
		{
			/* A bit select was specified for this signal, get the size */
			if( sscanf( tmp, "\[%d:%d]", &msb, &lsb ) != 2 ) 
			{        
				if( sscanf( tmp, "\[%d]", &lsb ) != 1 ) 
				{
					cerr << "Unrecognized $var format" << endl;
					exit( 1 );
				}
				else
				{
					msb = lsb;
				}
			}
			if( (fscanf( vcd, "%s", tmp ) != 1) || (strncmp( "$end", tmp, 4 ) != 0) ) 
			{
				cerr << "Unrecognized $var format" << endl;
				exit( 1 );
			}
		} 
		else 
		{
			msb = size - 1;
			lsb = 0;
		}

		vcd_id vi;
		vi.name_ = ref;
		vi.lsb = lsb;
		vi.msb = msb;
		vcd_ids_[id_code] = vi;
		
		bool found = false;
		for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
		{
			if(vpos->first == ref)
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			vcd_var vv;
			for(map<hname_t,PWire*>::const_iterator wpos = wires_.begin(); wpos != wires_.end(); ++wpos)
			{
				if(strcmp(wpos->first.peek_tail_name(), ref) == 0)
				{
					wid_ = wpos->second->getwidth();
					lsb_ = wpos->second->getlsb();
					msb_ = wpos->second->getmsb();
				}
			}
			
			vv.name_ = ref;
			vv.little_endia = msb_ >= lsb_ ? true : false;
			verinum vn(verinum::Vx, wid_);
			vv.pre_val_ = vn;
			vv.cur_val_ = vn;
			vv.width = wid_;
			vv.lsb = lsb_;
			vv.msb = msb_;
			vv.initial_toggle_stat();
			vcd_vars_[ref] = vv;
		}
	} 
	else 
	{
		cout << "Unrecognized $var format" << endl;
		exit( 1 );
    }
}

void Module::vcd_parse_def_scope( FILE* vcd) 
{
	char type[256];     /* Scope type                 */
	char id[256];       /* Name of scope to change to */
	
	if( fscanf( vcd, "%s %s $end", type, id ) == 2 ) 
	{
		/* Make sure that we have not exceeded any array boundaries */
		assert( strlen( type ) <= 256 );
		assert( strlen( id )   <= 256 );
		if((strcmp(type, "module") == 0) && (strcmp(name_, id) == 0))    
		{
			enter_scope = true;
			found_in_vcd = true;
		}
	} 
	else 
	{
		cerr << "Unrecognized $scope format" << endl;
		exit( 1 );
	}
}

void Module::vcd_parse_def( FILE* vcd ) 
{
	bool enddef_found = false;  /* If set to true, definition section is finished */
	char keyword[256];          /* Holds keyword value                            */
	int  chars_read;            /* Number of characters scanned in                */
	
	while( !enddef_found && (fscanf( vcd, "%s%n", keyword, &chars_read ) == 1) ) {
		
		assert( chars_read <= 256 );
		
		if( keyword[0] == '$' ) 
		{
			
			if( strncmp( "var", (keyword + 1), 3 ) == 0 ) 
			{
				if(enter_scope)
					vcd_parse_def_var( vcd );
			} 
			else if( strncmp( "scope", (keyword + 1), 5 ) == 0 ) 
			{
				vcd_parse_def_scope( vcd );
				if(enter_scope)
					scope_count = scope_count + 1;
			} 
			else if( strncmp( "upscope", (keyword + 1), 7 ) == 0 ) 
			{
				if(enter_scope)
					scope_count = scope_count - 1;
				if(scope_count < 0)
					enter_scope = false;
				vcd_parse_def_ignore( vcd );
			} 
			else if( strncmp( "enddefinitions", (keyword + 1), 14 ) == 0 )
			{
				enddef_found = true;
				vcd_parse_def_ignore( vcd );
			} 
			else 
			{
				vcd_parse_def_ignore( vcd );
			}
		} 
		else 
		{
			cerr << "Non-keyword located where one should have been " << keyword << endl;
			exit( 1 );
		}  
	}
	
	assert( enddef_found );
	
	/* Check to see that at least one instance was found */
	if( !found_in_vcd) 
	{
		cerr << "No module were found in specified VCD file that matched design!" << endl;
		exit( 1 );
	}
}

void Module::vcd_parse_sim( Design* des, FILE* vcd, ostream& report, int type)
{
	char token[4100];         /* Current token from VCD file       */
	int  chars_read;          /* Number of characters scanned in   */
	
	//cur_sim_time = -1;
	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ) 
	{
		assert( chars_read <= 4100 );
		if( token[0] == '$' ) 
		{
			/* Currently ignore all simulation keywords */
		} 
		else if( (token[0] == 'b') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_vector( vcd, (token + 1) );
		} 
		else if( (token[0] == 'r') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_ignore( vcd );
		} 
		else if( token[0] == '#' ) 
		{
			if( cur_sim_time >= 0 ) {
				sim_timestep(des, report, type);
				for(map<string, vcd_var>::iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
					vpos->second.pre_val_ = vpos->second.cur_val_;
			}
			cur_sim_time = atol( token + 1 );
		} 
		else if( (token[0] == '0') ||
			(token[0] == '1') ||
			(token[0] == 'x') ||
			(token[0] == 'X') ||
			(token[0] == 'z') ||
			(token[0] == 'Z') ) 
		{
			set_symbol_char( token + 1, token[0] );
		} 
		else 
		{
			cerr << "Badly placed token " << token << endl;
			exit( 1 );
		}
	}
}


void Module::vcd_parse_sim_ignore( FILE* vcd ) 
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) != 1 ) 
	{
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
	assert( chars_read <= 256 );
}

void Module::vcd_parse_sim_vector( FILE* vcd, char* value )
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) == 1 ) {
		assert( chars_read <= 256 );
		//find the symbol in vcd_vars_ table and set the value;
		map<string, vcd_id>::iterator pos = vcd_ids_.find(sym);
		if(pos != vcd_ids_.end())
		{
			//first we find the variable in vcd_vars
			map<string, vcd_var>::iterator vpos = vcd_vars_.find(pos->second.name_);
			if(vpos != vcd_vars_.end())
			{
				//vpos->second.pre_val_ = vpos->second.cur_val_;
				assert(strlen(value) <= vpos->second.width);
				unsigned index, locate;
				if(vpos->second.little_endia)
					index = pos->second.lsb;
				else
					index = pos->second.msb;
				for(unsigned idx = 0; idx < strlen(value); ++idx)
				{
					if(vpos->second.little_endia)
						locate = index - vpos->second.lsb;
					else
						locate = vpos->second.lsb - index;
					switch(value[idx])
					{
					case '0' :
						vpos->second.cur_val_.set(locate, verinum::V0);
						break;
					case '1':
						vpos->second.cur_val_.set(locate, verinum::V1);
						break;
					case 'x':
					case 'X':
						vpos->second.cur_val_.set(locate, verinum::Vx);
						break;
					case 'z':
					case 'Z':
						vpos->second.cur_val_.set(locate, verinum::Vz);
						break;
					}
					index = index + 1;
				}
			}
			else
			{
			}
		}
		else
		{
		}
	} 
	else 
	{
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
}

void Module::set_symbol_char( char* sym, char value ) 
{
	map<string, vcd_id>::iterator pos = vcd_ids_.find(sym);
	if(pos != vcd_ids_.end())
	{
		map<string, vcd_var>::iterator vpos = vcd_vars_.find(pos->second.name_);
		if(vpos != vcd_vars_.end())
		{
			assert(1 <= vpos->second.width);
			//vpos->second.pre_val_ = vpos->second.cur_val_;
			unsigned index, locate;
			if(vpos->second.little_endia)
				index = pos->second.lsb;
			else
				index = pos->second.msb;

			if(vpos->second.little_endia)
				locate = index - vpos->second.lsb;
			else
				locate = vpos->second.lsb - index;

			switch(value)
			{
			case '0' :
				vpos->second.cur_val_.set(locate, verinum::V0);
				break;
			case '1':
				vpos->second.cur_val_.set(locate, verinum::V1);
				break;
			case 'x':
			case 'X':
				vpos->second.cur_val_.set(locate, verinum::Vx);
				break;
			case 'z':
			case 'Z':
				vpos->second.cur_val_.set(locate, verinum::Vz);
				break;
			}
		}
	}
}

void Module::sim_timestep(Design* des, ostream& report, int type)
{
	bool found = false;
	set<unsigned, less<unsigned> > path;
	//for each process, iterator over the vcd_vars_ table find the value change
	//signals, if find one, simulate the process and report result
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		path.clear();
		for(set<string>::const_iterator spos = (*(cfg_->cfgs))[idx]->refs.begin(); spos != (*(cfg_->cfgs))[idx]->refs.end(); ++spos)
		{
			for(map<string, vcd_var>::const_iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
			{
				if((*spos == vpos->second.name_) && (vpos->second.cur_val_ != vpos->second.pre_val_))
				{
					//do simulation
					unsigned index = 1;
					while((index > 0) && (index <= (*(cfg_->cfgs))[idx]->root->count() - 2))
					{
						//jump over start @ statement
						if((*(*(cfg_->cfgs))[idx]->root)[index]->type == "ISCONTROL.EVENT")
						{
							index = index + 1;
							continue;
						}
						//jump over 
						if((*(*(cfg_->cfgs))[idx]->root)[index]->type.find("ISCONTROL") != 0)
						{
							path.insert((*(*(cfg_->cfgs))[idx]->root)[index]->lineno);
							index = index + 1;
							continue;
						}
						//for condition node
						path.insert((*(*(cfg_->cfgs))[idx]->root)[index]->lineno);
						//evaluate the condition expression and get the direction of the condition evaluate
						index = eval_cond_expr(des, (*(*(cfg_->cfgs))[idx]->root)[index]);
					}
					found = true;
					break;
				}
			}
			if(found) break;
		}
		if(type == 0) //paths coverage
		{
			map<unsigned, path_stat>::iterator mpos = paths.find((*(cfg_->cfgs))[idx]->lineno);
			if(mpos != paths.end())
			{
				mpos->second.add_path(path);
			}
			else
			{
				path_stat ps;
				ps.add_path(path);
				paths[(*(cfg_->cfgs))[idx]->lineno] = ps;
			}
		}
		if(type == 1) //statements coverage
		{
			map<unsigned, stmt_stat>::iterator mpos = statements.find((*(cfg_->cfgs))[idx]->lineno);
			if(mpos != statements.end())
			{
				mpos->second.add_stmt(path);
			}
			else
			{
				stmt_stat ss;
				ss.add_stmt(path);
				statements[(*(cfg_->cfgs))[idx]->lineno] = ss;
			}
		}

	}
	for(list<PGate*>::iterator gpos = gates_.begin(); gpos != gates_.end(); ++gpos)
	{
		if(((*gpos)->ty == PGate::ASSIGN) || ((*gpos)->ty == PGate::BUILDIN))
		{
			//First, we find the CfgNode of the assign or gate instantiation statement
			int i = mn_->find_node((*gpos)->get_lineno(),3);
			set<string> refs = mn_->get_assignnode(i)->get_refs();
			bool changed = false;
			for(set<string>::const_iterator refpos = refs.begin(); refpos != refs.end(); ++refpos)
			{
				for(map<string, vcd_var>::const_iterator vvpos = vcd_vars_.begin(); vvpos != vcd_vars_.end(); ++vvpos)
				{
					if((*refpos == vvpos->second.name_) && (vvpos->second.cur_val_ != vvpos->second.pre_val_))
					{
						changed = true;
						if((*gpos)->ty == PGate::ASSIGN)
						report << "Assign Statement at : " << (*gpos)->get_line() << " is covered" << endl;
						break;
					}
				}
				if(changed) break;
			}
		}
	}
}

int Module::eval_cond_expr(Design* des, Cfg_Node* node)
{
	int index;
	hname_t name(name_);
	NetScope*scope = des->find_scope(name); 
	verinum* vn = node->expr[0]->evaluate(des, scope, vcd_vars_);
	if(node->type.find("CASE") == string::npos) //not case statement
	{
		if(vn->len() > 1)
		{
			verinum::V v = unary_or(*vn);
			if(v == verinum::V0)
				return node->dsuc[1]->index;
			if(v == verinum::V1)
				return node->dsuc[0]->index;
		}
		else
		{
			if(vn[0] == verinum::V0)
				return node->dsuc[1]->index;
			if(vn[0] == verinum::V1)
				return node->dsuc[0]->index;
		}
	}
	else //case statement
	{
		for(unsigned idx = 0; idx < node->dsuc.count(); ++idx)
		{
			for(set<PExpr*>::const_iterator pos = node->dsuc[idx]->caseitem.begin(); pos != node->dsuc[idx]->caseitem.end(); ++pos)
			{
				verinum* ci = (*pos)->evaluate(des, scope, vcd_vars_);
				if(node->type == "ISCONTROL.CASE")
				{
					if(*vn == *ci)
						return node->dsuc[idx]->index;
				}
				if((node->type == "ISCONTROL.CASEX") || (node->type == "ISCONTROL.CASEZ"))
				{
					if(ceq(*vn, *ci))
						return node->dsuc[idx]->index;
				}
			}
		}
	}
	//return index;
}

void Module::find_all_path(const char* path_file)
{
	string path(path_file);
	if(path[path.length() - 1] != '\\') 
		path = path + "\\";// + "\\";
	path = path + name_ + "\\";// + "\\";


	//CreateDirectory(path.c_str(), NULL);

	if (!CreateDirectory(path.c_str(), NULL)) 
	{
		cerr << "Couldn't create new directory : " << path.c_str() << endl;; 
		return;
	}

	char tmp[4];
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		sprintf(tmp, "%d", (*(cfg_->cfgs))[idx]->id);
		//tmp[strlen(tmp) - 1] = '\0';
		/*char* filename = strdup(path.c_str());
		strcat(filename, tmp);*/
		string filename;
		filename.assign(path);
		filename = filename + "P" + tmp + ".txt";
		ofstream file(filename.c_str(), ios::out|ios::trunc);
		cerr << filename << endl;
		cout << "Process : " << (*(cfg_->cfgs))[idx]->id << endl;
		bool find = false;
		//unsigned index;
		map<int, string> pathes;
		
		//－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
		//(*(*(cfg_->cfgs))[idx]->root)[1]) 为指向头接点的指针
		Cfg_Node * n1;
		Route_Node rn;
		Route_Nodes rns;
		
		//int i;
		
		if((*(*(cfg_->cfgs))[idx]->root)[1]->type == "ISCONTROL.EVENT")
			n1 = (*(*(cfg_->cfgs))[idx]->root)[2];
		else
			n1 = (*(*(cfg_->cfgs))[idx]->root)[1];
		
		map<unsigned, int>::const_iterator pos = (*(cfg_->cfgs))[idx]->lineno_index.find(n1->lineno);
		rn.index = pos->second;
		rn.lineno = n1->lineno;
		rn.succ_no = n1->dsuc.count();
		rn.pass_no = 0;
		rn.cfg_n = n1;
		
		rns.push(rn);

		unsigned long pathidx = 1;
		string outpath;

		while(1)
		{
			//map<unsigned, int>::const_iterator pos2 = (*(cfg_->cfgs))[idx]->lineno_index.find(n1->lineno);
			do
			{
				if(!rns.pop(rn))	//如果堆栈中没有元素则跳出循环！路径寻找完毕
					goto fin;
			}
			while(rn.pass_no >= rn.succ_no);
			
			n1 = (*(*(cfg_->cfgs))[idx]->root)[rn.cfg_n->dsuc[rn.pass_no]->index];
			//下一个没有访问过的节点
			rn.pass_no++;
			rns.push(rn);
			
rep:
			
			if(n1->type == "ENDNODE")	//找到一条路径
			{
				rns.output(file, pathidx, outpath);
				pathes[pathidx] = outpath;
				pathidx = pathidx + 1;
				continue;
			}
			
			if(rns.node_exist(n1->lineno))
			{
				//	if (rns.get_node_pass(n1->lineno) < 2)
				//rns.set_node_pass_no(n1->lineno,1);
				rn.pass_no = rns.get_node_pass(n1->lineno) + 1;
			}
			else
				rn.pass_no = 1;
			map<unsigned, int>::const_iterator pos2 = (*(cfg_->cfgs))[idx]->lineno_index.find(n1->lineno);
			rn.index = pos2->second;
			rn.lineno = n1->lineno;
			rn.succ_no = n1->dsuc.count();
			rn.cfg_n = n1;
			
			if(rns.node_exist(n1->lineno))
				n1 = (*(*(cfg_->cfgs))[idx]->root)[rn.cfg_n->dsuc[(rn.pass_no - 1) % 2]->index];	
			else
				n1 = (*(*(cfg_->cfgs))[idx]->root)[rn.cfg_n->dsuc[rn.pass_no - 1]->index];
			
			rns.push(rn);
			
			goto rep;
		}

fin:
		file.close();//break;
		procpathes[(*(cfg_->cfgs))[idx]->id] = pathes;
		//－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
		//break;
	}
}

map<int, map<int, string> > Module::getpropathes()
{
	return procpathes;
}

void Module::dumpprocpathes(ostream& out)
{
	map<int, map<int, string> >::const_iterator ppos;
	map<int, string>::const_iterator pos;
	for(ppos = procpathes.begin(); ppos != procpathes.end(); ++ppos)
	{
        out << "P" << ppos->first <<": " << endl;
		for(pos = ppos->second.begin(); pos != ppos->second.end(); ++pos)
		{
			out << pos->first << ": " << pos->second << endl;
		}
	}
}

void Module::dumpprocpath(ostream&out, int procid, int pathid)
{
	map<int, string> pos;
	pos=procpathes[procid];
	out << "P" << procid << ": " << pathid << ": " + pos[pathid] << endl;
}

void Module::genpathcom(ostream&out, int simucycle)
{
	map<int, map<int, string> >::const_iterator ppos;
	map<int, string> pos;

	out<< "Cycle: "<< simucycle <<endl;

	int random;
	//int count;
	//srand( (unsigned)time( NULL ) );

	for(ppos = procpathes.begin(); ppos != procpathes.end(); ++ppos)
	{
		random=rand()%ppos->second.size()+1;
		pos = ppos->second;
		out<< name_ << ": " << ppos->first << " : " << pos[random] << endl;
	}
}