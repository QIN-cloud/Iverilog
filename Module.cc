/*
 * Copyright (c) 1998-2019 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

# include "config.h"

# include  "Module.h"
# include  "PGate.h"
# include  "PWire.h"
# include  "PExpr.h"
# include  <cassert>
# include  "string.h"
# include  <stack>
# include  <unordered_map>


using namespace std;

list<Module::named_expr_t> Module::user_defparms;

/* n is a permallocated string. */
Module::Module(LexicalScope*parent, perm_string n)
: PScopeExtra(n, parent)
{
      library_flag = false;
      is_cell = false;
      is_interface = false;
      program_block = false;
      uc_drive = UCD_NONE;
}

Module::~Module()
{
	for(VcdScope* vcd_scope : vcd_scopes)
	{
		delete vcd_scope;
	}
	for(pair<Cfg_Node*, PathNode*> pn : path_nodes_)
	{
		delete pn.second;
	}
}

void Module::add_gate(PGate*gate)
{
      gates_.push_back(gate);
}

unsigned Module::port_count() const
{
      return ports.size();
}

/*
 * Return the array of PEIdent object that are at this port of the
 * module. If the port is internally unconnected, return an empty
 * array.
 */
const vector<PEIdent*>& Module::get_port(unsigned idx) const
{
      assert(idx < ports.size());
      static const vector<PEIdent*> zero;

      if (ports[idx])
	    return ports[idx]->expr;
      else
	    return zero;
}

unsigned Module::find_port(const char*name) const
{
      assert(name != 0);
      for (unsigned idx = 0 ;  idx < ports.size() ;  idx += 1) {
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

      return ports.size();
}

perm_string Module::get_port_name(unsigned idx) const
{

      assert(idx < ports.size());
      if (ports[idx] == 0 || ports[idx]->name.str() == 0) {
              /* It is possible to have undeclared ports. These
                 are ports that are skipped in the declaration,
                 for example like so: module foo(x ,, y); The
                 port between x and y is unnamed and thus
                 inaccessible to binding by name. Port references
		 that aren't simple or escaped identifiers are
		 also inaccessible to binding by name. */
            return perm_string::literal("unnamed");
      }
      return ports[idx]->name;
}

PTask* Module::get_task(const string&name)
{
      const char* tmpname = name.c_str();
      perm_string tmp(tmpname);
	map<perm_string,PTask*>::const_iterator obj = tasks.find(tmp);
	if(obj==tasks.end())
		return 0;
	else
		return (*obj).second;
}

PTask* Module::get_task(perm_string name)
{
	map<perm_string,PTask*>::const_iterator obj = tasks.find(name);
	if(obj==tasks.end())
		return 0;
	else
		return (*obj).second;
}

PGate* Module::get_gate(perm_string name)
{
      for (list<PGate*>::iterator cur = gates_.begin()
		 ; cur != gates_.end() ; ++ cur ) {

	    if ((*cur)->get_name() == name)
		  return *cur;
      }

      return 0;
}

PWire* Module::get_wire(perm_string name)
{
      map<perm_string,PWire*>::const_iterator obj = wires.find(name);
      if (obj == wires.end())
	    return 0;
      else
	    return (*obj).second;
}

const map<perm_string, PWire*> Module::get_wires() const
{
      return wires;
}

const list<PGate*>& Module::get_gates() const
{
      return gates_;
}

PNamedItem::SymbolType Module::symbol_type() const
{
      if (program_block)
            return PROGRAM;
      if (is_interface)
            return INTERFACE;

      return MODULE;
}

void Module::build_cfgs()
{
      cfg_ = mn_->build_cfgs();
}

void Module::build_var_cfgs()
{
	svector<Cfg*>* cfgs = cfg_->cfgs;
	for(int i = 0; i < (*cfgs).count(); i++){
		Cfg* cfg = (*cfgs)[i];
		for(int j = 0; j < cfg->root->count(); j++){
			Cfg_Node* cn = (*(cfg->root))[j];
			if(cn->type.find("ISCONTROL") == 0){
				for(string ref : cn->refs){
					var_cfgs[ref].insert(i);
				}
			}
		}

	}
}

void Module::dump_cfg(ostream& o) const
{
	o << "module " << pscope_name() << endl;
	o << "{" << endl; 
	for(unsigned y = 0; y < cfg_->cfgs->count(); ++y){
		o << "Process ID: " << (*(cfg_->cfgs))[y]->id << endl;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[y]->root->count(); ++z){
			o << "Index = " << z <<", ";
			o << *(*((*(cfg_->cfgs))[y]->root))[z];
			o << endl;
		}
	}
	o << "}" << endl;
}


void Module::dump_vars(ostream& o) const
{
	/*Dump condit variables and processes they located in.*/
	o << "------------------var cfgs------------------"<< endl;
	map<string, set<unsigned>>::const_iterator var_cfg;
	for(var_cfg = var_cfgs.begin(); var_cfg != var_cfgs.end(); var_cfg++){
		o << "var " << var_cfg->first << " ";
		for(unsigned id : var_cfg->second){
			o << "{" << id << "}" << " ";
		}
		o << endl;
	}
	/*Dump the instantiated modules by this module type.*/
	o << "----------------module scopes---------------"<< endl;
	for(VcdScope* scope : vcd_scopes){
		o << "scope " << scope->name_ << endl;
		o << "(" << endl;
		map<string, VcdVar*>::iterator var;
		for(var = scope->vars_.begin(); var != scope->vars_.end(); var++){
			o << "var " << var->second->name << "[" << var->second->msb << ":" << var->second->lsb << "]" << " symbol " << var->first << endl;
		} 
		o << ")" << endl;
	}
}

void Module::dump_values(ostream& o) const
{
	for(VcdScope* scope : vcd_scopes){
		scope->dump(o);
	}
}

void Module::dump_sort_cfg(ostream& o) const
{
	o << "-----------synchorous process-----------" << endl;
	for(Cfg* cfg : sync_cfgs_){
		o << "ID" << " " << cfg->id << " " << "line" << " " << cfg->lineno << endl;
		o << "defs" << endl;
		o << "{";
		for(string def : cfg->defs){
			o << " " << def;
		}
		o << " " << "}" << endl;
		o << "refs" << endl;
		o << "{";
		for(string ref : cfg->refs){
			o << " " << ref;
		}
		o << " " << "}" << endl << endl;	
	}
	o << "-----------combination process-----------" << endl;
	for(Cfg* cfg : combine_cfgs_){
		o << "ID" << " " << cfg->id << " " << "line" << " " << cfg->lineno << endl;
		o << "defs" << endl;
		o << "{";
		for(string def : cfg->defs){
			o << " " << def;
		}
		o << " " << "}" << endl;
		o << "refs" << endl;
		o << "{";
		for(string ref : cfg->refs){
			o << " " << ref;
		}
		o << " " << "}" << endl << endl;	
	}
}

void Module::dump_sort_assign(ostream& o) const
{
	for(PGAssign* assign : assigns_){
		assign->dump(o);
	}
}

void Module::build_vartab(Design* d)
{
	assert(d);
	NetScope* scope;
	perm_string s = pscope_name();
	list<NetScope*> scopes = d->find_root_scopes();

	for(list<NetScope*>::const_iterator pos = scopes.begin();
	pos != scopes.end(); ++pos){
		if(s == (*pos)->basename()){
			scope = *pos;
			break;
		}
	}

	for(map<perm_string,PWire*>::const_iterator spos = wires.begin(); spos != wires.end(); ++spos){
		perm_string o = spos->first;
		RefVar* var = scope->build_var(o);
		if(var) vartab_[var->name] = var;
	}
}

PathNode::~PathNode()
{

}

VcdScope::~VcdScope()
{
	map<string, VcdVar*>::iterator var;
	for(var = vars_.begin(); var != vars_.end(); var++){
		delete var->second;
	}
	vars_.clear();
}

void VcdScope::dump(ostream& o) const
{
	map<string, VcdVar*>::const_iterator var;
	o << module_->pscope_name() << " " << name_ << endl;
	o << "{" << endl;
	for(var = vars_.begin(); var != vars_.end(); var++){
		o << var->first << " ";
		var->second->dump(o);
	}
	o << "}" << endl;
}

void VcdScope::initialize(map<string, verinum>& variety_symbols, ostream& o)
{
	toggle_ = false;
	map<string, VcdVar*>::iterator var = vars_.begin();
	for(; var != vars_.end(); var++)
	{
		if(variety_symbols.find(var->first) == variety_symbols.end())
			var->second->sim_val = var->second->cur_val;
		else
		{
			string ptype = module_->vartab_[var->second->name]->ptype;
			var->second->sim_val = ptype == "PINPUT" ? var->second->cur_val : var->second->pre_val; 
		}
	}
	set<string> defs;
	module_->assign_evaluate(defs, this, true, o);
	o << "$Initial Values$" << endl;
	o << "{" << endl;
	for(var = vars_.begin(); var != vars_.end(); var++)
	{
		o << "\t";
		o << var->second->name << " = ";
		var->second->sim_val.dump(o);
		o << endl; 
	}
	o << "}" << endl;
}

void VcdScope::update(set<string>& defs, ostream& o)
{
	o << "$Direct Influence$ ";
	for(string def : defs)
	{
		o << def << " ";
	}
	o << endl;
	o << "{" << endl;
	map<string, VcdVar*>::iterator var = vars_.begin();
	for(; var != vars_.end(); var++)
	{
		if(defs.find(var->second->name) != defs.end())
		{
			var->second->sim_val = var->second->cur_val;
			o << "\t";
			o << var->second->name << " -> ";
			var->second->sim_val.dump(o);
			o << endl;
		}
	}
	o << "}" << endl;
	module_->assign_evaluate(defs, this, false, o);
}

AssignNode::~AssignNode()
{

}

void AssignNode::dump(ostream& o)
{
	o << "{" << endl;
	o << name_ << " indegree : " << in_ << " outdegree : " << out_ << endl;
	if(assign_) assign_->dump(cout);
	o << "next : ";
	list<string>::iterator next = next_.begin();
	for(next; next != next_.end(); next++){
		o << (*next) << " ";
	}
	o << endl;
	o << "}" << endl;
}

void Module::build_paths()
{
	//generate paths for every process
	for(unsigned i = 0; i < cfg_->cfgs->count(); i++){

		Cfg* cfg = (*(cfg_->cfgs))[i];

		//generate paths for a process
		PathNode* pn = gen_path((*(cfg->root))[0], cfg->root, 0);

		//store the vector and string of paths
		set<unsigned> route;
		for(string s : pn->paths_){
			if(s.size() == 1) continue;
			paths_[i].push_back(s);
			unsigned num = 0;
			for(unsigned j = 0; j < s.size(); j++){
				if(s[j] == ' '){
					if(num){
						route.insert((*(cfg->root))[num]->lineno);
						num = 0;
					}
				}
				else{
					num = num * 10 + (s[j] - '0');
				}
			}
			if(num) route.insert((*(cfg->root))[num]->lineno);
			routes_[cfg].push_back(route);
			route.clear();
		}
	}
}

PathNode* Module::gen_path(Cfg_Node* node, svector<Cfg_Node*>* root, unsigned index)
{
	if(!path_nodes_[node])
	{
		PathNode* pn = new PathNode(node);
		if(node->type != "ENDNODE"){
			for(unsigned i = 0; i < node->dsuc.count(); i++){
				PathNode* son_node = gen_path((*root)[node->dsuc[i]->index], root, node->dsuc[i]->index);
				if(node->type != "STARTNODE"){
					if(son_node->paths_.empty()) 
						pn->paths_.push_back(to_string(index));
					else{
						for(string s : son_node->paths_){
							pn->paths_.push_back(to_string(index) + " " + s);
						}
					}
				}
				else{
					for(string s : son_node->paths_){
						pn->paths_.push_back(s);
					}
				}
			}
		}
		path_nodes_[node] = pn;
	}
	return path_nodes_[node];
}

void Module::dump_paths(ostream& o) const
{
	o << pscope_name() << endl;
	map<Cfg*, vector<set<unsigned> > >::const_iterator pos;
	for(pos = routes_.begin(); pos != routes_.end(); pos++){
		o << "Process ID : " << pos->first->id << endl;
		for(set<unsigned> route : pos->second){
			for(unsigned lineno : route){
				o << lineno << " ";
			}
			o << endl;
		}
	}
}

void Module::parse_assigns()
{
	//Build the Directed Graph by assign statements.
	list<PGate*>::iterator gate = gates_.begin();
	for(gate; gate != gates_.end(); gate++)
	{
		if(PGAssign* assign = dynamic_cast<PGAssign*>(*gate))
		{
			//Parsing the string names for every variable in assign statement.
			set<string> lnames = assign->pin(0)->get_var_names();
			assert(lnames.size() == 1);
			set<string> rnames = assign->pin(1)->get_var_names();

			//Build node for lref.
			string lname = *(lnames.begin());
			if(assign_pos_.find(lname) == assign_pos_.end())
			{
				AssignNode* node = new AssignNode(lname);
				assign_pos_[lname] = node;
			}
			AssignNode* l_node = assign_pos_[lname];
			l_node->in_ += rnames.size();
			l_node->assign_= assign;

			//Build node for rref.
			for(string rname : rnames)
			{
				if(assign_pos_.find(rname) == assign_pos_.end())
				{
					AssignNode* node = new AssignNode(rname);
					assign_pos_[rname] = node;
				}
				AssignNode* r_node = assign_pos_[rname];
				r_node->out_ += 1;
				r_node->next_.push_back(lname);
			}
		}
	}
}

void Module::sort_assigns()
{
	//Build a unordered map for sorting the assign statements.
	unordered_map<string, AssignNode*> node_map;
	map<string, AssignNode*>::iterator pos = assign_pos_.begin();
	for(pos; pos != assign_pos_.end(); pos++)
	{
		node_map[pos->first] = pos->second;
	}
	//Get the 0 indegree node and delete it's link until map is empty.
	while(!node_map.empty())
	{
		unordered_map<string, AssignNode*>::iterator m_pos = node_map.begin();
		for(m_pos; m_pos != node_map.end(); m_pos++)
		{
			if(m_pos->second->in_ == DELETE_INDEGREE)
			{
				if(m_pos->second->assign_)
				{
					assigns_.push_back(m_pos->second->assign_);
				}
				for(string next : m_pos->second->next_)
				{
					assign_pos_[next]->in_ -= 1;
				}
				node_map.erase(m_pos->first);
				break;
			}
		}
	}
}

void Module::parse_wires()
{
	map<string, AssignNode*>::iterator node = assign_pos_.begin();
	for(node; node != assign_pos_.end(); node++){
		if(vartab_.find(node->first) != vartab_.end()){
			if(node->second->assign_){
				vartab_[node->first]->ptype = "Assign";
			}
		}
	}
}

void Module::find_assign(string var, map<PGAssign*, string>& assigns)
{
	if(assign_pos_[var]->assign_){
		assigns[assign_pos_[var]->assign_] = var;
	}

	list<string>::iterator next = assign_pos_[var]->next_.begin();
	for(next; next != assign_pos_[var]->next_.end(); next++){
		find_assign(*next, assigns);
	}
}

void Module::gen_assign_smt(set<string>& refs, bool type, ostream& o, map<string, RefVar*>& vars, set<SmtVar*>& used, unsigned cur_time)
{
	map<PGAssign*, string> select_assigns;
	if(!refs.empty()){
		for(string var : refs){
			if(assign_pos_.find(var) != assign_pos_.end()){
				find_assign(var, select_assigns);
			}
		}
	}

	cout << ">>>assigns" << endl;

	list<PGAssign*>::iterator assign = assigns_.begin();
	for(assign; assign != assigns_.end(); assign++){
		if(type || select_assigns.find(*assign) != select_assigns.end()){
			(*assign)->dump_smt(design_, o, vars, used, this, cur_time);
			(*assign)->dump(cout);
		}
	}

	if(!type){
		map<PGAssign*, string>::iterator pos = select_assigns.begin();
		for(pos; pos != select_assigns.end(); pos++){
			refs.insert(pos->second);
		}
	}

	if(!select_assigns.empty())
	{
		o << endl;
	}
}

void Module::sort_cfgs()
{
	unordered_map<Cfg*, unordered_map<string, bool> > cfg_used;
	set<pair<Cfg*, Cfg*> > compares;
	vector<Cfg*> cfgs;

	//Complete defs with assigns for every cfg.
	for(int i = 0; i < (*(cfg_->cfgs)).count(); i++)
	{
		Cfg* cfg = (*(cfg_->cfgs))[i];

		if(!cfg->always)
			continue;

		if(cfg->sync)
			sync_cfgs_.push_back(cfg);

		else
		{
			complete_defs(cfg, cfg_used);
			cfgs.push_back(cfg);
		}
	}

	if(cfgs.size() == 1)
	{
		combine_cfgs_.push_back(cfgs.front());
		return;
	}

	//Compare the excuted order of cfgs.
	for(int i = 0; i < cfgs.size(); i++)
	{
		for(int j = i + 1; j < cfgs.size(); j++)
		{
			if(Cfg* pre_cfg = compare_cfgs(cfgs[i], cfgs[j], cfg_used))
			{
				Cfg* next_cfg = pre_cfg == cfgs[i] ? cfgs[j] : cfgs[i];
				compares.insert(make_pair(pre_cfg, next_cfg));
			}
		}
	}

	//Build a Direct Graph by cfg orders.
	unordered_map<Cfg*, CfgTPNode*> d_graph;

	for(pair<Cfg*, Cfg*> cp : compares)
	{
		CfgTPNode* pre_nd;
		CfgTPNode* next_nd;

		if(d_graph.find(cp.first) == d_graph.end())
		{
			pre_nd = new CfgTPNode(cp.first);
			d_graph[cp.first] = pre_nd;
		}

		if(d_graph.find(cp.second) == d_graph.end())
		{
			next_nd = new CfgTPNode(cp.second);
			d_graph[cp.second] = next_nd;
		}

		pre_nd = d_graph[cp.first];
		next_nd = d_graph[cp.second];

		pre_nd->next_.push_back(cp.second);
		pre_nd->out_ += 1;
		next_nd->in_ += 1;
	}

	//Top-logic sort by Direct Graph.
	unordered_map<Cfg*, bool> delete_nodes;
	while(delete_nodes.size() != d_graph.size())
	{
		unordered_map<Cfg*, CfgTPNode*>::iterator pos = d_graph.begin();
		for(pos; pos != d_graph.end(); pos++)
		{
			if(delete_nodes.find(pos->first) == delete_nodes.end())
			{
				if(pos->second->in_ == DELETE_INDEGREE)
				{
					list<Cfg*>::iterator direct_pos = pos->second->next_.begin();
					for(direct_pos; direct_pos != pos->second->next_.end(); direct_pos++)
					{
						d_graph[(*direct_pos)]->in_ -= 1;
						pos->second->out_ -= 1;
					}
					delete_nodes[pos->first] = true;
					combine_cfgs_.push_back(pos->first);
				}
			}
		}
	}

}

void Module::complete_defs(Cfg* cfg, unordered_map<Cfg*, unordered_map<string, bool> >& used)
{
	map<PGAssign*, string> select_assigns;
	set<string> refs = cfg->refs;
	set<string> defs = cfg->defs;

	if(!defs.empty()){
		for(string var : defs){
			if(assign_pos_.find(var) != assign_pos_.end()){
				find_assign(var, select_assigns);
			}
		}
	}

	map<PGAssign*, string>::iterator pos = select_assigns.begin();
	for(pos; pos != select_assigns.end(); pos++){
		defs.insert(pos->second);
	}

	for(string ref : refs)
	{
		used[cfg][ref] = CFG_REF;
	}

	for(string def : defs)
	{
		used[cfg][def] = CFG_DEF;
	}

}

Cfg* Module::compare_cfgs(Cfg* lcfg, Cfg* rcfg, unordered_map<Cfg*, unordered_map<string, bool> >& used)
{
	unordered_map<string, bool>::iterator pos;

	for(pos = used[lcfg].begin(); pos != used[lcfg].end(); pos++)
	{
		if(used[lcfg][pos->first] == CFG_DEF)
		{
			if(used[rcfg].find(pos->first) != used[rcfg].end())
			{
				if(used[rcfg][pos->first] == CFG_REF)
					return lcfg;
			}
		}
	}

	for(pos = used[rcfg].begin(); pos != used[rcfg].end(); pos++)
	{
		if(used[rcfg][pos->first] == CFG_DEF)
		{
			if(used[lcfg].find(pos->first) != used[lcfg].end())
			{
				if(used[lcfg][pos->first] == CFG_REF)
					return rcfg;
			}
		}
	}

	return lcfg;
}

void Module::dump_vartab(ostream& o) const
{
	map<string, RefVar*>::const_iterator var = vartab_.begin();
	for(var; var != vartab_.end(); var++)
	{
		var->second->dump(o);
	}
}

void Module::initialize()
{
	build_vartab(design_);

	parse_assigns();

	sort_assigns();

	parse_wires();

	sort_cfgs();
}

void Module::enumrate(ostream& enums, ostream& paths, ostream& report)
{
	for(map<Cfg*, vector<set<unsigned> > >::iterator pos = routes_.begin(); pos != routes_.end(); pos++)
	{
		paths << "id: " << pos->first->id << endl;
		for(unsigned i = 0; i < pos->second.size(); i++)
		{
			paths << "path" << i << " : ";
			for(unsigned lineno : pos->second[i])
			{
				paths << lineno << " ";
			}
			paths << endl;
		}
	}
	
	list<unsigned> path;
	dfs_paths(enums, path, 0);

	for(map<unsigned, vector<string> >::iterator pos = paths_.begin(); pos != paths_.end(); pos++)
	{
		if(!pos->second.empty())
		{
			report << "ID " << pos->first << endl;
			for(unsigned i = 0; i < pos->second.size(); i++)
			{
				report << "0" << " ";
			}
			report << "end" << endl;
		}
	}
}

void Module::dfs_paths(ostream& o, list<unsigned>& path, unsigned index)
{
	if(index < paths_.size())
	{
		for(unsigned i = 0; i < paths_[index].size(); i++)
		{
			path.push_back(i);
			dfs_paths(o, path, index + 1);
			if(index == (paths_.size() - 1))
			{
				o << "path ";
				for(unsigned id : path)
				{
					o << id << " ";
				}
				o << "end" << endl;
			}
			path.pop_back();
		}
	}
}

void Module::tran_nodes()
{
	map<unsigned, vector<string> >::iterator pos;
	for(pos = paths_.begin(); pos != paths_.end(); pos++)
	{
		for(string s : pos->second)
		{
			vector<unsigned> temp = split_path(s);
			if(nodes_.find(pos->first) == nodes_.end())
				nodes_[pos->first] = vector<vector<unsigned> >();
			nodes_[pos->first].push_back(temp);
		}
	}
}

vector<unsigned> split_path(string path)
{
	vector<unsigned> res;
	string num;
	for(char c : path)
	{
		if(c <= '9' && c >= '0')
			num = num + c;
		else
		{
			res.push_back(atoi(num.c_str()));
			num.clear();
		}
	}
	res.push_back(atoi(num.c_str()));
	return res;
}

void Module::assign_evaluate(set<string>& defs, VcdScope* scope, bool type, ostream& o)
{
	map<PGAssign*, string> select_assigns;
	if(!defs.empty())
	{
		for(string def : defs)
		{
			if(assign_pos_.find(def) != assign_pos_.end())
			{
				find_assign(def, select_assigns);
			}
		}
	}
	set<VcdVar*> vars;
	list<PGAssign*> assigns;
	list<PGAssign*>::iterator pos = assigns_.begin();
	for(; pos != assigns_.end(); pos++)
	{
		if(type || select_assigns.find(*pos) != select_assigns.end())
		{
			VcdVar* var = (*pos)->evaluate(design_, design_->find_scope(hname_t(pscope_name())), scope);
			if(!type)
			{
				vars.insert(var);
				assigns.push_back(*pos);
			}
		}
	}
	if(!assigns.empty())
	{
		o << "[Assigns Influence]" << endl;
		o << "{" << endl;
		for(PGAssign* assign : assigns)
		{
			assign->dump(o);
		}
		o << "}" << endl;
		o << "$Change Values$" << endl;
		o << "{" << endl;
		for(VcdVar* var : vars)
		{
			o << "\t" << var->name << " -> ";
			var->sim_val.dump(o);
			o << endl;
		}
		o << "}" << endl;
	}
	defs.clear();
	vars.clear();
	select_assigns.clear();
}



