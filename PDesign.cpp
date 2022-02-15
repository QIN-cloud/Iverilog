#include "config.h"

#include <queue>
#include "PDesign.h"
#include "Module.h"
#include "CfgNode.h"
#include "PGate.h"

PDesign::PDesign()
{
}

PDesign::~PDesign()
{
}

void PDesign::set_modules(map<string, Module*>& modules)
{
	modules_ = modules;
}

void PDesign::set_udps(map<string, PUdp*>& udps)
{
	udps_ = udps;
}

map<string, Module*> PDesign::get_modules()
{
	return modules_;
}

map<string, PUdp*> PDesign::get_udps()
{
	return udps_;
}

void PDesign::dump(ostream&o)
{
	
}

void PDesign::pathcover(Design* des, /*Top module name*/string module_name, const char* vcd_file, ostream& target_file, int type)
{
	map<string, Module*>::iterator pos;
	pos = modules_.find(module_name);
	if (pos != modules_.end())
	{
		//I add code to measure coverage of the modules instantiated in the top module
		set<string> cover_modules;
		queue<string> qmodules;
		cover_modules.clear();
		cover_modules.insert(module_name);
		qmodules.push(module_name);
		while(!qmodules.empty())
		{
			map<string, Module*>::iterator mpos;
			string& stmp = qmodules.front();
			mpos = modules_.find(stmp);
			qmodules.pop();
			list<PGate*> inst_modules = mpos->second->get_gates();
			for(list<PGate*>::iterator gpos = inst_modules.begin(); gpos != inst_modules.end(); ++gpos)
			{
				if((*gpos)->ty == PGate::MODULE)
				{
					cover_modules.insert((*gpos)->get_type());
					qmodules.push((*gpos)->get_type());
				}
			}
		}

		for(set<string>::iterator spos = cover_modules.begin(); spos != cover_modules.end(); ++spos)
		{
			map<string, Module*>::iterator mpos;
			mpos = modules_.find(module_name);
			if(mpos != modules_.end())
			{
				ModuleNode* node = mpos->second->build_node(*this);
				mpos->second->set_modulenode(node);
				mpos->second->build_cfgs();
				mpos->second->build_cetable();
				mpos->second->pathcover(des, vcd_file, target_file, type);
			}
			else
			{
				cerr << "Invalid module or instance name for coverage report: " << module_name << endl;
				exit(1);
			}
		}
	}
	else
	{
		cerr << "Invalid module or instance name for coverage report: " << module_name << endl;
		exit(1);
	}
}

void PDesign::togglecover(string cover_module, const char* vcd_file, ostream& report)
{
	report << "TOGGLE COVERAGE RESULTS BY MODULE\n";
    report << "---------------------------------\n";
    
	map<string, Module*>::iterator pos;
	pos = modules_.find(cover_module);
	if (pos != modules_.end())
	{
		pos->second->togglecover(vcd_file, report);
	}
}


void PDesign::fsmcov(string cover_module, const char* vcd_file, string fsm_vars, ostream& report)
{
	report << "FSM COVERAGE RESULTS BY MODULE\n";
    report << "---------------------------------\n";

	map<string, Module*>::iterator pos;
	pos = modules_.find(cover_module);
	if (pos != modules_.end())
	{
		pos->second->fsmcover(vcd_file, fsm_vars, report);
	}
}