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
# include <unordered_map>

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
		for(var = scope->vars.begin(); var != scope->vars.end(); var++){
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

void Module::build_vartab(Design* d)
{
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
	int count = 0;
	for(map<perm_string,PWire*>::const_iterator spos = wires.begin(); spos != wires.end(); ++spos){
		perm_string o = spos->first;
		RefVar* var = scope->build_var(o);
		if(var) vartab_.insert(var);
		count += 1;
	}
}

PathNode::~PathNode()
{

}

VcdScope::~VcdScope()
{
	map<string, VcdVar*>::iterator var;
	for(var = vars.begin(); var != vars.end(); var++){
		delete var->second;
	}
	vars.clear();
}

void VcdScope::dump(ostream& o) const
{
	map<string, VcdVar*>::const_iterator var;
	o << module_->pscope_name() << " " << name_ << endl;
	o << "{" << endl;
	for(var = vars.begin(); var != vars.end(); var++){
		o << var->first << " ";
		var->second->dump(o);
	}
	o << "}" << endl;
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

void Module::random_path(ostream& o, string seeds) const
{
	o << "cycle: " << 0 << endl;
	ifstream in(seeds.c_str());
	if(!in){
		cerr << seeds << "can't open!" << endl;
		exit(1);
	}
	unsigned seed;
	map<unsigned, vector<string> >::const_iterator pos;
	for(pos = paths_.begin(); pos != paths_.end(); pos++){
		in >> seed;
		o << pscope_name() << " " << pos->first << " " << pos->second[seed%(pos->second.size()-1)] << endl;
	}
	in.close();
}

void Module::parse_assigns()
{
	//Build the Directed Graph by assign statements.
	list<PGate*>::iterator gate = gates_.begin();
	for(gate; gate != gates_.end(); gate++){

		if(PGAssign* assign = dynamic_cast<PGAssign*>(*gate)){

			//Parsing the string names for every variable in assign statement.
			set<string> lnames = assign->pin(0)->get_var_names();
			assert(lnames.size() == 1);
			set<string> rnames = assign->pin(1)->get_var_names();

			//Build node for lref.
			string lname = *(lnames.begin());
			if(assign_pos_.find(lname) == assign_pos_.end()){
				AssignNode* node = new AssignNode(lname);
				assign_pos_[lname] = node;
			}
			AssignNode* l_node = assign_pos_[lname];
			l_node->in_ += rnames.size();
			l_node->assign_= assign;

			//Build node for rref.
			for(string rname : rnames){
				if(assign_pos_.find(rname) == assign_pos_.end()){
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
	for(pos; pos != assign_pos_.end(); pos++){
		node_map[pos->first] = pos->second;
	}
	//Get the 0 indegree node and delete it's link until map is empty.
	while(!node_map.empty()){
		unordered_map<string, AssignNode*>::iterator m_pos = node_map.begin();
		for(m_pos; m_pos != node_map.end(); m_pos++){
			if(!m_pos->second->in_){
				if(m_pos->second->assign_){
					assigns_.push_back(m_pos->second->assign_);
				}
				for(string next : m_pos->second->next_){
					assign_pos_[next]->in_ -= 1;
				}
				node_map.erase(m_pos->first);
				break;
			}
		}
	}
}

void Module::parse_wires(map<string, RefVar*>& vars)
{
	map<string, AssignNode*>::iterator node = assign_pos_.begin();
	for(node; node != assign_pos_.end(); node++){
		if(vars.find(node->first) != vars.end()){
			if(node->second->assign_){
				vars[node->first]->ptype = "Assign";
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

void Module::gen_assign_smt(set<string>& refs, bool type, ofstream& o, map<string, RefVar*>& vars, set<SmtVar*>& used)
{
	map<PGAssign*, string> select_assigns;
	for(string var : refs){
		if(assign_pos_.find(var) != assign_pos_.end()){
			if(assign_pos_[var]->assign_){
				find_assign(var, select_assigns);
			}
		}
	}

	list<PGAssign*>::iterator assign = assigns_.begin();
	for(assign; assign != assigns_.end(); assign++){
		if(type || select_assigns.find(*assign) != select_assigns.end()){
			(*assign)->dump_smt(design_, o, vars, used, this);
		}
	}

	map<PGAssign*, string>::iterator pos = select_assigns.begin();
	for(pos; pos != select_assigns.end(); pos++){
		refs.insert(pos->second);
	}

}