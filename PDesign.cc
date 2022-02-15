#include "config.h"

#include "PDesign.h"
#include "Module.h"
#include "CfgNode.h"

PDesign::PDesign()
{
}

PDesign::~PDesign()
{
}

void PDesign::set_modules(map<perm_string, Module*> modules)
{
	modules_ = modules;
}

void PDesign::set_udps(map<perm_string, PUdp*>& udps)
{
	udps_ = udps;
}

map<perm_string, Module*> PDesign::get_modules()
{
	return modules_;
}

map<perm_string, PUdp*> PDesign::get_udps()
{
	return udps_;
}

void PDesign::dump(ostream&o)
{
	
}

void PDesign::coverage(Design* des, string module_name, const char* vcd_file)
{
	const char* constc = nullptr;         //初始化const char*类型，并赋值为空
    constc= module_name.c_str();
	perm_string tmp(constc);
	map<perm_string, Module*>::iterator pos;
	pos = modules_.find(tmp);
	if (pos != modules_.end())
	{
		ModuleNode* node = pos->second->build_node(*this);
		pos->second->set_modulenode(node);
		pos->second->build_cfgs();
		pos->second->build_cetable();
		pos->second->coverage(des, vcd_file);
	}
	else
	{
		cerr << "Invalid module or instance name for coverage report: " << module_name << endl;
		exit(1);
	}
}