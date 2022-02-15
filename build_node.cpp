#include "config.h"
#include "Statement.h"
#include "CfgNode.h"
#include "PDesign.h"
#include "Module.h"
#include "PTask.h"
#include "PWire.h"
#include "PExpr.h"
#include "PGate.h"
#include "PUdp.h"

CfgNode* Statement::build_node(PDesign& de)
{
	return 0;
}

CfgNode* PAssign::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	unsigned idx;
	for(idx = 0; idx < rval_->vars_count(); ++idx)
	  node->add_ref(rval_->get_var(idx));
	for(idx = 0; idx < lval_->vars_count(); ++idx)
	  node->add_def(lval_->get_var(idx));

	node->add_funcs(lval_->get_funcname());
	node->add_funcs(rval_->get_funcname());

	node->set_expr(rval_);
	node->set_assign_type("NOCONTROL.BLOCKING");
	  
	//node->dump(cerr);
	return node;
}

CfgNode* PAssignNB::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	unsigned idx;
	for(idx = 0; idx < rval_->vars_count(); ++idx)
	  node->add_ref(rval_->get_var(idx));
	for(idx = 0; idx < lval_->vars_count(); ++idx)
	  node->add_def(lval_->get_var(idx));
	
	node->add_funcs(lval_->get_funcname());
	node->add_funcs(rval_->get_funcname());

	node->set_expr(rval_);
	node->set_assign_type("NOCONTROL.NONBLOCKING");

	return node;
}

CfgNode* PBlock::build_node(PDesign& de)
{
	BlockNode* bn = new BlockNode();
	for(unsigned idx = 0; idx < list_.count(); ++idx)
	{
	  CfgNode* node = list_[idx]->build_node(de);
	  bn->add_node(node);
	  if(idx == 0)
		  bn->set_lineno(node->get_lineno());
	}
	return bn;
}

CfgNode* PCallTask::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	if(path_.peek_name(0)[0] == '$'){
	    cerr<<"System call, Not supported yet!"<<endl;
	    exit(0);
	}
	map<string, Module*> modules = de.get_modules();
	map<string, Module*>::iterator pos;
	string tmps(path_.peek_name(path_.component_count()-1));
	PTask* task;
	//find the task definition
	for(pos = modules.begin(); pos != modules.end(); ++pos)
	{
	    task = pos->second->get_task(tmps);
	    if(task != 0)
	    {
		if(task->port_count() != parms_.count())
		{
		    cerr<<get_line()<<" : Task Call with incorrect number of parameters, I will give up!"<<endl;
		    exit (0);
		}
		CfgNode* node = new CfgNode(get_lineno());
		for(unsigned idx = 0; idx < task->port_count(); ++idx)
		{
			switch(task->get_port(idx)->get_port_type()){
				case NetNet::PINPUT:
					node->add_refs(parms_[idx]->get_vars());
					break;
				case NetNet::POUTPUT:
					node->add_defs(parms_[idx]->get_vars());
					break;
				case NetNet::PINOUT:
					node->add_refs(parms_[idx]->get_vars());
					node->add_defs(parms_[idx]->get_vars());
					break;
			}
		}
		set<string> tmp;
		tmp.insert(tmps);
		node->add_funcs(tmp);
		return node;
	    }
	}
}

CfgNode* PCase::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
    CaseNode* cn = new CaseNode(get_lineno());
    svector<CfgNode*> nodes;
    CfgNode* rootnode;
    
    rootnode = new CfgNode(get_lineno());
    rootnode->add_refs(expr_->get_vars());
	rootnode->set_expr(expr_);
	switch(type_)
	{
	case NetCase::EQ:
		rootnode->set_casetype(0);
		break;
	case NetCase::EQX:
		rootnode->set_casetype(1);
		break;
	case NetCase::EQZ:
		rootnode->set_casetype(2);
		break;
	}
    cn->add_node(rootnode);

    for(unsigned idx = 0; idx < items_->count(); ++idx)
    {
		CfgNode* node = (*items_)[idx]->stat->build_node(de);
		node->set_caseitem((*items_)[idx]->expr);
		cn->get_node(0)->add_infl((*items_)[idx]->stat->get_linenos());
		cn->get_node(0)->add_dsucc(node->get_lineno());
		cn->add_node(node);	
    }
    //cn->dump(cerr);
    return cn;
}

CfgNode* PCAssign::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	node->add_refs(expr_->get_vars());
	node->add_defs(lval_->get_vars());
	node->add_funcs(lval_->get_funcname());
	node->add_funcs(expr_->get_funcname());
	node->set_expr(lval_);
	//node->dump(cerr);
	return node;
}

CfgNode* PCondit::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CondNode* cn = new CondNode(get_lineno());
	CfgNode* root = new CfgNode(get_lineno());
	root->add_refs(expr_->get_vars());
	root->add_funcs(expr_->get_funcname());
	root->set_expr(expr_);

	if(if_)
	{
		root->add_infl(if_->get_linenos());
		root->add_dsucc(if_->get_lineno());
	}
	if(else_)
	{
		root->add_infl(else_->get_linenos());
		root->add_dsucc(else_->get_lineno());
	}
	cn->add_node(root, 0);
	if(if_)
	{
		CfgNode* ifnode = if_->build_node(de);
		cn->add_node(ifnode, 1);
	}
	else
		cn->add_node(NULL, 1);
	if(else_)
	{
		CfgNode* elsenode = else_->build_node(de);
		cn->add_node(elsenode, 2);
	}
	else
		cn->add_node(NULL, 2);
	//cn->dump(cerr);
	return cn;
}

CfgNode* PDeassign::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	node->add_defs(lval_->get_vars());
	node->add_funcs(lval_->get_funcname());
	node->set_expr(lval_);

	//node->dump(cerr);
	return node;
}

CfgNode* PDelayStatement::build_node(PDesign& de)
{
	EventNode* en = new EventNode(get_lineno());
	CfgNode* node = new CfgNode(get_lineno());
	node->add_refs(delay_->get_vars());
	node->add_funcs(delay_->get_funcname());

	en->add_node(node, 0);
	if(statement_){
		CfgNode* tmp = statement_->build_node(de);
		en->add_node(tmp, 1);
	}
	return en;
}

CfgNode* PEventStatement::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	EventNode* en = new EventNode(get_lineno());
	CfgNode* node = new CfgNode(get_lineno());
	for(unsigned idx = 0; idx < expr_.count(); ++idx)
	{
		node->add_refs(expr_[idx]->get_vars());
		node->add_funcs(expr_[idx]->get_funcname());
	}
	if(statement_){
		CfgNode* tmp = statement_->build_node(de);
		node->add_infl(statement_->get_linenos());
		set<int, less<int> > infls = node->get_infls();
		if(infls.size() > 0){
		  set<int, less<int> >::iterator ipos;
		  ipos = infls.begin();
  		  node->add_dsucc(*ipos);
		}
		en->add_node(tmp, 1);
	}
	en->add_node(node, 0);
	//en->dump(cerr);
	return en;
}

CfgNode* PForce::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	node->add_refs(expr_->get_vars());
	node->add_defs(lval_->get_vars());
	node->add_funcs(lval_->get_funcname());
	node->add_funcs(expr_->get_funcname());
	node->set_expr(lval_);

	return node;
}

CfgNode* PForStatement::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	LoopNode* fn = new LoopNode(get_lineno());
	CfgNode* root = new CfgNode(get_lineno());
	root->add_refs(name1_->get_vars());
	root->add_refs(expr1_->get_vars());
	root->add_refs(cond_->get_vars());
	root->add_refs(name2_->get_vars());
	root->add_refs(expr2_->get_vars());
	root->add_funcs(name1_->get_funcname());
	root->add_funcs(expr1_->get_funcname());
	root->add_funcs(cond_->get_funcname());
	root->add_funcs(expr2_->get_funcname());
	root->add_funcs(name2_->get_funcname());
	root->set_expr(cond_);

	if(statement_){
		root->add_infl(statement_->get_linenos());
		root->add_dsucc(statement_->get_lineno());
	}
	fn->add_node(root, 0);
	if(statement_){
		CfgNode* node = statement_->build_node(de);
		node->add_dsucc(root->get_lineno());
		fn->add_node(node, 1);
	}
	//delete root;
	//delete node;
	//fn->dump(cerr);
	return fn;
}

CfgNode* PRepeat::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	LoopNode* fn = new LoopNode(get_lineno());
	CfgNode* root = new CfgNode(get_lineno());
	root->add_refs(expr_->get_vars());
	root->add_funcs(expr_->get_funcname());
	root->set_expr(expr_);

	if(statement_){
		root->add_infl(statement_->get_linenos());
		root->add_dsucc(statement_->get_lineno());
		CfgNode* node = statement_->build_node(de);
		node->add_dsucc(root->get_lineno());
		fn->add_node(node, 1);
	}
	fn->add_node(root, 0);
	//delete root;
	//delete node;
	//fn->dump(cerr);
	return fn;
}

CfgNode* PRelease::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	node->add_defs(lval_->get_vars());
	node->add_funcs(lval_->get_funcname());
	node->set_expr(lval_);

	//node->dump(cerr);
	return node;
}

CfgNode* PTrigger::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	string tmps(event_.peek_name(event_.component_count()-1));
	node->add_def(tmps);
	//node->dump(cerr);
	return node;
}

CfgNode* PWhile::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	LoopNode* fn = new LoopNode(get_lineno());
	CfgNode* root = new CfgNode(get_lineno());
	root->add_refs(cond_->get_vars());
	root->add_funcs(cond_->get_funcname());
	root->set_expr(cond_);

	if(statement_){
		root->add_infl(statement_->get_linenos());
		root->add_dsucc(statement_->get_lineno());
		CfgNode* node = statement_->build_node(de);
		node->add_dsucc(root->get_lineno());
		fn->add_node(node, 1);
	}
	fn->add_node(root, 0);
	//delete root;
	//delete node;
	//fn->dump(cerr);
	return fn;
}

ProcessNode* PProcess::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	ProcessNode* pn = new ProcessNode(get_lineno());
	pn->set_id(id_);
	pn->set_sync(is_synchronous());
	CfgNode* root = new CfgNode(get_lineno());	
	//root->add_infl(statement_->get_linenos());
	if(statement_){
		CfgNode* node = statement_->build_node(de);
		pn->add_node(node, 1);
	}
	//root->add_refs(node->get_refs());
	//root->add_defs(node->get_defs());
	pn->add_node(root, 0);
	//pn->dump(cerr);
	return pn;
}

CfgNode* PGAssign::build_node(PDesign& de)
{
	CfgNode* node = new CfgNode(get_lineno());
	node->add_defs((*pins_)[0]->get_vars());
	node->add_refs((*pins_)[1]->get_vars());
	node->add_refs(delay_.get_vars());
	node->add_funcs((*pins_)[0]->get_funcname());
	node->add_funcs((*pins_)[1]->get_funcname());
	return node;
}

CfgNode* PGBuiltin::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	CfgNode* node = new CfgNode(get_lineno());
	if (pin_count()) {
	    if (pin(0)) node->add_defs((*pins_)[0]->get_vars());
			      
	    for (unsigned idx = 1 ;  idx < pin_count() ;  idx += 1) {
		  if (pin(idx)) node->add_refs((*pins_)[idx]->get_vars());
	    }
        }
	node->add_refs(delay_.get_vars());
	node->add_refs(msb_->get_vars());
	node->add_refs(lsb_->get_vars());
	//node->dump(cerr);
	return node;
}

CfgNode* PGModule::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	assert(get_name() != "");
	
	if (msb_) {
	    cerr << get_line() << ": sorry: Module instantiation arrays "
		  "are not yet supported." << endl;
	    return 0;
        }
        
	CfgNode* node = new CfgNode(get_lineno());
	map<string, Module*> modules = de.get_modules();
	map<string, Module*>::iterator mpos = modules.find(type_);
	if(mpos != modules.end())
	{
		if(pins)
			{
			  if(npins_ != mpos->second->port_count())
			  {
			    cerr<<get_line()<<" : Module instance with incorrect port number!"<<endl;
			    exit(0);
			  }
			  map<hname_t,PWire*> wires = mpos->second->get_wires();
			  map<hname_t,PWire*>::iterator wpos;
			  if(npins_ > 0)
			  {
				  for(unsigned i = 0; i < npins_; ++i)
				  {
					  hname_t tmp(pins[i].name.c_str());
					  wpos = wires.find(tmp);
					  if(wpos != wires.end())
						  if(wpos->second->get_port_type() == NetNet::PINPUT)
						  {
							  if(pins[i].parm)
								  node->add_refs(pins[i].parm->get_vars());
						  }
						  else if(wpos->second->get_port_type() == NetNet::POUTPUT)
						  {
							  if(pins[i].parm)
								  node->add_defs(pins[i].parm->get_vars());
						  }
						  else if(wpos->second->get_port_type() == NetNet::PINOUT)
						  {
							  if(pins[i].parm)
							  {
								  node->add_refs(pins[i].parm->get_vars());
								  node->add_defs(pins[i].parm->get_vars());
							  }
						  }
				  }
			  }
			}
			else
			{
			  if (pin_count()) {
			  	if(pin_count() != mpos->second->port_count())
			  	{
			    		cerr<<get_line()<<" : Module instance with incorrect port number!"<<endl;
			    		exit(0);
			  	}
			  	map<hname_t,PWire*> wires2 = mpos->second->get_wires();
			  	map<hname_t,PWire*>::iterator wpos2;
				for(unsigned j = 0; j < pin_count(); ++j)
				{
					hname_t tmps(mpos->second->get_port_name(j).c_str());
			    		wpos2 = wires2.find(tmps);
		     		        if(wpos2 != wires2.end())
			                  if(wpos2->second->get_port_type() == NetNet::PINPUT)
			            	    node->add_refs((*pins_)[j]->get_vars());
			        	  else if(wpos2->second->get_port_type() == NetNet::POUTPUT)
			            	    node->add_defs((*pins_)[j]->get_vars());
			        	  else if(wpos2->second->get_port_type() == NetNet::PINOUT)
			        	  {
			        	    node->add_refs((*pins_)[j]->get_vars());
			        	    node->add_defs((*pins_)[j]->get_vars());
			        	  }
			  	}
			  }
			}
			node->add_funcs(get_funcname());
			node->add_mods(get_modname());
			//node->dump(cerr);
			return node;
	}
	
	
	map<string, PUdp*> udps = de.get_udps();
	map<string, PUdp*>::iterator upos = udps.find(type_);
	if(upos != udps.end())
	{
		if(pins)
			{
			  if(npins_ != upos->second->ports.count())
			  {
			    cerr<<get_line()<<" : Udp instance with incorrect port number!"<<endl;
			    exit(0);
			  }
			  node->add_defs(pins[0].parm->get_vars());
			  for(unsigned l = 1; l < npins_; ++l)
			    node->add_refs(pins[l].parm->get_vars());
			}
			else
			{
				if(pin_count())
				{
					if(pin_count() != upos->second->ports.count())
					{
			    			cerr<<get_line()<<" : Udp instance with incorrect port number!"<<endl;
			    			exit(0);
			    		}
			    		node->add_defs((*pins_)[0]->get_vars());
			  		for(unsigned m = 1; m < upos->second->ports.count(); ++m)
			    		    node->add_refs((*pins_)[m]->get_vars());
			  	}				
			}
			node->add_funcs(get_funcname());
			node->add_mods(get_modname());
			//node->dump(cerr);
			return node;
	}
	
	cerr<<get_line()<<": No such Module or Udp defined in this design!"<<endl;
	exit (0);
}

ProcessNode* PTask::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	ProcessNode* tn = new ProcessNode(get_lineno());
	if(statement_){
		CfgNode* node = statement_->build_node(de);
		tn->add_node(node, 1);
	}
	tn->add_node(0,0);
	//tn->dump(cerr);
	return tn;
}

ProcessNode* PFunction::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	ProcessNode* fn = new ProcessNode(get_lineno());
	if(statement_){
		CfgNode* node = statement_->build_node(de);
		fn->add_node(node, 1);
	}
	fn->add_node(0,0);
	//fn->dump(cerr);
	return fn;
}

ModuleNode* Module::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	ModuleNode* md = new ModuleNode(get_lineno());
	//unsigned idx;
	list<PGate*>::iterator pos;
	for(pos = gates_.begin(); pos != gates_.end(); ++pos)
	{
		CfgNode* node = (*pos)->build_node(de);
		md->add_node(node);
	}
	list<PProcess*>::iterator ppos;
	for(ppos = behaviors_.begin(); ppos != behaviors_.end(); ++ppos)
	{
		ProcessNode* node = (*ppos)->build_node(de);
		md->add_node(node, 0);
	}
	map<string,PFunction*>::iterator fpos;
	for(fpos = funcs_.begin(); fpos != funcs_.end(); ++fpos)
	{
		ProcessNode* node = (*fpos).second->build_node(de);
		md->add_node(node,1);
	}
	map<string, PTask*>::iterator tpos;
	for(tpos = tasks_.begin(); tpos != tasks_.end(); ++tpos)
	{
		ProcessNode* node = (*tpos).second->build_node(de);
		md->add_node(node,2);
	}
	//md->dump(cerr);
	return md;
}

svector<ModuleNode*>* PDesign::build_nodes(PDesign& de, string mainmodule)
{
	svector<ModuleNode*>* nodes = new svector<ModuleNode*>();
	map<string, Module*>::iterator pos;
	pos = modules_.find(mainmodule);
	if (pos != modules_.end())
	{
		ModuleNode* node = pos->second->build_node(de);
		nodes = new svector<ModuleNode*>(*nodes, node);
	}
	else
	{
		cerr << "Invalid module name for program slicing: " << mainmodule << endl;
		exit(1);
	}
	return nodes;
}

svector<ModuleNode*>* PDesign::build_nodes(PDesign& de)
{
	svector<ModuleNode*>* nodes = new svector<ModuleNode*>();
	map<string, Module*>::iterator pos;
	for(pos = modules_.begin(); pos != modules_.end(); ++pos)
	{
		ModuleNode* node = pos->second->build_node(de);
		nodes = new svector<ModuleNode*>(*nodes, node);
	}
	return nodes;
}

void PDesign::build_nodes()
{
	map<string, Module*>::iterator pos;
	for(pos = modules_.begin(); pos != modules_.end(); ++pos)
	{
		ModuleNode* node = pos->second->build_node(*this);
		pos->second->set_modulenode(node);
	}
}

CfgNode* PForever::build_node(PDesign& de)
{
	//cerr<<"Build node for:"<<get_line()<<endl;
	LoopNode* ln = new LoopNode(get_lineno());
	CfgNode* root = new CfgNode(get_lineno());
	if(statement_){
		CfgNode* node = statement_->build_node(de);
		root->add_dsucc(statement_->get_lineno());
		root->add_infl(statement_->get_linenos());
		node->add_dsucc(get_lineno());
		ln->add_node(node, 1);
	}
	ln->add_node(root, 0);
	//ln->dump(cerr);
	return ln;
}
