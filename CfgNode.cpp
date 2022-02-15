#include "config.h"
#include "compiler.h"
#include "CfgNode.h"
#include "PExpr.h"

CfgNode::CfgNode(int ln)
:lineno_(ln)
{
	assign_type_ = "";
	casetype = -1;
}

CfgNode::CfgNode()
:lineno_(-1)
{
	assign_type_ = "";
	casetype = -1;
}

CfgNode::~CfgNode()
{
}

void CfgNode::dump(ostream&o)
{
  set<string>::iterator pos;
  o<<"Line: "<<lineno_<<", REF={";
  for(pos = refs_.begin(); pos != refs_.end(); ++pos)
    o<<*pos<<" ";
  o<<"}, DEF={";
  for(pos = defs_.begin(); pos != defs_.end(); ++pos)
    o<<*pos<<" ";
  o<<"},INFLUENCE={";
  
  set<int, less<int> >::iterator ipos;
  for(ipos = infls_.begin(); ipos != infls_.end(); ++ipos)
    o<<*ipos<<" ";
  o<<"}, DIRECTSUCCUSS={";
  for(ipos = dsuccs_.begin(); ipos != dsuccs_.end(); ++ipos)
    o<<*ipos<<" ";
  o<<"}";
  
  for(unsigned idx = 0; idx < expr_.count(); ++idx)
  {
	expr_[idx]->dump(o);
	o << ", ";
  }
  o << endl;
}

svector<Node*>& CfgNode::construct_node(int t)
{
	Node* node = new Node;
	node->lineno_ = lineno_;
	node->refs_ = refs_;
	node->defs_ = defs_;
	node->infls_ = infls_;
	node->dsuccs_ = dsuccs_;
	node->funcs_ = funcs_;
	node->mods_ = mods_;
	node->expr_ = svector<PExpr*>(expr_);
	node->caseitem_ = svector<PExpr*>(caseitem_);
	if(assign_type_ != "")
		node->assign_type = assign_type_;

	switch(t){
		case 0:
			node->type = "NOCONTROL";
			break;
		case 1: 
			node->type = "ISCONTROL.CONDITION";
			break;
		case 2:
			node->type = "PROCESS";
			break;
		case 3:
			node->type = "ASSIGN";
			break;
		case 4:
			node->type = "ISCONTROL.LOOP";
			break;
		case 5:
			node->type = "ISCONTROL.CASE";
			break;
		case 6:
			node->type = "ISCONTROL.EVENT";
			break;
		case 7:
			node->type = "ISCONTROL.CASEX";
			break;
		case 8:
			node->type = "ISCONTROL.CASEZ";
			break;
	}

	svector<Node*>* nodes = new svector<Node*>();
	nodes = new svector<Node*>(*nodes, node);
	return *nodes;
}

set<string>& CfgNode::get_refs()
{
	return refs_;
}

set<string>& CfgNode::get_defs()
{
	return defs_;
}

unsigned CfgNode::ref_count()
{
	return refs_.size();
}

unsigned CfgNode::def_count()
{
	return defs_.size();
}

string CfgNode::find_ref(string& ref)
{
	set<string>::iterator pos = refs_.find(ref);
	static string tmps("");
	if(pos != refs_.end())
	  return *pos;
	else
	  return tmps;
}

string CfgNode::find_def(string& def)
{
	set<string>::iterator pos = defs_.find(def);
	static string tmps("");
	if(pos != defs_.end())
	  return *pos;
	else
	  return tmps;
}

set<int, less<int> >& CfgNode::get_infls()
{
	return infls_;
}

set<int, less<int> >& CfgNode::get_dirsuccs()
{
	return dsuccs_;
}
 
unsigned CfgNode::infl_count()
{
	return infls_.size();
}

unsigned CfgNode::dsucc_count()
{
	return dsuccs_.size();
}

int CfgNode::find_infl(int infl)
{
	set<int, less<int> >::iterator pos = infls_.find(infl);
	if(pos != infls_.end())
	  return *pos;
	else
	  return -1;
}

int CfgNode::find_dirsucc(int dsucc)
{
	set<int, less<int> >::iterator pos = dsuccs_.find(dsucc);
	if(pos != dsuccs_.end())
	  return *pos;
	else
	  return -1;
}
    
void CfgNode::add_ref(string& ref)
{
	refs_.insert(ref);
}

void CfgNode::add_def(string& def)
{
	defs_.insert(def);
}

void CfgNode::add_refs(svector<string>& refs)
{
	for(unsigned idx = 0; idx < refs.count(); ++idx)
		refs_.insert(refs[idx]);
}

void CfgNode::add_refs(set<string>& refs)
{
	set<string>::iterator pos;
	for(pos = refs.begin(); pos != refs.end(); ++pos)
		refs_.insert(*pos);
}

void CfgNode::add_funcs(set<string>& funcs)
{
	set<string>::iterator pos;
	for(pos = funcs.begin(); pos != funcs.end(); ++pos)
		funcs_.insert(*pos);
}

void CfgNode::add_mods(set<string>& mods)
{
	set<string>::iterator pos;
	for(pos = mods.begin(); pos != mods.end(); ++pos)
		mods_.insert(*pos);
}

void CfgNode::add_defs(set<string>& defs)
{
	set<string>::iterator pos;
	for(pos = defs.begin(); pos != defs.end(); ++pos)
		defs_.insert(*pos);
}

void CfgNode::add_defs(svector<string>& defs)
{
	for(unsigned idx = 0; idx < defs.count(); ++idx)
		defs_.insert(defs[idx]);
}

void CfgNode::add_infl(svector<unsigned>&infl)
{
	for(unsigned idx = 0; idx < infl.count(); ++idx)
            infls_.insert(infl[idx]);
}

void CfgNode::add_dsucc(int dsucc)
{
	dsuccs_.insert(dsucc);
}
    
void CfgNode::set_lineno(int lineno)
{
	lineno_ = lineno;
}

int CfgNode::get_lineno()
{
	return lineno_;
}

BlockNode::BlockNode()
:CfgNode()
{
}

BlockNode::BlockNode(int ln)
:CfgNode(ln)
{
}

BlockNode::~BlockNode()
{
}

void BlockNode::add_node(CfgNode* node)
{
	if(nodes_.count() >= 1)
		nodes_[nodes_.count()-1]->add_dsucc(node->get_lineno());
	nodes_ = svector<CfgNode*>(nodes_, node);
}

CfgNode* BlockNode::get_node(int idx)
{
	assert(idx < nodes_.count());
              return nodes_[idx];
}

svector<CfgNode*>& BlockNode::get_nodes()
{
	return nodes_;
}
      
int BlockNode::find_node(CfgNode& node)
{
	for(unsigned idx = 0; idx < nodes_.count(); ++idx)
                if(nodes_[idx]->get_lineno() == node.get_lineno())
	            return idx;
	return -1;
}

void BlockNode::add_dsucc(int dsucc)
{
	nodes_[nodes_.count()-1]->add_dsucc(dsucc);
}

set<string>& BlockNode::get_refs()
{
	set<string>* refs = new set<string>;
	set<string>::iterator pos;
	set<string> ref;
	for(unsigned idx = 0; idx < nodes_.count(); ++idx)
	{
		ref  = nodes_[idx]->get_refs();
		for(pos = ref.begin(); pos != ref.end(); ++pos)
			refs->insert(*pos);
	}
	return *refs;
}
      
set<string>& BlockNode::get_defs()
{
      	set<string>* defs = new set<string>;
      	for(unsigned idx = 0; idx < nodes_.count(); ++idx)
      	{
      		set<string>::iterator pos;
		for(pos = nodes_[idx]->get_defs().begin(); pos != nodes_[idx]->get_defs().end(); ++pos)
		defs->insert(*pos);
		     
      	}
      	return *defs;
}

set<string>& BlockNode::get_funcs()
{
	  set<string>* tmp = new set<string>;
	  set<string>::const_iterator pos;
	  for(unsigned idx = 0; idx < nodes_.count(); ++idx)
	  {
		  for(pos = nodes_[idx]->get_funcs().begin(); 
		  pos != nodes_[idx]->get_funcs().end();
		  ++pos)
			  tmp->insert(*pos);
	  }
	  return *tmp;
}

set<string>& BlockNode::get_mods()
{
	  set<string>* tmp = new set<string>;
	  set<string>::const_iterator pos;
	  for(unsigned idx = 0; idx < nodes_.count(); ++idx)
	  {
		  for(pos = nodes_[idx]->get_mods().begin(); 
		  pos != nodes_[idx]->get_mods().end();
		  ++pos)
			  tmp->insert(*pos);
	  }
	  return *tmp;
}
      
CaseNode::CaseNode()
:CfgNode()
{
}

CaseNode::CaseNode(int ln)
:CfgNode(ln)
{
}

CaseNode::~CaseNode()
{
}


void CaseNode::add_node(CfgNode* node)
{
	paths_ = svector<CfgNode*>(paths_, node);
}

svector<CfgNode*>& CaseNode::get_nodes()
{
	return paths_;
}

CfgNode* CaseNode::get_node(int idx)
{
	assert(idx < paths_.count());
	return paths_[idx];
}

unsigned CaseNode::path_count()
{
	return paths_.count();
}

void CaseNode::add_dsucc(int dsucc)
{
	for(unsigned idx = 1; idx < paths_.count(); ++idx)
	{
	    paths_[idx]->add_dsucc(dsucc);
	}
}

set<string>& CaseNode::get_refs()
{
      	set<string>* refs = new set<string>;
      	for(unsigned idx = 0; idx < paths_.count(); ++idx)
      	{
      		set<string>::iterator pos;
		for(pos = paths_[idx]->get_refs().begin(); pos != paths_[idx]->get_refs().end(); ++pos)
			refs->insert(*pos);
      	}
      	return *refs;
}

set<string>& CaseNode::get_defs()
{
      	set<string>* defs = new set<string>;
      	for(unsigned idx = 0; idx < paths_.count(); ++idx)
      	{
		set<string>::iterator pos;
		for(pos = paths_[idx]->get_defs().begin(); pos != paths_[idx]->get_defs().end(); ++pos)
      		defs->insert(*pos);
      	}
      	return *defs;
}

set<string>& CaseNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	for(unsigned idx = 0; idx < paths_.count(); ++idx)
	{
      		set<string>::iterator pos;
			for(pos = paths_[idx]->get_funcs().begin(); pos != paths_[idx]->get_funcs().end(); ++pos)
				tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& CaseNode::get_mods()
{
	set<string>* tmp = new set<string>;
	for(unsigned idx = 0; idx < paths_.count(); ++idx)
	{
      		set<string>::iterator pos;
			for(pos = paths_[idx]->get_mods().begin(); pos != paths_[idx]->get_mods().end(); ++pos)
				tmp->insert(*pos);
	}
	return *tmp;
}

CondNode::CondNode()
:CfgNode()
{
}

CondNode::CondNode(int ln)
:CfgNode(ln)
{
}

CondNode::~CondNode()
{
}

void CondNode::add_node(CfgNode* node, int type)
{
    	if(type == 0)
    	  root_ = node;
    	else if(type == 1)
    	  if_ = node;
    	else
    	  else_ = node;
}

CfgNode* CondNode::get_node(int type)
{
    	if(type == 0)
    	  return root_;
    	else if(type == 1)
    	  return if_;
    	else
    	  return else_;
}

void CondNode::add_dsucc(int dsucc)
{
	if(if_ && else_)//has if and else
	{
	   	if_->add_dsucc(dsucc);
		else_->add_dsucc(dsucc);
	}
	if(if_ && !else_)//has if but no else
	{
	   	if_->add_dsucc(dsucc);
		root_->add_dsucc(dsucc);
	}
	if(!if_ && else_)//no if but has else
	{
		else_->add_dsucc(dsucc);
		root_->add_dsucc(dsucc);
	}
	if(!if_ && !else_)
	{
		root_->add_dsucc(dsucc);
	}
}

set<string>& CondNode::get_refs()
{
	set<string>* refs = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_refs().begin(); pos != root_->get_refs().end();++pos)
	       	  refs->insert(*pos);
	}
	if(if_){
		for(pos = if_->get_refs().begin(); pos != if_->get_refs().end();++pos)
	       	  refs->insert(*pos);
	}
	if(else_){
		for(pos = else_->get_refs().begin(); pos != else_->get_refs().end();++pos)
       		  refs->insert(*pos);
	}
      	return *refs;
}

set<string>& CondNode::get_defs()
{
	set<string>* defs = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_defs().begin(); pos != root_->get_defs().end();++pos)
	       	  defs->insert(*pos);
	}
	if(if_){
		for(pos = if_->get_defs().begin(); pos != if_->get_defs().end();++pos)
       		  defs->insert(*pos);
	}
	if(else_){
		for(pos = else_->get_defs().begin(); pos != else_->get_defs().end();++pos)
       		  defs->insert(*pos);
	}
      	return *defs;
}

set<string>& CondNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_funcs().begin(); pos != root_->get_funcs().end();++pos)
	       	  tmp->insert(*pos);
	}
	if(if_){
		for(pos = if_->get_funcs().begin(); pos != if_->get_funcs().end();++pos)
       		  tmp->insert(*pos);
	}
	if(else_){
		for(pos = else_->get_funcs().begin(); pos != else_->get_funcs().end();++pos)
       		  tmp->insert(*pos);
	}
      	return *tmp;
}
	
set<string>& CondNode::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_mods().begin(); pos != root_->get_mods().end();++pos)
	       	  tmp->insert(*pos);
	}
	if(if_){
		for(pos = if_->get_mods().begin(); pos != if_->get_mods().end();++pos)
       		  tmp->insert(*pos);
	}
	if(else_){
		for(pos = else_->get_mods().begin(); pos != else_->get_mods().end();++pos)
       		  tmp->insert(*pos);
	}
      	return *tmp;
}
     
EventNode::EventNode()
:CfgNode()
{
}

EventNode::EventNode(int ln)
:CfgNode(ln)
{
}

EventNode::~EventNode()
{
}

void EventNode::add_node(CfgNode* node, int type)
{
    	if(type == 0)
    	  root = node;
    	else
    	  st = node;
}
 
CfgNode* EventNode::get_node(int type)
{
    	if(type == 0)
    	  return root;
    	else 
    	  return st;
}

void EventNode::add_dsucc(int dsucc)
{
	if(st)
	    	st->add_dsucc(dsucc);
}

set<string>& EventNode::get_refs()
{
	set<string>* refs = new set<string>;
	set<string>::iterator pos;
	if(root){
		for(pos = root->get_refs().begin(); pos != root->get_refs().end(); ++pos)
      		    refs->insert(*pos);
	}
	if(st){
		for(pos = st->get_refs().begin(); pos != st->get_refs().end(); ++pos)
      		    refs->insert(*pos);
	}
      	return *refs;
}

set<string>& EventNode::get_defs()
{
	set<string>* defs = new set<string>;
	set<string>::iterator pos;
	if(root){
		for(pos = root->get_defs().begin(); pos != root->get_defs().end(); ++pos)
      		    defs->insert(*pos);
	}
	if(st){
		for(pos = st->get_defs().begin(); pos != st->get_defs().end(); ++pos)
      		    defs->insert(*pos);
	}
      	return *defs;
}

set<string>& EventNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root){
		for(pos = root->get_funcs().begin(); pos != root->get_funcs().end(); ++pos)
      		    tmp->insert(*pos);
	}
	if(st){
		for(pos = st->get_funcs().begin(); pos != st->get_funcs().end(); ++pos)
      		    tmp->insert(*pos);
	}
      	return *tmp;
}

set<string>& EventNode::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root){
		for(pos = root->get_mods().begin(); pos != root->get_mods().end(); ++pos)
      		    tmp->insert(*pos);
	}
	if(st){
		for(pos = st->get_mods().begin(); pos != st->get_mods().end(); ++pos)
      		    tmp->insert(*pos);
	}
      	return *tmp;
}

void EventNode::dump(ostream&o)
{
	    if(root)
		    root->dump(o);
	    if(st)
		    st->dump(o);
}

LoopNode::LoopNode()
:CfgNode()
{
}

LoopNode::LoopNode(int ln)
:CfgNode(ln)
{
}

LoopNode::~LoopNode()
{
}

void LoopNode::add_node(CfgNode* node, int type)
{
    	if(type == 0)
    	  root_ = node;
    	else
    	  st_ = node;
}

CfgNode* LoopNode::get_node(int type)
{
    	if(type == 0)
    	  return root_;
    	else 
    	  return st_;
}

void LoopNode::add_dsucc(int dsucc)
{
	if(root_)
	    	root_->add_dsucc(dsucc);
}

set<string>& LoopNode::get_refs()
{
	set<string>* refs = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_refs().begin(); pos != root_->get_refs().end(); ++pos)
	      	    refs->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_refs().begin(); pos != st_->get_refs().end(); ++pos)
	      	    refs->insert(*pos);
	}
      	return *refs;
}

set<string>& LoopNode::get_defs()
{
	set<string>* defs = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_defs().begin(); pos != root_->get_defs().end(); ++pos)
	      	    defs->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_defs().begin(); pos != st_->get_defs().end(); ++pos)
	      	    defs->insert(*pos);
	}
      	return *defs;
}

set<string>& LoopNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_funcs().begin(); pos != root_->get_funcs().end(); ++pos)
	      	    tmp->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_funcs().begin(); pos != st_->get_funcs().end(); ++pos)
	      	    tmp->insert(*pos);
	}
      	return *tmp;
}

set<string>& LoopNode::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_mods().begin(); pos != root_->get_mods().end(); ++pos)
	      	    tmp->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_mods().begin(); pos != st_->get_mods().end(); ++pos)
	      	    tmp->insert(*pos);
	}
      	return *tmp;
}
                                      
ProcessNode::ProcessNode()
:CfgNode()
{
}

ProcessNode::ProcessNode(int ln)
:CfgNode(ln)
{
}

ProcessNode::~ProcessNode()
{
}

void ProcessNode::add_node(CfgNode* node, int type)
{
	if(type == 0)
	  root_ = node;
	else
	  st_ = node;
}

CfgNode* ProcessNode::get_node(int type)
{
	if(type == 0)
	  return root_;
	else
	  return st_;	
}

set<string>& ProcessNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_funcs().begin(); pos != root_->get_funcs().end(); ++pos)
	      	    tmp->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_funcs().begin(); pos != st_->get_funcs().end(); ++pos)
	      	    tmp->insert(*pos);
	}
      	return *tmp;
}

set<string>& ProcessNode::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	if(root_){
		for(pos = root_->get_mods().begin(); pos != root_->get_mods().end(); ++pos)
	      	    tmp->insert(*pos);
	}
	if(st_)
	{
		for(pos = st_->get_mods().begin(); pos != st_->get_mods().end(); ++pos)
	      	    tmp->insert(*pos);
	}
      	return *tmp;
}

ModuleNode::ModuleNode()
:CfgNode()
{
}

ModuleNode::ModuleNode(int ln)
:CfgNode(ln)
{
}

ModuleNode::~ModuleNode()
{
}
    
void ModuleNode::add_node(ProcessNode* node, int type)
{
	switch(type){
	  case 0:
	    procs_ = svector<ProcessNode*>(procs_, node);
	    break;
	  case 1:
	    funcs_ = svector<ProcessNode*>(funcs_, node);
	    break;
	  case 2:
	    tasks_ = svector<ProcessNode*>(tasks_, node);
	}
}

svector<ProcessNode*> ModuleNode::get_tasknodes()
{
	return tasks_;
}

void ModuleNode::add_node(CfgNode* node)
{
	assigns_ = svector<CfgNode*>(assigns_, node);
}

svector<ProcessNode*> ModuleNode::get_procnodes()
{
	return procs_;
}

svector<ProcessNode*> ModuleNode::get_funcnodes()
{
	return funcs_;
}

svector<CfgNode*> ModuleNode::get_assignnodes()
{
	return assigns_;
}

unsigned ModuleNode::nodes_count(int type)
{
	switch(type){
	  case 0:
	    return procs_.count();
	    break;
	  case 1:
	    return funcs_.count();
	    break;
	  case 2:
	    return tasks_.count();
	    break;
	  case 3:
	    return assigns_.count();
	    break;
	}
}

CfgNode* ModuleNode::get_procnode(int idx, int type)
{
	switch(type){
	  case 0:
	    assert(idx < procs_.count());
	    return procs_[idx];
	  case 1:
	    assert(idx < funcs_.count());
	    return funcs_[idx];
	  case 2:
	    assert(idx < tasks_.count());
	    return tasks_[idx];
	}
}

CfgNode* ModuleNode::get_assignnode(int idx)
{
	assert(idx < assigns_.count());
	return assigns_[idx];
}

int ModuleNode::find_node(CfgNode& node, int type)
{
	unsigned idx;
	switch(type){
	  case 0:
	    for(idx = 0; idx < procs_.count(); ++idx)
	      if(node.get_lineno() == procs_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 1:
	    for(idx = 0; idx < funcs_.count(); ++idx)
	      if(node.get_lineno() == funcs_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 2:
	    for(idx = 0; idx < tasks_.count(); ++idx)
	      if(node.get_lineno() == tasks_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 3:
	    for(idx = 0; idx < assigns_.count(); ++idx)
	      if(node.get_lineno() == assigns_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  default:
		  return -1;
		  break;
	}
}

int ModuleNode::find_node(int lineno, int type)
{
	unsigned idx;
	switch(type){
	  case 0:
	    for(idx = 0; idx < procs_.count(); ++idx)
	      if(lineno == procs_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 1:
	    for(idx = 0; idx < funcs_.count(); ++idx)
	      if(lineno == funcs_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 2:
	    for(idx = 0; idx < tasks_.count(); ++idx)
	      if(lineno == tasks_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  case 3:
	    for(idx = 0; idx < assigns_.count(); ++idx)
	      if(lineno == assigns_[idx]->get_lineno())
	        return idx;
	    return -1;
		break;
	  default:
		  return -1;
		  break;
	}
}

set<string>& ModuleNode::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	unsigned idx;
	for(idx = 0; idx < procs_.count(); ++idx)
	{
		for(pos = procs_[idx]->get_funcs().begin(); pos != procs_[idx]->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < funcs_.count(); ++idx)
	{
		for(pos = funcs_[idx]->get_funcs().begin(); pos != funcs_[idx]->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < tasks_.count(); ++idx)
	{
		for(pos = tasks_[idx]->get_funcs().begin(); pos != tasks_[idx]->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < assigns_.count(); ++idx)
	{
		for(pos = assigns_[idx]->get_funcs().begin(); pos != assigns_[idx]->get_funcs().end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

set<string>& ModuleNode::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::iterator pos;
	unsigned idx;
	for(idx = 0; idx < procs_.count(); ++idx)
	{
		for(pos = procs_[idx]->get_mods().begin(); pos != procs_[idx]->get_mods().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < funcs_.count(); ++idx)
	{
		for(pos = funcs_[idx]->get_mods().begin(); pos != funcs_[idx]->get_mods().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < tasks_.count(); ++idx)
	{
		for(pos = tasks_[idx]->get_mods().begin(); pos != tasks_[idx]->get_mods().end(); ++pos)
			tmp->insert(*pos);
	}
	for(idx = 0; idx < assigns_.count(); ++idx)
	{
		for(pos = assigns_[idx]->get_mods().begin(); pos != assigns_[idx]->get_mods().end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

void BlockNode::dump(ostream& o)
{
	for(unsigned idx = 0; idx < nodes_.count(); ++idx)
		nodes_[idx]->dump(o);
}

svector<Node*>& BlockNode::construct_node(int)
{
	svector<Node*>* nodes = new svector<Node*>();
	for(unsigned idx = 0; idx < nodes_.count(); ++idx)
		nodes = new svector<Node*>(*nodes, nodes_[idx]->construct_node(0));
	return *nodes;
}

void CaseNode::dump(ostream& o)
{
	for(unsigned idx = 0; idx < paths_.count(); ++idx)
		paths_[idx]->dump(o);
}

svector<Node*>& CaseNode::construct_node(int)
{
	svector<Node*>* nodes = new svector<Node*>();
	for(unsigned idx = 0; idx < paths_.count(); ++idx)
	{
		if(idx==0)
		{
			switch(paths_[idx]->get_casetype())
			{
			case 0:
				nodes = new svector<Node*>(*nodes, paths_[idx]->construct_node(5));
				break;
			case 1:
				nodes = new svector<Node*>(*nodes, paths_[idx]->construct_node(7));
				break;
			case 2:
				nodes = new svector<Node*>(*nodes, paths_[idx]->construct_node(8));
				break;
			}
		}
		else
			nodes = new svector<Node*>(*nodes, paths_[idx]->construct_node(0));
	}
	return *nodes;
}

void CondNode::dump(ostream& o)
{
	if(root_)
		root_->dump(o);
	if(if_)
		if_->dump(o);
	if(else_)
		else_->dump(o);
}

svector<Node*>& CondNode::construct_node(int)
{
	svector<Node*>* nodes = new svector<Node*>();
	if(root_)
                nodes = new svector<Node*>(*nodes, root_->construct_node(1));
        if(if_)
                nodes = new svector<Node*>(*nodes, if_->construct_node(0));
        if(else_)
                nodes = new svector<Node*>(*nodes, else_->construct_node(0));
	return *nodes;
}

void LoopNode::dump(ostream& o)
{
	if(root_)
		root_->dump(o);
	if(st_)
		st_->dump(o);
}

svector<Node*>& LoopNode::construct_node(int)
{
        svector<Node*>* nodes = new svector<Node*>();
        if(root_)
                nodes = new svector<Node*>(*nodes, root_->construct_node(4));
        if(st_)
                nodes = new svector<Node*>(*nodes, st_->construct_node(0));
        return *nodes;
}

void ProcessNode::dump(ostream&o)
{
	if(root_)
		root_->dump(o);
	if(st_)
		st_->dump(o);
}

svector<Node*>& ProcessNode::construct_node(int)
{
        svector<Node*>* nodes = new svector<Node*>();
        if(root_)
                nodes = new svector<Node*>(*nodes, root_->construct_node(2));
        if(st_)
                nodes = new svector<Node*>(*nodes, st_->construct_node(0));
		for(unsigned idx = 1; idx < nodes->count(); ++idx)
			if((*nodes)[0]->lineno_ != (*nodes)[idx]->lineno_)
				(*nodes)[0]->infls_.insert((*nodes)[idx]->lineno_);
        return *nodes;
}

void ModuleNode::dump(ostream&o)
{
	unsigned idx;
	for(idx = 0; idx <procs_.count(); ++idx)
		procs_[idx]->dump(o);
	for(idx = 0; idx <funcs_.count(); ++idx)
		funcs_[idx]->dump(o);
	for(idx = 0; idx <tasks_.count(); ++idx)
		tasks_[idx]->dump(o);
	for(idx = 0; idx <assigns_.count(); ++idx)
		assigns_[idx]->dump(o);
}

svector<Node*>& ModuleNode::construct_node(int)
{
	svector<Node*>* nodes = new svector<Node*>();
	unsigned idx;
	for(idx = 0; idx <procs_.count(); ++idx)
		nodes = new svector<Node*>(*nodes, procs_[idx]->construct_node(2));
	for(idx = 0; idx <funcs_.count(); ++idx)
		nodes = new svector<Node*>(*nodes, funcs_[idx]->construct_node(2));
	for(idx = 0; idx <tasks_.count(); ++idx)
		nodes = new svector<Node*>(*nodes, tasks_[idx]->construct_node(2));
	for(idx = 0; idx <assigns_.count(); ++idx)
		nodes = new svector<Node*>(*nodes, assigns_[idx]->construct_node(3));
	return *nodes;
}

svector<Node*>& EventNode::construct_node(int)
{
	svector<Node*>* nodes = new svector<Node*>();
	if(root)
		nodes = new svector<Node*>(*nodes, root->construct_node(6));
    if(st)
		nodes = new svector<Node*>(*nodes, st->construct_node(0));
	return *nodes;
}

ostream& operator << (ostream& o, const Node& n)
{
	set<string>::const_iterator pos;
      	o<<"Line: "<<n.lineno_<<", REF={";
	for(pos = n.refs_.begin(); pos != n.refs_.end(); ++pos)
		o<<*pos<<" ";
	o<<"}, DEF={";
	for(pos = n.defs_.begin(); pos != n.defs_.end(); ++pos)
		o<<*pos<<" ";
	o<<"},INFLUENCE={";
	
	set<int, less<int> >::const_iterator ipos;
	for(ipos = n.infls_.begin(); ipos != n.infls_.end(); ++ipos)
		o<<*ipos<<" ";
	o<<"}, DIRECTSUCCUSS={";
	for(ipos = n.dsuccs_.begin(); ipos != n.dsuccs_.end(); ++ipos)
		o<<*ipos<<" ";
	o<<"}"<<", TYPE={"<<n.type.c_str()<<"}";
	
	o<<", Function={";
	for(pos = n.funcs_.begin(); pos != n.funcs_.end(); ++pos)
		o<<*pos<<" ";
	o<<"}, Module={";
	for(pos = n.mods_.begin(); pos != n.mods_.end(); ++pos)
		o<<*pos<<" ";
	o<<"}";

	for(unsigned idx = 0; idx < n.expr_.count(); ++idx)
	{
		o << " , expression={";
		n.expr_[idx]->dump(o);
		o << ", " << endl;
	}

	o << "}" << endl;

	return o;
}
