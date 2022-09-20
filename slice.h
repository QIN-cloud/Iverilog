#ifndef __Slice_H_
#define __Slice_H_

#include <set>
#include <map>
#include <iostream>
#include "svector.h"
#include "CfgNode.h"

using namespace std;

class Module;
class PUdp;

typedef struct slicer{
	set<int, less<int> > slice;
	set<string> vars, funcs, mods;
}slicer;

slicer program_slicing(svector<ModuleNode*>*,const char *);

void slice_dump(std::ostream&, Module*, slicer*);

#endif
