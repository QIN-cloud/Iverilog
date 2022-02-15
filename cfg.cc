#include "CfgNode.h"
#include "slice.h"
#include "PExpr.h"

Cfg* ModuleNode::build_cfg(ProcessNode* pn)
{
	//new a CFG struct to store the CFG nodes
	Cfg* cfg = new Cfg;
	cfg->id = pn->get_id();
	cfg->lineno = pn->get_lineno();
	cfg->sync = pn->get_sync();
	cfg->root = new svector<Cfg_Node*>(0);
	
	//generate all the nodes for current process
	svector<Node*> nodes(pn->construct_node(2));
	//use this set to store the built node's line no, to avoid 
	//duiplicate building the same node
	unsigned count = 0;
	//add start node of the cfg
	Cfg_Node* start = new Cfg_Node;
	start->lineno = -1;
	directsucc* dsc;
	dsc = new directsucc;
	dsc->index = count + 1;
	start->dsuc = svector<directsucc*>(start->dsuc, dsc);
	start->type = "STARTNODE";
	start->visited = false;
	cfg->root = new svector<Cfg_Node*>(*(cfg->root), start);
	count = count + 1;

	//add normal nodes of the cfg
	for(unsigned i = 0; i < nodes.count(); i++)
	{
		//ignore the process head
		if(nodes[i]->type == "PROCESS") continue;
		/*
		if(i >= 1)
			if((nodes[i]->type == "ISCONTROL.EVENT") && (nodes[i-1]->type == "PROCESS"))
			{
				count = count + 1;
				continue;
			}
		*/
		//add the lineno to root array
		Cfg_Node* tmp;
		tmp = new Cfg_Node;
		tmp->lineno = nodes[i]->lineno_;
		tmp->type = nodes[i]->type;
		tmp->expr = svector<PExpr*>(nodes[i]->expr_);
		tmp->defs = nodes[i]->defs_;
		tmp->refs = nodes[i]->refs_;
		tmp->infls = nodes[i]->infls_;
		tmp->visited = false;
		tmp->condid = -1;
		tmp->exprid = -1;
		tmp->assign_type = nodes[i]->assign_type;
		cfg->root = new svector<Cfg_Node*>(*(cfg->root), tmp);
		
		//add it to map
		cfg->lineno_index[nodes[i]->lineno_] = count;
		//cfg->index_lineno[count] = nodes[i]->lineno_;
		count = count + 1;
	}

	//add end node of the cfg
	Cfg_Node* end = new Cfg_Node;
	end->lineno = -1;
	dsc = new directsucc;
	dsc->index = -1;
	end->dsuc = svector<directsucc*>(end->dsuc, dsc);
	//end->dsuc[0].index = -1;
	end->type = "ENDNODE";
	end->visited = false;
	cfg->root = new svector<Cfg_Node*>(*(cfg->root), end);

	map<unsigned, int>::iterator index, index2;
	//iterate over the nodes to add direct succ for each cfg node
	for(unsigned i = 0; i < nodes.count(); i++)
	{
		//ignore the process head
		if(nodes[i]->type == "PROCESS") continue;
		/*
		if(i >= 1)
		{
			if((nodes[i-1]->type == "PROCESS") && (nodes[i]->type == "ISCONTROL.EVENT"))
			{
				continue;
			}
		}
		*/
		//find index in nodes
		index = cfg->lineno_index.find(nodes[i]->lineno_);
		if(index == cfg->lineno_index.end())
		{
			cerr<<"Error!"<<endl;
			exit(1);
		}
		//the last line has none direct success
		if(nodes[i]->dsuccs_.size() == 0)
		{
			directsucc* dsctmp = new directsucc;
			dsctmp->index = cfg->root->count() - 1;
			(*(cfg->root))[index->second]->dsuc = svector<directsucc*>((*(cfg->root))[index->second]->dsuc,dsctmp);
		}
		//match direct success and caseitem
		else
		{
			count = 0;//number of direct success 
			unsigned itemcount = 0;// number of direct success caseitem
			for(set<int, less<int> >::iterator ipos = nodes[i]->dsuccs_.begin();
				ipos != nodes[i]->dsuccs_.end();
				++ipos)
			{
				for(unsigned j = 0; j < cfg->lineno_index.size() - index->second + 1; ++j)
				{
					index2 = cfg->lineno_index.find(*ipos + j);
					if(index2 != cfg->lineno_index.end()) break;
				}
				directsucc* dsctmp2 = new directsucc;
				if(index2->second == i)
					dsctmp2->index = index2->second + 1;
				else 
					dsctmp2->index = index2->second;
				//there are two Cfg_Nodes next
				if((nodes[i]->type == "ISCONTROL.CONDITION")||(nodes[i]->type == "ISCONTROL.LOOP")||(nodes[i]->type == "ISCONTROL.EVENT"))
				{
					if(count == 0)
						dsctmp2->cond = true;
					else
						dsctmp2->cond = false;
				}
				//there are more Cfg_Nodes next
				else if((nodes[i]->type == "ISCONTROL.CASE") || (nodes[i]->type == "ISCONTROL.CASEX") || (nodes[i]->type == "ISCONTROL.CASEZ")) //is case statement
				{
					if(nodes[index2->second]->caseitem_.size() > 0)
					{
						for(list<PExpr*>::iterator eidx = nodes[index2->second]->caseitem_.begin();
						 eidx != nodes[index2->second]->caseitem_.end(); ++eidx)
							dsctmp2->caseitem.insert(*eidx);
					}
					else //default
						dsctmp2->cond = false;
					dsctmp2->caseitemidx = itemcount;
					itemcount += 1;
				}
				(*(cfg->root))[index->second]->dsuc = svector<directsucc*>((*(cfg->root))[index->second]->dsuc, dsctmp2);
				
				count = count + 1;
			}
			//Process line
			if((nodes[i]->type.find("ISCONTROL") == 0)&& (nodes[i]->dsuccs_.size() == 1))
			{
				directsucc* dsctmp3 = new directsucc;
				dsctmp3->index = cfg->root->count() - 1;
				dsctmp3->cond = false;
				(*(cfg->root))[index->second]->dsuc = svector<directsucc*>((*(cfg->root))[index->second]->dsuc, dsctmp3); 
			}
		}
	}

	for(int index = 0; index < nodes.count(); index++)
	{
		delete nodes[index];
	}

	//add all variables to this cfg set
	for(unsigned nidx = 0; nidx < cfg->root->count(); ++nidx)
	{
		for(set<string>::const_iterator spos = (*(cfg->root))[nidx]->defs.begin(); spos != (*(cfg->root))[nidx]->defs.end(); ++spos)
		{
			cfg->defs.insert(*spos);
		}
		for(set<string>::const_iterator spos = (*(cfg->root))[nidx]->refs.begin(); spos != (*(cfg->root))[nidx]->refs.end(); ++spos)
		{
			cfg->refs.insert(*spos);
		}
	}
	return cfg;
}

Module_Cfgs* ModuleNode::build_cfgs()
{
	Module_Cfgs* mcs = new Module_Cfgs;
	mcs->cfgs = new svector<Cfg*>(0);
	Cfg* cfg;
	for(unsigned idx = 0; idx <procs_.count(); ++idx)
	{
		cfg = build_cfg(procs_[idx]);
		cfg->id = idx;
		mcs->cfgs = new svector<Cfg*>((*mcs->cfgs), cfg); 
	}

	return mcs;
}

//Dump every line(Cfg_Node) about direct success lines and conditions 
//Conditions maybe singel or mutible,so use if-else to depart the situation
std::ostream& operator << (std::ostream& o, const Cfg_Node& cn)
{
	o << cn.type<< " Line Number: " << cn.lineno<<",";
	set<directsucc*>::const_iterator pos;
	if(cn.dsuc.count() == 0)
		o << ", This is the last line";
	else if(cn.dsuc.count() == 1)
	{
		o << " The Direct Successor is " << cn.dsuc[0]->index;
		o << ", The condition expression ID is " << cn.condid;
	}
	else
	{
		o << "The condition expression ID =" << cn.condid;
		for(unsigned pos = 0; pos < cn.dsuc.count(); ++pos)
		{
			if(cn.dsuc[pos]->caseitem.size() == 0)
			{
				o << ", " << cn.dsuc[pos]->cond << " Path : " << cn.dsuc[pos]->index;
			}
			else
			{
				for(set<PExpr*>::const_iterator idx = cn.dsuc[pos]->caseitem.begin(); idx != cn.dsuc[pos]->caseitem.end(); ++idx)
				{
					o << ", ";
					(*idx)->dump(o);
				}
				o <<  " : Path : " << cn.dsuc[pos]->index;
			}
		}
	}
	o << endl;

	return o;
}
