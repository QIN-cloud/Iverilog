#ifndef __Design_H
#define __Design_H

#include <map>
#include <string>
#include <iostream>
#include "svector.h"
#include "StringHeap.h"

using namespace std;

class Module;
class ModuleNode;
class PUdp;
class Design;

class PDesign {
  public:
    PDesign();
    ~PDesign();
    
    void set_modules(map<perm_string, Module*> modules);
    void set_udps(map<perm_string, PUdp*>& udps);
    map<perm_string, Module*> get_modules();
    map<perm_string, PUdp*> get_udps();
    
    svector<ModuleNode*>* build_nodes(PDesign& de, string mainmodule);
	  svector<ModuleNode*>* build_nodes(PDesign& de);
	  void build_nodes();
    void dump(ostream&o);
	  void coverage(Design*, string, const char*);
  
  private:
    map<perm_string, Module*> modules_;
    map<perm_string, PUdp*> udps_;
};

#endif
