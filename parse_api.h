#ifndef __parse_api_H
#define __parse_api_H

#include <stdio.h>
#include <string>
#include <map>

class Module;
class PUdp;

/*
 * These are maps of the modules and primitives parsed from the
 * Verilog source into pform for elaboration. The parser adds modules
 * to these maps as it compiles modules in the verilog source.
 */
extern map<string,Module*> pform_modules;
extern map<string,PUdp*>   pform_primitives;

/*
 * This code actually invokes the parser to make modules. The first
 * parameter is the name of the file that is to be parsed. The
 * optional second parameter is the opened descriptor for the file. If
 * the descriptor is 0 (or skipped) then the function will attempt to
 * open the file on its own.
 */
extern int  pform_parse(const char*path, FILE*file =0);

extern string vl_file;

#endif
