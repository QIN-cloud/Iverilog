#ifndef __Design_H
#define __Design_H

#include <map>
#include <string>
#include <iostream>
#include "svector.h"

class Module;
class ModuleNode;
class PUdp;
class Design;

class PDesign {
  public:
    PDesign();
    ~PDesign();
    
    void set_modules(map<string, Module*>& modules);
    void set_udps(map<string, PUdp*>& udps);
    map<string, Module*> get_modules();
    map<string, PUdp*> get_udps();
    
    svector<ModuleNode*>* build_nodes(PDesign& de, string mainmodule);
	svector<ModuleNode*>* build_nodes(PDesign& de);
	void build_nodes();
    void dump(ostream&o);
	
	void pathcover(Design*, string, const char*, ostream&, int);
	void togglecover(string cover_module, const char* vcd_file, ostream& report);
	void fsmcov(string cover_module, const char* vcd_file, string fsm_vars, ostream& report);
  
  private:
    map<string, Module*> modules_;
    map<string, PUdp*> udps_;
};

#endif
