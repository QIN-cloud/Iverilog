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
# include  "Statement.h"
# include  <cassert>
# include  "string.h"
# include  <stack>
# include  <unordered_map>
# include  <unistd.h>


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
	// for(pair<Cfg_Node*, PathNode*> pn : path_nodes_) {
	// 	delete pn.second;
	// }
	// map<unsigned, BranchTree*>::iterator pos = branchs_.begin();
	// for(; pos != branchs_.end(); pos++) {
	// 	delete pos->second;
	// }
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
	assert(mn_);
	cfg_ = mn_->build_cfgs();
}

void Module::get_condit() {
	svector<Cfg*>* cfgs = cfg_->cfgs;
	for(int i = 0; i < (*cfgs).count(); i++) {
		Cfg* cfg = (*cfgs)[i];
		for(int j = 0; j < cfg->root->count(); j++) {
			Cfg_Node* cn = (*(cfg->root))[j];
			if(cn->type.find("ISCONTROL") == 0) {
				for(string ref : cn->refs){
					condit[ref] = true;
				}
			}
		}

	}
}

void Module::dump_cfg(ostream& o) const
{
	o << "module " << pscope_name() << endl;
	o << "{" << endl; 
	for(unsigned y = 0; y < cfg_->cfgs->count(); ++y) {
		o << "Process ID: " << (*(cfg_->cfgs))[y]->id << endl;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[y]->root->count(); ++z) {
			o << "Index = " << z <<", ";
			o << *(*((*(cfg_->cfgs))[y]->root))[z];
			o << endl;
		}
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
	if(!assign_.empty())
	{
		for(PGAssign* assign : assign_)
		{
			assign->dump(cout);
		}
	}
	o << "next : ";
	unordered_map<string, bool>::iterator next = next_.begin();
	for(next; next != next_.end(); next++){
		o << next->first << " ";
	}
	o << endl;
	o << "}" << endl;
}

void Module::build_exprs()
{
    map<PExpr*, set<PExpr*> > expr_item;
	for(PGAssign* assign : assign_order){
		assign->build_expr(expr_item);
	}
	for(unsigned i = 0; i < cfg_->cfgs->count(); i++){
		Cfg* cfg = (*(cfg_->cfgs))[i];
		for(unsigned j = 0; j < cfg->root->count(); j++){
			Cfg_Node* node = (*(cfg->root))[j];
			if(node->type.find("ISCONTROL") == 0
			&& node->type.compare("ISCONTROL.EVENT") != 0){
				node->expr[0]->build_expr(expr_item, nullptr);
			}
		}
	}
	map<PExpr*, set<PExpr*> >::iterator pos = expr_item.begin();
	for(; pos != expr_item.end(); pos++){
        ExpressionNode* node = new ExpressionNode;
        node->expr = pos->first;
        unsigned id = 0;
        for(PExpr* item : pos->second) {
            node->item[item] = id++;
        }
	}
}

void Module::get_vhdl_line()
{
	vhdl_line.push_back("");
    perm_string file = get_file();
    fstream in(file);
    string temp;
    while(!in.eof()) {
        getline(in, temp);
        vhdl_line.push_back(temp.c_str());
    }
}

void Module::parse_assigns()
{
	//Build the Directed Graph by assign statements.
	list<PGate*>::iterator gate = gates_.begin();
	for(gate; gate != gates_.end(); gate++)
	{
		if((*gate)->ty == PGate::ASSIGN)
		{
			PGAssign* assign = dynamic_cast<PGAssign*>(*gate);
			//Parsing the string names for every variable in assign statement.
			set<string> lnames = assign->pin(0)->get_var_names();
			set<string> rnames = assign->pin(1)->get_var_names();

			//Build node for lref.
			for(string lname : lnames)
			{
				if(assign_map.find(lname) == assign_map.end())
				{
					AssignNode* node = new AssignNode(lname);
					assign_map[lname] = node;
				}
				AssignNode* l_node = assign_map[lname];
				l_node->assign_.push_back(assign);

				//Build node for rref.
				for(string rname : rnames)
				{
					if(rname.compare(lname) == 0)
						continue;
					if(assign_map.find(rname) == assign_map.end())
					{
						AssignNode* node = new AssignNode(rname);
						assign_map[rname] = node;
					}
					AssignNode* r_node = assign_map[rname];
					if(r_node->next_.find(lname) == r_node->next_.end())
					{
						r_node->out_ += 1;
						l_node->in_ += 1;
						r_node->next_[lname] = true;
					}
				}
			}
		}
	}
}

void Module::sort_assigns()
{
	if(assign_map.empty()) return;
	//Build a unordered map for sorting the assign statements.
	unordered_map<string, AssignNode*> node_map;
	map<string, AssignNode*>::iterator pos = assign_map.begin();
	for(pos; pos != assign_map.end(); pos++)
	{
		node_map[pos->first] = pos->second;
	}
	//Get the 0 indegree node and delete it's link until map is empty.
	map<PGAssign*, bool> records;
	while(!node_map.empty())
	{
		unordered_map<string, AssignNode*>::iterator m_pos = node_map.begin();
		for(m_pos; m_pos != node_map.end(); m_pos++)
		{
			if(m_pos->second->in_ == DELETE_INDEGREE)
			{
				if(!m_pos->second->assign_.empty())
				{
					for(PGAssign* assign : m_pos->second->assign_)
					{
						if(records.find(assign) == records.end())
						{
							assign_order.push_back(assign);
							records[assign] = true;
						}
					}
				}
				unordered_map<string, bool>::iterator n_pos = m_pos->second->next_.begin();
				for(; n_pos != m_pos->second->next_.end(); n_pos++)
				{
					assign_map[n_pos->first]->in_ -= 1;
				}
				node_map.erase(m_pos->first);
				break;
			}
		}
	}
	records.clear();
}

void Module::find_assign(string var, map<PGAssign*, bool>& select_assigns, set<string>& assign_defs)
{
	if(!assign_map[var]->assign_.empty()){
		for(PGAssign* assign : assign_map[var]->assign_){
			select_assigns[assign] = true;
		}
		assign_defs.insert(var);
	}

	unordered_map<string, bool>::iterator next = assign_map[var]->next_.begin();
	for(; next != assign_map[var]->next_.end(); next++){
		find_assign(next->first, select_assigns, assign_defs);
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
		if(!cfg->always) continue;
		if(cfg->sync) {
			sync_always.push_back(cfg);
		} else {
			complete_defs(cfg, cfg_used);
			cfgs.push_back(cfg);
		}
	}

	if(cfgs.size() == 1)
	{
		comb_always.push_back(cfgs.front());
	} else {
		//Compare the excuted order of combine cfgs.
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

		//Build a Direct Graph by combine cfg orders.
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

		//Build node for others combine cfg.
		for(Cfg* cfg : cfgs) {
			if(d_graph.find(cfg) == d_graph.end()) {
				CfgTPNode* node = new CfgTPNode(cfg);
				d_graph[cfg] = node;
			}
		}

		//Top-logic sort the combine process by Direct Graph.
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
						comb_always.push_back(pos->first);
					}
				}
			}
		}
	}
}

void Module::complete_defs(Cfg* cfg, unordered_map<Cfg*, unordered_map<string, bool> >& used)
{
	map<PGAssign*, bool> select_assigns;
	set<string> assign_defs;
	set<string> refs = cfg->refs;
	set<string> defs = cfg->defs;

	if(!defs.empty()){
		for(string var : defs){
			if(assign_map.find(var) != assign_map.end()){
				find_assign(var, select_assigns, assign_defs);
			}
		}
	}

	for(string def : assign_defs)
	{
		defs.insert(def);
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

	return nullptr;
}

void Module::build_lines() 
{
	for(unsigned i = 0; i < cfg_->cfgs->count(); i++) 
	{
		Cfg* cfg = (*(cfg_->cfgs))[i];
		unsigned count = 0;
		for(unsigned j = 0; j < cfg->root->count(); j++)
		{
			Cfg_Node* node = (*(cfg->root))[j];
			if(node->lineno > 0) 
			{
				if(cfg->pp_line.find(node->lineno) == cfg->pp_line.end()) 
				{
					cfg->pp_line[node->lineno] = count++;
				}
			}
		}
	}
}

void Module::build_paths() 
{
	for(unsigned i = 0; i < cfg_->cfgs->count(); i++)
	{
		Cfg* cfg = (*(cfg_->cfgs))[i];
		for(unsigned j = 0; j < cfg->root->count(); j++)
		{
			Cfg_Node* node = (*(cfg->root))[j];
			if(node->type == "ISCONTROL.EVENT") 
			{
				CoverBitVecArray* bv = new CoverBitVecArray(cfg->pp_line.size());
				bv->set_high(cfg->pp_line[node->lineno]);
				build_cfg_path(cfg, ++j, bv);
				delete bv;
				break;
			}
		}
	}
}

void Module::build_cfg_path(Cfg* cfg, unsigned idx, CoverBitVecArray* bv) 
{
	Cfg_Node* node = (*(cfg->root))[idx];
	if(node->type == "ENDNODE") {
		assert(bv);
		CoverBitVecArray* nbv = new CoverBitVecArray(bv->get_size());
		nbv->merge_array(bv);
		cfg->pp_path.push_back(nbv);
		return;
	}
	bv->set_high(cfg->pp_line[node->lineno]);
	if(node->type.find("ISCONTROL") == 0) {
		for(unsigned i = 0; i < node->dsuc.count(); ++i){
			build_cfg_path(cfg, node->dsuc[i]->index, bv);
		}
	}	
	else {
		build_cfg_path(cfg, node->dsuc[0]->index, bv);
	}
	bv->set_low(cfg->pp_line[node->lineno]);
}