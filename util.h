#ifndef __util_H
#define __util_H

#include <map>
#include <string>
#include "verinum.h"

class PExpr;
class Design;
class NetScope;

/*
 * This file attempts to locate a module in a file. It operates by
 * looking for a plausible Verilog file to hold the module, and
 * invoking the parser to bring in that file's contents.
 */
extern bool load_module(const char*type);



struct attrib_list_t {
      string key;
      verinum val;
};

extern attrib_list_t* evaluate_attributes(const map<string,PExpr*>&att,
					  unsigned&natt,
					  const Design*des,
					  const NetScope*scope);

#endif
